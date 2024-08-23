#ifndef __NC_JSON_INL__
#define __NC_JSON_INL__

#pragma once

#define JSON_PARSE_CHECK_(expression, type)                                                                 \
    JSON_INTERNAL_ASSERT_CHECK_EX (expression, "Parse error: in=%s pos=%u.",                                \
                                                convert_json_string(JSON_TSTRING(type)(in, len)).c_str (),  \
                                                static_cast<unsigned int> (pos))
#define JSON_PARSE_CHECK(expression)  JSON_PARSE_CHECK_ (expression, char_t)
#define JSON_PARSE_CHECKA(expression) JSON_PARSE_CHECK_ (expression, char)
#define JSON_PARSE_CHECKW(expression) JSON_PARSE_CHECK_ (expression, wchar_t)

#define JSON_DECODE_CHECK(expression)                                                                           \
    JSON_INTERNAL_ASSERT_CHECK_EX (expression, "Decode error: in=%s.",                                          \
                                                convert_json_string(JSON_TSTRING(char_t)(in, len)).c_str())

namespace JSON {

inline std::string
convert_json_string (const JSON_TSTRING (char) & str)
{
    return str;
}

// inline std::string
// convert_json_string (const JSON_TSTRING (wchar_t) & wstr)
// {
//     String str;
//     str.format ("%S", wstr.c_str ());
//     return str;
// }

inline const char*
get_type_name (int type)
{
    switch (type)
    {
        case NIL:
            return "Null";
        case INTEGER:
            return "Integer";
        case FLOAT:
            return "Float";
        case BOOLEAN:
            return "Boolean";
        case STRING:
            return "String";
        case OBJECT:
            return "Object";
        case ARRAY:
            return "Array";
    }
    return "Unknown";
}

template<class char_t>
ValueT<char_t>::ValueT (Type type)
    : _type (type)
{
    switch (_type)
    {
        case NIL:
            break;
        case INTEGER:
            _integer = 0;
            break;
        case FLOAT:
            _float = 0;
            break;
        case BOOLEAN:
            _boolean = false;
            break;
        case STRING:
            _needConv = true;
            break;
        case OBJECT:
            break;
        case ARRAY:
            break;
    }
}

template<class char_t>
ValueT<char_t>::ValueT (const ValueT<char_t> &v)
    : _type (v._type)
{
    switch (_type)
    {
        case NIL:
            _type = NIL;
            break;
        case INTEGER:
            _integer = v._integer;
            break;
        case FLOAT:
            _float = v._float;
            break;
        case BOOLEAN:
            _boolean = v._boolean;
            break;
        case STRING:
            _needConv = v._needConv;
            _string   = v._string;
            break;
        case ARRAY:
            _array = v._array;
            break;
        case OBJECT:
            _object = v._object;
            break;
    }
}

template<class char_t>
void
ValueT<char_t>::assign (const ValueT<char_t> &v)
{
    if (_type != v._type)
    {
        clear ();
    }
    _type = v._type;
    switch (_type)
    {
        case NIL:
            _type = NIL;
            break;
        case INTEGER:
            _integer = v._integer;
            break;
        case FLOAT:
            _float = v._float;
            break;
        case BOOLEAN:
            _boolean = v._boolean;
            break;
        case STRING:
            _needConv = v._needConv;
            _string   = v._string;
            break;
        case ARRAY:
            _array = v._array;
            break;
        case OBJECT:
            _object = v._object;
            break;
    }
}

#ifdef __ENABLE_CXX11__
template<class char_t>
void
ValueT<char_t>::assign (ValueT<char_t>&& v)
{
    if (_type != v._type)
    {
        clear ();
    }
    _type = v._type;
    switch (_type)
    {
        case NIL:
            _type = NIL;
            break;
        case INTEGER:
            _integer = v._integer;
            break;
        case FLOAT:
            _float = v._float;
            break;
        case BOOLEAN:
            _boolean = v._boolean;
            break;
        case STRING:
            _needConv = v._needConv;
            _string   = JSON_MOVE (v._string);
            break;
        case ARRAY:
            _array = JSON_MOVE (v._array);
            break;
        case OBJECT:
            _object = JSON_MOVE (v._object);
            break;
    }
    v._type = NIL;
}
#endif

template<class char_t>
void
ValueT<char_t>::clear ()
{
    switch (_type)
    {
        case STRING:
            _string.clear ();
            break;
        case ARRAY:
            _array.clear ();
            break;
        case OBJECT:
            _object.clear ();
            break;
        default:
            break;
    }
    _type = NIL;
}

namespace detail {
    namespace {
        template<class T, class char_t>
        inline void
        internal_to_string (const T& v,
                            JSON_TSTRING (char_t) & out,
                            int (*fmter) (char_t*, size_t, const char_t*, ...),
                            const char_t* fmt)
        {
            // double 24 bytes, int64 20 bytes
            static const size_t bufSize = 25;
            const size_t        len     = out.length ();
            out.resize (len + bufSize);
            int ret = fmter (&out[0] + len, bufSize, fmt, v);
            if (ret == bufSize || ret < 0)
            {
                JSON_INTERNAL_ASSERT_CHECK_EX(false, "Format error.");
            }
            out.resize (len + ret);
        }

        template<class T, class char_t>
        void to_string (const T& v, JSON_TSTRING (char_t) & out);

        template<>
        inline void
        to_string<int64_t, char> (const int64_t& v, JSON_TSTRING (char) & out)
        {
            internal_to_string<int64_t, char> (v, out,
        #ifdef __WINDOWS__
                                            _snprintf, "%I64d"
        #else
                                            snprintf, "%lld"
        #endif
            );
        }

        template<>
        inline void
        to_string<int64_t, wchar_t> (const int64_t& v, JSON_TSTRING (wchar_t) & out)
        {
            internal_to_string<int64_t, wchar_t> (v, out, swprintf,
        #ifdef __WINDOWS__
                                                L"%I64d"
        #else
                                                L"%lld"
        #endif
            );
        }

        template<>
        inline void
        to_string<double, char> (const double& v, JSON_TSTRING (char) & out)
        {
            internal_to_string<double, char> (v, out,
        #ifdef __WINDOWS__
                                            _snprintf
        #else
                                            snprintf
        #endif
                                            ,
                                            "%.16g");
        }

        template<>
        inline void
        to_string<double, wchar_t> (const double& v, JSON_TSTRING (wchar_t) & out)
        {
            internal_to_string<double, wchar_t> (v, out, swprintf, L"%.16g");
        }

        template<class T, class char_t>
        inline JSON_TSTRING (char_t) to_string (const T& v)
        {
            JSON_TSTRING (char_t)
            out;
            to_string (v, out);
            return JSON_MOVE (JSON_TSTRING (char_t) (out));
        }

        char
        int_to_hex (int n)
        {
            return n["0123456789abcdef"];
        }

        void
        to_hex (int ch, JSON_TSTRING (wchar_t) & out)
        {
            out += int_to_hex ((ch >> 4) & 0xF);
            out += int_to_hex (ch & 0xF);
        }

        template<int size>
        void encode_unicode (wchar_t ch, JSON_TSTRING (wchar_t) & out);

        // For UTF16 Encoding
        template<>
        void
        encode_unicode<2> (wchar_t ch, JSON_TSTRING (wchar_t) & out)
        {
            out += '\\';
            out += 'u';
            to_hex ((ch >> 8) & 0xFF, out);
            to_hex (ch & 0xFF, out);
        }

        // For UTF32 Encoding
        template<>
        void
        encode_unicode<4> (wchar_t ch, JSON_TSTRING (wchar_t) & out)
        {
            if (ch > 0xFFFF)
            {
                ch = static_cast<int> (ch) - 0x10000;
                encode_unicode<2> ((unsigned short)(0xD800 | (ch >> 10)), out);
                encode_unicode<2> ((unsigned short)(0xDC00 | (ch & 0x03FF)), out);
            }
            else
            {
                encode_unicode<2> ((unsigned short)ch, out);
            }
        }

        inline void
        encode (const char* in, size_t len, JSON_TSTRING (char) & out)
        {
            for (size_t pos = 0; pos < len; ++pos)
            {
                switch (in[pos])
                {
                    case '\"':
                        out += "\\\"";
                        break;
                    case '\\':
                        out += "\\\\";
                        break;
                    case '/':
                        out += "\\/";
                        break;
                    case '\b':
                        out += "\\b";
                        break;
                    case '\f':
                        out += "\\f";
                        break;
                    case '\n':
                        out += "\\n";
                        break;
                    case '\r':
                        out += "\\r";
                        break;
                    case '\t':
                        out += "\\t";
                        break;
                    default:
                        out += in[pos];
                        break;
                }
            }
        }

        inline void
        encode (const wchar_t* in, size_t len, JSON_TSTRING (wchar_t) & out)
        {
            for (size_t pos = 0; pos < len; ++pos)
            {
                if (in[pos] > 127)
                {
                    encode_unicode<sizeof (wchar_t)> (in[pos], out);
                }
                else
                {
                    switch (in[pos])
                    {
                        case '\"':
                            out += L"\\\"";
                            break;
                        case '\\':
                            out += L"\\\\";
                            break;
                        case '/':
                            out += L"\\/";
                            break;
                        case '\b':
                            out += L"\\b";
                            break;
                        case '\f':
                            out += L"\\f";
                            break;
                        case '\n':
                            out += L"\\n";
                            break;
                        case '\r':
                            out += L"\\r";
                            break;
                        case '\t':
                            out += L"\\t";
                            break;
                        default:
                            out += in[pos];
                            break;
                    }
                }
            }
        }

        int
        hex_to_int (int ch)
        {
            if ('0' <= ch && ch <= '9')
                return (ch - '0');
            else if ('a' <= ch && ch <= 'f')
                return (ch - 'a' + 10);
            else if ('A' <= ch && ch <= 'F')
                return (ch - 'A' + 10);
            JSON_INTERNAL_ASSERT_CHECK_EX(false, "Decode error: invalid character=0x%x.", ch);
        }

        template<class char_t>
        unsigned short
        hex_to_ushort (const char_t* in, size_t len)
        {
            JSON_DECODE_CHECK (len >= 4);
            unsigned char highByte = (hex_to_int (in[0]) << 4) | hex_to_int (in[1]);
            unsigned char lowByte  = (hex_to_int (in[2]) << 4) | hex_to_int (in[3]);
            return (highByte << 8) | lowByte;
        }

        template<class char_t>
        void decode_unicode_append (unsigned int ui, JSON_TSTRING (char_t) & out);

        template<>
        void
        decode_unicode_append<char> (unsigned int ui, JSON_TSTRING (char) & out)
        {
            const size_t len = out.length ();
            if (ui <= 0x0000007F) {
                // * U-00000000 - U-0000007F:  0xxxxxxx
                out.resize (len + 1);
                out[len] = (ui & 0x7F);
            }
            else if (ui >= 0x00000080 && ui <= 0x000007FF) {
                // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
                out.resize (len + 2);
                out[len + 1] = (ui & 0x3F) | 0x80;
                out[len]     = ((ui >> 6) & 0x1F) | 0xC0;
            }
            else if (ui >= 0x00000800 && ui <= 0x0000FFFF) {
                // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
                out.resize (len + 3);
                out[len + 2] = (ui & 0x3F) | 0x80;
                out[len + 1] = ((ui >> 6) & 0x3F) | 0x80;
                out[len]     = ((ui >> 12) & 0x0F) | 0xE0;
            }
            else if (ui >= 0x00010000 && ui <= 0x001FFFFF) {
                // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                out.resize (len + 4);
                out[len + 3] = (ui & 0x3F) | 0x80;
                out[len + 2] = ((ui >> 6) & 0x3F) | 0x80;
                out[len + 1] = ((ui >> 12) & 0x3F) | 0x80;
                out[len]     = ((ui >> 18) & 0x07) | 0xF0;
            }
            else if (ui >= 0x00200000 && ui <= 0x03FFFFFF) {
                // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                out.resize (len + 5);
                out[len + 4] = (ui & 0x3F) | 0x80;
                out[len + 3] = ((ui >> 6) & 0x3F) | 0x80;
                out[len + 2] = ((ui >> 12) & 0x3F) | 0x80;
                out[len + 1] = ((ui >> 18) & 0x3F) | 0x80;
                out[len]     = ((ui >> 24) & 0x03) | 0xF8;
            }
            else if (ui >= 0x04000000 && ui <= 0x7FFFFFFF) {
                // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                out.resize (len + 6);
                out[len + 5] = (ui & 0x3F) | 0x80;
                out[len + 4] = ((ui >> 6) & 0x3F) | 0x80;
                out[len + 3] = ((ui >> 12) & 0x3F) | 0x80;
                out[len + 2] = ((ui >> 18) & 0x3F) | 0x80;
                out[len + 1] = ((ui >> 24) & 0x3F) | 0x80;
                out[len]     = ((ui >> 30) & 0x01) | 0xFC;
            }
        }

        template<>
        void
        decode_unicode_append<wchar_t> (unsigned int ui, JSON_TSTRING (wchar_t) & out)
        {
            out += ui;
        }

        template<class char_t>
        size_t
        decode_unicode (const char_t* in, size_t len, JSON_TSTRING (char_t) & out)
        {
            size_t       ret = 4;
            unsigned int ui  = hex_to_ushort (in, len);
            if (ui >= 0xD800 && ui < 0xDC00)
            {
                JSON_DECODE_CHECK (len >= 6 && in[4] == '\\' && in[5] == 'u');
                ui = (ui & 0x3FF) << 10;
                ui += (hex_to_ushort (in + 6, len - 6) & 0x3FF) + 0x10000;
                ret += 6;
            }
            decode_unicode_append<char_t> (ui, out);
            return ret;
        }

        template<class char_t>
        void
        decode (const char_t* in, size_t len, JSON_TSTRING (char_t) & out)
        {
            for (size_t pos = 0; pos < len; ++pos)
            {
                switch (in[pos])
                {
                    case '\\':
                        JSON_PARSE_CHECK (pos + 1 < len);
                        ++pos;
                        switch (in[pos])
                        {
                            case '\"':
                            case '\\':
                            case '/':
                                out += in[pos];
                                break;
                            case 'b':
                                out += '\b';
                                break;
                            case 'f':
                                out += '\f';
                                break;
                            case 'n':
                                out += '\n';
                                break;
                            case 'r':
                                out += '\r';
                                break;
                            case 't':
                                out += '\t';
                                break;
                            case 'u':
                                pos += decode_unicode<char_t> (in + pos + 1, len - pos - 1, out);
                                break;
                            default:
                                JSON_PARSE_CHECK (false);
                        }
                        break;
                    default:
                        out += in[pos];
                        break;
                }
            }
        }

        template<class char_t>
        const char_t* boolean_true ();
        template<>
        inline const char*
        boolean_true<char> ()
        {
            return "true";
        }
        template<>
        inline const wchar_t*
        boolean_true<wchar_t> ()
        {
            return L"true";
        }

        template<class char_t>
        inline size_t
        boolean_true_length ()
        {
            return 4;
        }
        template<class char_t>
        inline size_t
        boolean_true_raw_length ()
        {
            return 4 * sizeof (char_t);
        }

        template<class char_t>
        const char_t* boolean_false ();
        template<>
        inline const char*
        boolean_false<char> ()
        {
            return "false";
        }
        template<>
        inline const wchar_t*
        boolean_false<wchar_t> ()
        {
            return L"false";
        }

        template<class char_t>
        inline size_t
        boolean_false_length ()
        {
            return 5;
        }
        template<class char_t>
        inline size_t
        boolean_false_raw_length ()
        {
            return 5 * sizeof (char_t);
        }

        template<class char_t>
        const char_t* nil_null ();
        template<>
        inline const char*
        nil_null<char> ()
        {
            return "null";
        }
        template<>
        inline const wchar_t*
        nil_null<wchar_t> ()
        {
            return L"null";
        }

        template<class char_t>
        inline size_t
        nil_null_length ()
        {
            return 4;
        }
        template<class char_t>
        inline size_t
        nil_null_raw_length ()
        {
            return 4 * sizeof (char_t);
        }

        template<class char_t>
        int64_t ttoi64 (const char_t* in, char_t** end);
        template<>
        int64_t
        ttoi64<char> (const char* in, char** end)
        {
            return
        #ifdef __WINDOWS__
                _strtoi64 (in, end, 10);
        #else
                strtoll (in, end, 10);
        #endif
        }
        template<>
        int64_t
        ttoi64<wchar_t> (const wchar_t* in, wchar_t** end)
        {
            return
        #ifdef __WINDOWS__
                _wcstoi64 (in, end, 10);
        #else
                wcstoll (in, end, 10);
        #endif
        }

        template<class char_t>
        double 
        ttod (const char_t* in, char_t** end);
        template<>
        double 
        ttod<char> (const char* in, char** end)
        {
            return strtod (in, end);
        }
        template<>
        double 
        ttod<wchar_t> (const wchar_t* in, wchar_t** end)
        {
            return wcstod (in, end);
        }

        template<class char_t>
        bool check_need_conv (char_t ch);
        template<>
        inline bool
        check_need_conv<char> (char ch)
        {
            return false;
        }
        template<>
        inline bool
        check_need_conv<wchar_t> (wchar_t ch)
        {
            return ch > 127;
        }

       /*
        * May suffer performance degradation, use `find` or `count + []` then call o() to get reference instead.
        */
        template<class char_t, class T>
        typename json_enable_if<json_is_same<ObjectT<char_t>, T>::value, T>::type
        internal_type_casting (const JSON::ValueT<char_t>& v, const T& value)
        {
            switch (v.type ())
            {
                case NIL:
                    break;
                case OBJECT:
                    return T (v.o ());
                default:
                    JSON_INTERNAL_ASSERT_CHECK_EX(false, "Type-casting error: from (%s) type to object.", get_type_name (v.type ()));
            }
            return T (value);
        }

       /*
        * May suffer performance degradation, use `find` or `count + []` then call a() to get reference instead.
        */
        template<class char_t, class T>
        typename json_enable_if<json_is_same<ArrayT<char_t>, T>::value, T>::type
        internal_type_casting (const JSON::ValueT<char_t>& v, const T& value)
        {
            switch (v.type ())
            {
                case NIL:
                    break;
                case ARRAY:
                    return T (v.a ());
                default:
                    JSON_INTERNAL_ASSERT_CHECK_EX(false, "Type-casting error: from (%s) type to array.", get_type_name (v.type ()));
            }
            return T (value);
        }

        template<class char_t, class T>
        typename json_enable_if<json_is_arithmetic<T>::value, T>::type
        internal_type_casting (const JSON::ValueT<char_t>& v, const T& value)
        {
            switch (v.type ())
            {
                case NIL:
                    break;
                case INTEGER:
                    return T (v.i ());
                case FLOAT:
                    return T (v.f ());
                case BOOLEAN:
                    return T (v.b ());
                case STRING:
                    if (v.s () == boolean_true<char_t> ())
                    {
                        return T (1);
                    }
                    else if (v.s () == boolean_false<char_t> ())
                    {
                        return T (0);
                    }
                    else
                    {
                        char_t* end = 0;
                        double  d   = ttod (v.s ().c_str (), &end);
                        JSON_INTERNAL_ASSERT_CHECK_EX(end == &v.s ()[0] + v.s ().length (), "Type-casting error: (%s) to arithmetic.", convert_json_string (v.s ()).c_str ());
                        return T (d);
                    }
                    JSON_INTERNAL_ASSERT_CHECK_EX(false, "Type-casting error: (%s) to arithmetic.", convert_json_string (v.s ()).c_str ());
                default:
                    JSON_INTERNAL_ASSERT_CHECK_EX(false, "Type-casting error: from (%s) type to arithmetic.", get_type_name (v.type ()));
            }
            return T (value);
        }

        template<class char_t, class T>
        typename json_enable_if<json_is_same<JSON_TSTRING (char_t), T>::value, T>::type
        internal_type_casting (const JSON::ValueT<char_t>& v, const T& value)
        {
            switch (v.type ())
            {
                case NIL:
                    break;
                case INTEGER:
                    return to_string<int64_t, char_t> (v.i ());
                case FLOAT:
                    return to_string<double, char_t> (v.f ());
                case BOOLEAN:
                    return T (v.b () ? detail::boolean_true<char_t> () : detail::boolean_false<char_t> ());
                case STRING:
                    return T (v.s ());
                default:
                    JSON_INTERNAL_ASSERT_CHECK_EX(false, "Type-casting error: from (%s) type to string.", get_type_name(v.type()));
            }
            return T (value);
        }
    } // namespace
} // namespace detail

template<class char_t>
template<class T>
T
JSON::ValueT<char_t>::get (const tstring& key, const T& value) const
{
    JSON_CHECK_TYPE (_type, OBJECT);
    typename ObjectT<char_t>::const_iterator it = _object.find (key);
    if (it != _object.end ())
    {
        return JSON_MOVE ((detail::internal_type_casting<char_t, T> (it->second, value)));
    }
    return T (value);
}

template<class char_t>
void
ValueT<char_t>::write (tstring& out) const
{
    switch (_type)
    {
        case INTEGER:
            detail::to_string (_integer, out);
            break;
        case FLOAT:
            detail::to_string (_float, out);
            break;
        case BOOLEAN:
            out += (_boolean ? detail::boolean_true<char_t> () : detail::boolean_false<char_t> ());
            break;
        case NIL:
            out += detail::nil_null<char_t> ();
            break;
        case STRING:
            out += '\"';
            if (_needConv)
            {
                detail::encode (_string.c_str (), _string.length (), out);
            }
            else
            {
                out += _string;
            }
            out += '\"';
            break;
        case ARRAY:
            WriterT<char_t>::write (_array, out);
            break;
        case OBJECT:
            WriterT<char_t>::write (_object, out);
            break;
    }
}

#define WHITE_SPACE_CASES \
    case ' ':             \
    case '\b':            \
    case '\f':            \
    case '\n':            \
    case '\r':            \
    case '\t':

#define SKIP_WHITE_SPACE_CASES \
    WHITE_SPACE_CASES          \
    break;

#define NUMBER_1_9_CASES \
    case '1':            \
    case '2':            \
    case '3':            \
    case '4':            \
    case '5':            \
    case '6':            \
    case '7':            \
    case '8':            \
    case '9':

#define NUMBER_0_9_CASES \
    case '0':            \
        NUMBER_1_9_CASES

#define NUMBER_ENDINGS \
    WHITE_SPACE_CASES  \
    case ',':          \
    case ']':          \
    case '}':

template<class char_t>
size_t
ValueT<char_t>::read_string (const char_t* in, size_t len)
{
    enum
    {
        NONE = 0,
        NORMAL,
        ESCAPE
    };
    unsigned char state    = NONE;
    bool          needConv = false;
    size_t        pos      = 0;
    size_t        start    = 0;
    while (pos < len)
    {
        switch (state)
        {
            case NONE:
                switch (in[pos])
                {
                    case '\"':
                        state = NORMAL;
                        start = pos + 1;
                        break;
                        SKIP_WHITE_SPACE_CASES
                    default:
                        JSON_PARSE_CHECK (false);
                }
                break;
            case NORMAL:
            case ESCAPE:
                if (state != ESCAPE)
                {
                    if (detail::check_need_conv<char_t> (in[pos]))
                    {
                        needConv = true;
                    }
                    else if (in[pos] == '\\')
                    {
                        state    = ESCAPE;
                        needConv = true;
                    }
                    else if (in[pos] == '\"')
                    {
                        clear ();
                        _type = STRING;
                        if (needConv)
                        {
                            detail::decode (in + start, pos - start, _string);
                        }
                        else
                        {
                            _string.assign (in + start, pos - start);
                        }
                        _needConv = needConv;
                        return pos + 1;
                    }
                }
                else
                {
                    if (in[pos] == 'u')
                    {
                        needConv = true;
                    }
                    state = NORMAL;
                }
                break;
        }
        ++pos;
    }
    JSON_PARSE_CHECK (false);
}

template<class char_t>
size_t
ValueT<char_t>::read_number (const char_t* in, size_t len)
{
    enum
    {
        NONE = 0,
        SIGN,
        ZERO,
        DIGIT,
        POINT,
        DIGIT_FRAC,
        EXP,
        EXP_SIGN,
        DIGIT_EXP
    };
    unsigned char state = NONE;
    size_t        pos   = 0;
    size_t        start = 0;
    while (pos < len)
    {
        switch (state)
        {
            case NONE:
                switch (in[pos])
                {
                    case '-':
                        state = SIGN;
                        start = pos;
                        break;
                    case '0':
                        state = ZERO;
                        start = pos;
                        break;
                        NUMBER_1_9_CASES
                        state = DIGIT;
                        start = pos;
                        break;
                        SKIP_WHITE_SPACE_CASES
                    default:
                        JSON_PARSE_CHECK (false);
                }
                break;

            case SIGN:
                switch (in[pos])
                {
                    case '0':
                        state = ZERO;
                        break;
                        NUMBER_1_9_CASES
                        state = DIGIT;
                        break;
                    default:
                        JSON_PARSE_CHECK (false);
                }
                break;

            case ZERO:
                JSON_PARSE_CHECK (in[pos] != '0');
                if (!isdigit (in[pos]) || state != DIGIT)
                {
                    switch (in[pos])
                    {
                        case '.':
                            state = POINT;
                            break;
                            NUMBER_ENDINGS
                            goto GOTO_END;
                        default:
                            JSON_PARSE_CHECK (false);
                    }
                }
                break;
            case DIGIT:
                if (!isdigit (in[pos]) || state != DIGIT)
                {
                    switch (in[pos])
                    {
                        case '.':
                            state = POINT;
                            break;
                            NUMBER_ENDINGS
                            goto GOTO_END;
                        default:
                            JSON_PARSE_CHECK (false);
                    }
                }
                break;

            case POINT:
                switch (in[pos])
                {
                    NUMBER_0_9_CASES
                    state = DIGIT_FRAC;
                    break;
                    default:
                        JSON_PARSE_CHECK (false);
                }
                break;

            case DIGIT_FRAC:
                switch (in[pos])
                {
                    NUMBER_0_9_CASES
                    state = DIGIT_FRAC;
                    break;
                    case 'e':
                    case 'E':
                        state = EXP;
                        break;
                        NUMBER_ENDINGS
                        goto GOTO_END;
                }
                break;

            case EXP:
                switch (in[pos])
                {
                    NUMBER_0_9_CASES
                    state = DIGIT_EXP;
                    break;
                    case '+':
                    case '-':
                        state = EXP_SIGN;
                        break;
                    default:
                        JSON_PARSE_CHECK (false);
                }
                break;

            case EXP_SIGN:
                switch (in[pos])
                {
                    NUMBER_0_9_CASES
                    state = DIGIT_EXP;
                    break;
                    default:
                        JSON_PARSE_CHECK (false);
                }
                break;

            case DIGIT_EXP:
                switch (in[pos])
                {
                    NUMBER_0_9_CASES
                    break;
                    NUMBER_ENDINGS
                    goto GOTO_END;
                }
                break;
        }
        ++pos;
    }
GOTO_END:
    char_t* end = 0;
    switch (state)
    {
        case ZERO:
        case DIGIT:
            clear ();
            _type    = INTEGER;
            _integer = detail::ttoi64<char_t> (in + start, &end);
            JSON_PARSE_CHECK (end == in + pos);
            return pos;
        case DIGIT_FRAC:
        case DIGIT_EXP:
            clear ();
            _type  = FLOAT;
            _float = detail::ttod<char_t> (in + start, &end);
            JSON_PARSE_CHECK (end == in + pos);
            return pos;
    }
    JSON_PARSE_CHECK (false);
}

template<class char_t>
size_t
ValueT<char_t>::read_nil (const char_t* in, size_t len)
{
    size_t pos = 0;
    while (pos < len)
    {
        switch (in[pos])
        {
            case 'n':
                JSON_PARSE_CHECK (len - pos >= detail::nil_null_length<char_t> ());
                if (memcmp (in + pos, detail::nil_null<char_t> (), detail::nil_null_raw_length<char_t> ()) == 0)
                {
                    clear ();
                    _type = NIL;
                    return pos + detail::nil_null_length<char_t> ();
                }
                break;
                SKIP_WHITE_SPACE_CASES
            default:
                JSON_PARSE_CHECK (false);
        }
        ++pos;
    }
    JSON_PARSE_CHECK (false);
}

template<class char_t>
size_t
ValueT<char_t>::read_boolean (const char_t* in, size_t len)
{
    size_t pos = 0;
    while (pos < len)
    {
        switch (in[pos])
        {
            case 't':
                JSON_PARSE_CHECK (len - pos >= detail::boolean_true_length<char_t> ());
                if (memcmp (in + pos, detail::boolean_true<char_t> (), detail::boolean_true_raw_length<char_t> ()) == 0)
                {
                    clear ();
                    _type    = BOOLEAN;
                    _boolean = true;
                    return pos + detail::boolean_true_length<char_t> ();
                }
                break;
            case 'f':
                JSON_PARSE_CHECK (len - pos >= detail::boolean_false_length<char_t> ());
                if (memcmp (in + pos, detail::boolean_false<char_t> (), detail::boolean_false_raw_length<char_t> ()) ==
                    0)
                {
                    clear ();
                    _type    = BOOLEAN;
                    _boolean = false;
                    return pos + detail::boolean_false_length<char_t> ();
                }
                break;
                SKIP_WHITE_SPACE_CASES
            default:
                JSON_PARSE_CHECK (false);
        }
        ++pos;
    }
    JSON_PARSE_CHECK (false);
}

#define OBJECT_ARRAY_PARSE_END(type)              \
    JSON_PARSE_CHECK (pv.back ()->_type == type); \
    pv.pop_back ();                               \
    if (pv.empty ())                              \
    {                                             \
        /* Object/Array parse finished. */        \
        return pos + 1;                           \
    }                                             \
    if (pv.back ()->_type == OBJECT)              \
    {                                             \
        state = OBJECT_PAIR_VALUE;                \
    }                                             \
    else if (pv.back ()->_type == ARRAY)          \
    {                                             \
        state = ARRAY_ELEM;                       \
    }

#define PUSH_VALUE_TO_STACK(type)                                     \
    if (pv.back ()->_type == NIL)                                     \
    {                                                                 \
        /* Object */                                                  \
        pv.back ()->_type = type;                                     \
    }                                                                 \
    else                                                              \
    {                                                                 \
        /* Array */                                                   \
        pv.back ()->_array.push_back (JSON_MOVE (ValueT<char_t> ())); \
        pv.back ()->_array.back ()._type = type;                      \
        pv.push_back (&pv.back ()->_array.back ());                   \
    }

template<class char_t>
size_t
ValueT<char_t>::read (const char_t* in, size_t len)
{
    // Indicate current parse state
    enum
    {
        NONE = 0,

        /* { */
        OBJECT_LBRACE,
        /* {" ," */
        OBJECT_PAIR_KEY_QUOTE,
        /* "..." */
        OBJECT_PAIR_KEY,
        /* "...": */
        OBJECT_PAIR_COLON,
        /* "...": "..." */
        OBJECT_PAIR_VALUE,
        /* {..., */
        OBJECT_COMMA,

        /* [ */
        ARRAY_LBRACKET,
        /* [...  [...,... */
        ARRAY_ELEM,
        /* [..., */
        ARRAY_COMMA
    };
    unsigned char state = NONE;
    size_t        pos   = 0;
    union
    {
        size_t start;
        size_t (ValueT<char_t>::*fp) (const char_t*, size_t);
    } u;
    memset (&u, 0, sizeof (u));
    vector<ValueT<char_t>*> pv (1, this);
    ValueT<char_t>          v;
    while (pos < len)
    {
        switch (state)
        {
            case NONE:
                // Topmost value parse.
                switch (in[pos])
                {
                    case '{':
                        state = OBJECT_LBRACE;
                        clear ();
                        _type = OBJECT;
                        break;
                    case '[':
                        state = ARRAY_LBRACKET;
                        clear ();
                        _type = ARRAY;
                        break;
                        SKIP_WHITE_SPACE_CASES
                    default:
                        JSON_PARSE_CHECK (false);
                }
                break;

            case OBJECT_LBRACE:
            case OBJECT_COMMA:
                switch (in[pos])
                {
                    case '\"':
                        state   = OBJECT_PAIR_KEY_QUOTE;
                        u.start = pos + 1;
                        break;
                    case '}':
                        if (state == OBJECT_LBRACE)
                        {
                            OBJECT_ARRAY_PARSE_END (OBJECT);
                        }
                        else
                        {
                            JSON_PARSE_CHECK (false);
                        }
                        break;
                        SKIP_WHITE_SPACE_CASES
                    default:
                        JSON_PARSE_CHECK (false);
                }
                break;

            case OBJECT_PAIR_KEY_QUOTE:
                switch (in[pos])
                {
                    case '\\':
                        if (in[pos + 1] != 'u')
                        {
                            ++pos;
                            while (pos < len)
                            {
                                if (in[pos + 1] == '\"' && in[pos] != '\\')
                                {
                                    break;
                                }
                                ++pos;
                            }
                            if (pos >= len)
                            {
                                JSON_PARSE_CHECK (false);
                            }
                        }
                        break;
                    case '\"':
                        state = OBJECT_PAIR_KEY;
                        // Insert a value
                        v.read_string (in + u.start - 1, pos - u.start + 1 + 1);
                        pv.push_back (&pv.back ()->_object[JSON_MOVE (v._string)]);
                        v.clear ();
                        u.start = 0;
                        break;
                    default:
                        break;
                }
                break;

            case OBJECT_PAIR_KEY:
                switch (in[pos])
                {
                    case ':':
                        state = OBJECT_PAIR_COLON;
                        break;
                        SKIP_WHITE_SPACE_CASES
                    default:
                        JSON_PARSE_CHECK (false);
                }
                break;

            case OBJECT_PAIR_COLON:
            case ARRAY_LBRACKET:
            case ARRAY_COMMA:
                switch (in[pos])
                {
                    case '\"':
                        u.fp = &ValueT::read_string;
                        break;
                    case '-':
                        NUMBER_0_9_CASES
                        u.fp = &ValueT::read_number;
                        break;
                    case 't':
                    case 'f':
                        u.fp = &ValueT::read_boolean;
                        break;
                    case 'n':
                        u.fp = &ValueT::read_nil;
                        break;
                    case '{':
                        state = OBJECT_LBRACE;
                        PUSH_VALUE_TO_STACK (OBJECT);
                        break;
                    case '[':
                        state = ARRAY_LBRACKET;
                        PUSH_VALUE_TO_STACK (ARRAY);
                        break;
                    case ']':
                        if (state == ARRAY_LBRACKET)
                        {
                            OBJECT_ARRAY_PARSE_END (ARRAY);
                        }
                        else
                        {
                            JSON_PARSE_CHECK (false);
                        }
                        break;
                        SKIP_WHITE_SPACE_CASES
                    default:
                        JSON_PARSE_CHECK (false);
                }
                if (u.fp)
                {
                    // If top elem is array, push a elem.
                    if (pv.back ()->_type == ARRAY)
                    {
                        pv.back ()->_array.push_back (JSON_MOVE (ValueT<char_t> ()));
                        pv.push_back (&pv.back ()->_array.back ());
                    }
                    // ++pos at last, so minus 1 here.
                    pos += (pv.back ()->*u.fp) (in + pos, len - pos) - 1;
                    u.fp = 0;
                    // pop nil/number/boolean/string Value
                    pv.pop_back ();
                    JSON_PARSE_CHECK (!pv.empty ());
                    switch (pv.back ()->_type)
                    {
                        case OBJECT:
                            state = OBJECT_PAIR_VALUE;
                            break;
                        case ARRAY:
                            state = ARRAY_ELEM;
                            break;
                        default:
                            JSON_PARSE_CHECK (false);
                    }
                }
                break;

            case OBJECT_PAIR_VALUE:
                switch (in[pos])
                {
                    case '}':
                        OBJECT_ARRAY_PARSE_END (OBJECT);
                        break;
                    case ',':
                        state = OBJECT_COMMA;
                        break;
                        SKIP_WHITE_SPACE_CASES
                    default:
                        JSON_PARSE_CHECK (false);
                }
                break;

            case ARRAY_ELEM:
                switch (in[pos])
                {
                    case ']':
                        OBJECT_ARRAY_PARSE_END (ARRAY);
                        break;
                    case ',':
                        state = ARRAY_COMMA;
                        break;
                        SKIP_WHITE_SPACE_CASES
                    default:
                        JSON_PARSE_CHECK (false);
                }
                break;
        }
        ++pos;
    }
    JSON_PARSE_CHECK (false);
}

template<class char_t>
void
WriterT<char_t>::write (const ObjectT<char_t>& o, JSON_TSTRING (char_t) & out)
{
    out += '{';
    bool first_elem = true;
    for (typename ObjectT<char_t>::const_iterator it = o.begin (); it != o.end (); ++it)
    {
        if (first_elem)
        {
            first_elem = false;
        }
        else
        {
            out += ',';
        }
        out += '\"';
        out += it->first;
        out += '\"';
        out += ':';
        it->second.write (out);
    }
    out += '}';
}

template<class char_t>
void
WriterT<char_t>::write (const ArrayT<char_t>& a, JSON_TSTRING (char_t) & out)
{
    out += '[';
    bool first_elem = true;
    for (typename ArrayT<char_t>::const_iterator it = a.begin (); it != a.end (); ++it)
    {
        if (first_elem)
        {
            first_elem = false;
        }
        else
        {
            out += ',';
        }
        it->write (out);
    }
    out += ']';
}

template<class char_t>
bool
operator== (const ObjectT<char_t>& lhs, const ObjectT<char_t>& rhs)
{
    if (lhs.size () != rhs.size ())
    {
        return false;
    }
    typename ObjectT<char_t>::const_iterator lit = lhs.begin ();
    typename ObjectT<char_t>::const_iterator rit = rhs.begin ();
    for (; lit != lhs.end (); ++lit, ++rit)
    {
        if (lit->first != rit->first)
        {
            return false;
        }
        if (lit->second != rit->second)
        {
            return false;
        }
    }
    return true;
}

template<class char_t>
bool
operator== (const ArrayT<char_t>& lhs, const ArrayT<char_t>& rhs)
{
    if (lhs.size () != rhs.size ())
    {
        return false;
    }
    for (size_t i = 0; i < lhs.size (); ++i)
    {
        if (lhs[i] != rhs[i])
        {
            return false;
        }
    }
    return true;
}

template<class char_t>
bool
operator== (const ValueT<char_t>& lhs, const ValueT<char_t>& rhs)
{
    if (lhs.type () != rhs.type ())
    {
        return false;
    }
    switch (lhs.type ())
    {
        case NIL:
            return true;
        case INTEGER:
            return lhs.i () == rhs.i ();
        case FLOAT:
            return fabs (lhs.f () - rhs.f ()) < JSON_EPSILON;
        case BOOLEAN:
            return lhs.b () == rhs.b ();
        case STRING:
            return lhs.s () == rhs.s ();
        case ARRAY:
            return lhs.a () == rhs.a ();
        case OBJECT:
            return lhs.o () == rhs.o ();
    }
    return true;
}

template<class char_t>
std::vector<string>
ValueT<char_t>::get_keys ()
{
    std::vector<string>                      key_list;
    typename ObjectT<char_t>::const_iterator lit = _object.begin ();
    for (; lit != _object.end (); ++lit)
    {
        key_list.push_back (lit->first);
    }

    return key_list;
}

template<class char_t>
std::vector<string>
ValueT<char_t>::get_keys_like (const string& key_like)
{
    std::vector<string>                      like_key_list;
    typename ObjectT<char_t>::const_iterator lit = _object.begin ();
    for (; lit != _object.end (); ++lit)
    {
        if (0 == strncmp (lit->first.c_str (), key_like.c_str (), strlen (key_like.c_str ())))
        {
            like_key_list.push_back (lit->first);
        }
    }

    return like_key_list;
}

} // namespace JSON

#endif // __NC_JSON_INL__
