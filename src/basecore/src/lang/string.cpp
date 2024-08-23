#include "lang/string.h"
#include "lang/Exception.h"
#include <stdio.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

namespace detail {

    inline void throw_xran (const cpp::lang::String &msg) 
    {
        // throw cpp::lang::Exception(0, ::toSTLString(msg));
    }

    template<class T>
    inline const T*
    find(const T *first, size_t count, const T &ch)
    {
        // look for ch in [first, first + count)
        for (; 0 < count; --count, ++first) {
            if (*first == ch) {
                return (first);
            }
        }
        return (0);
    }

    template<class T>
    inline bool
    match (const T *first, const T *ch, size_t count)
    {
        // look for ch in [first, first + count)
        for (; 0 < count; --count, ++first, ++ch) {
            if (*first != *ch) {
                return false;
            }
        }
        return true;
    }

    template<class T>
    inline int
    compare (const T *first1, const T *first2, size_t count)
    {
        // compare [first1, first1 + count) with [first2, ...)
        for (; 0 < count; --count, ++first1, ++first2) {
            if (*first1 != *first2) {
                return ((*first1 < *first2) ? -1 : 1);
            }
        }
        return (0);
    }

    #ifdef __WINDOWS__
    // Return the Unicode string of @ptr.
    // return the Unicode string buffer, the caller should release it.
    inline tchar_t*
    getUnicodeString (const char *ptr, size_t num, size_t *outLen)
    {
        UTF16LEConversion conv;

        size_t len = conv.getByteCount (ptr, num, ASCII_ENCODING);

        tchar_t* wBuf = new tchar_t[len / sizeof (tchar_t) + 1];

        try {
            len = conv.toBytes (ptr, num, ASCII_ENCODING, wBuf, len);
        }
        catch (...) {
            delete[] wBuf;
            throw;
        }

        wBuf[len / sizeof (tchar_t)] = 0; // append the terminate character.

        if (outLen != 0) {
            *outLen = len / sizeof (tchar_t);
        }
        return wBuf;
    }

    #endif

}; // namespace detail

/////////////////////////////////////////////////////////////////////////////////////////////////////
// String class

namespace cpp {
namespace lang {

// public static
const String String::EMPTY;

// public
String &String::assign(const tchar_t *ptr)
{
    return assign(ptr, ptr ? t_strlen (ptr) : 0);
}

// public
String &String::assign(const String &source)
{
    if (this != &source) {
        _str.assign (source._str);
    }
    return (*this);
}

// public
String &String::assign(const String &source, size_t offset, size_t count)
{
    if (source.getLength () < offset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR"));
    }
    return _assign(source, offset, count);
}

// public
String &String::_assign(const String &source, size_t offset, size_t count)
{
    size_t length = source.getLength();

    size_t num = length - offset;
    if (count < num) {
        num = count; // trim num to size
    }
    if (this == &source) {
        _remove ((size_t)(offset + num));
        _remove ((size_t)0, offset); // substring
    }
    else {
        _str.assign (source._str, offset, num);
    }

    return (*this);
}

// public
String &String::assign(const tchar_t *ptr, size_t num)
{
    if (ptr == 0) {
        // detail::throw_xran(String("Null Pointer"));
    }

    if (NO_POSITION <= num) {
        // detail::throw_xran(String("the number of string is too long.")); // result too long
    }

    return _assign (ptr, num);
}

// public
String &String::_assign(const tchar_t *ptr, size_t num)
{
    // assign [ptr, ptr + num)
    if (inside (ptr)) {
        return (_assign(*this, (ptr - getMyPtr ()), num)); // substring
    }
    _str.assign(ptr, num);

    return (*this);
}

// public
String &String::assign (tchar_t ch, size_t count)
{
    if (count == NO_POSITION) {
        // detail::throw_xran(String("count is too long.")); // result too long
    }
    _str.assign (count, ch);

    return (*this);
}

#ifdef __ENABLE_CXX11__
String &String::assign(String &&source)
{
    if (this != &source) {
        auto str = _str;
        _str     = source._str;

        source._str = str;
        source.clear ();
    }
    return *this;
}
#endif

// public
String &String::append(const String &source, size_t offset, size_t count)
{
    // append source [offset, offset + count)
    if (source.getLength () < offset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset off end
    }
    size_t num = source.getLength () - offset;
    if (num < count) {
        count = num; // trim count to size
    }

    if (count > 0) {
        _str.append (source._str, offset, count);
    }

    return (*this);
}

// public
String &String::append(const tchar_t *ptr, size_t count)
{
    // append [ptr, ptr + count)
    if (inside (ptr)) {
        size_t offset = (ptr - getMyPtr ());

        return append (*this, offset, count); // substring
    }

    size_t size = getLength ();
    if (NO_POSITION - size <= count) {
        // detail::throw_xran(String("count is too long.")); // result too long
    }

    if (count > 0) {
        _str.append (ptr, count);
    }

    return (*this);
}

String &String::append(tchar_t ch, size_t count)
{
    // append count * ch
    size_t size = getLength ();

    if (NO_POSITION - size <= count) {
        // detail::throw_xran(String("count is too long")); // result too long?
    }
    if (count > 0) {
        _str.append (count, ch);
    }

    return (*this);
}

// public
tchar_t String::at (size_t n) const
{
    // subscript mutable sequence with checking
    if (getLength () <= n) {
        // detail::throw_xran(String("IDS_INDEX_OFR")); // offset off end
    }

    return (getMyPtr()[n]);
}

// public
tchar_t &String::at(size_t n) // read/write access
{
    // subscript mutable sequence with checking
    if (getLength () <= n) {
        // detail::throw_xran(String("IDS_INDEX_OFR")); // offset off end
    }

    return (getMyPtr()[n]);
}

// public
String &String::remove(size_t offset /* = 0 */, size_t count /* = NO_POSITION */)
{
    if (getLength () < offset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset off end
    }

    return _remove (offset, count);
}

// public
String &String::_remove (size_t offset /* = 0 */, size_t count /* = NO_POSITION */)
{
    // remove characters [offset, offset + count)
    size_t size = getLength ();

    if ((size - offset) < count) {
        count = size - offset; // trim count
    }

    if (count > 0) {
        _str.erase (offset, count);
    }

    return (*this);
}

// public
void String::swap(String &source)
{
    //
    //swap control information
    //
    _str.swap (source._str);
}

// public
void String::reserve (size_t newSiz /* = 0*/)
{
    _str.reserve (newSiz);
}

// public
size_t String::copy(tchar_t* ptr, size_t count, size_t offset /* = 0 */) const
{
    // copy [offset, offset + count) to [ptr, ptr + count)
    size_t size = getLength ();

    if (size <= offset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset off end
    }

    if (size - offset < count) {
        count = size - offset;
    }

    ::memcpy (ptr, getMyPtr() + offset, count * sizeof(tchar_t));

    return (count);
}

// public
size_t String::find(const tchar_t *ptr, size_t offset, size_t count) const
{
    // look for [ptr, ptr + count) beginnng at or after offset
    size_t size = getLength ();
    if (count == 0 && offset <= size) {
        return (offset); // null string always matches (if inside string)
    }

    size_t num;
    if (offset < size && count <= (num = size - offset)) {
        // room for match, look for it
        const tchar_t *uptr, *vptr;

        for (num -= count - 1, vptr = getMyPtr () + offset; (uptr = detail::find (vptr, num, *ptr)) != 0;
             num -= uptr - vptr + 1, vptr = uptr + 1)
        {
            if (detail::compare (uptr, ptr, count) == 0) {
                return (uptr - getMyPtr ()); // found a match
            }
        }
    }

    return (NO_POSITION); // no match
}

// public
size_t String::rfind(const tchar_t *ptr, size_t offset, size_t count) const
{
    size_t size = getLength ();
    // look for [ptr, ptr + count) beginning before offset
    if (count == 0) {
        return (offset < size ? offset : size); // null always matches
    }

    if (count <= size) {
        // room for match, look for it
        const tchar_t *uptr = getMyPtr () + (offset < size - count ? offset : size - count);

        for (;; --uptr) {
            if ((*uptr == *ptr) && detail::compare (uptr, ptr, count) == 0) {
                return (uptr - getMyPtr ()); // found a match
            }
            else if (uptr == getMyPtr ()) {
                break; // at beginning, no more chance for match
            }
        }
    }

    return (NO_POSITION); // no match
}

// public
size_t String::findFirstOf(const tchar_t *ptr, size_t offset, size_t count) const
{
    // look for one of [ptr, ptr + count) at or after offset
    size_t size = getLength ();

    if (count > 0 && offset < size) {
        // room for match, look for it
        const tchar_t *const vptr = getMyPtr () + size;

        for (const tchar_t* uptr = getMyPtr () + offset; uptr + count <= vptr; ++uptr) {
            if (detail::match (uptr, ptr, count)) {
                return (uptr - getMyPtr ()); // found a match
            }
        }
    }

    return (NO_POSITION); // no match
}

// public
size_t String::findLastOf(const tchar_t *ptr, size_t offset, size_t count) const
{
    // look for one of [ptr, ptr + count) before offset
    size_t size = getLength ();

    if (count > 0 && size > 0) {
        for (const tchar_t* uptr = getMyPtr () + (offset < size ? offset : size - 1) - count + 1;; --uptr) {
            bool first = detail::match (uptr, ptr, count);
            if (first) {
                return (uptr - getMyPtr ()); // found a match
            }
            else if (uptr == getMyPtr ()) {
                break; // at beginning, no more chance for match
            }
        }
    }

    return (NO_POSITION); // no match
}

// public
size_t String::findFirstNotOf(const tchar_t *ptr, size_t offset, size_t count) const
{
    // look for none of [ptr, ptr + count) at or after offset
    size_t size = getLength ();

    if (offset < size) {
        // room for match, look for it
        const tchar_t* const vptr = getMyPtr () + size;

        for (const tchar_t* uptr = getMyPtr () + offset; uptr + count <= vptr; ++uptr) {
            bool first = detail::match (uptr, ptr, count);
            if (first == false) {
                return (uptr - getMyPtr ());
            }
        }
    }

    return (NO_POSITION);
}

// public
size_t String::findLastNotOf (const tchar_t* ptr, size_t offset, size_t count) const
{
    // look for none of [ptr, ptr + count) before offset
    size_t size = getLength ();

    if (size > 0) {
        for (const tchar_t* uptr = getMyPtr () + (offset < size ? offset : size - 1) - count + 1;; --uptr) {
            bool first = detail::match (uptr, ptr, count);
            if (first == false) {
                return (uptr - getMyPtr () + count - 1);
            }
            else if (uptr == getMyPtr ()) {
                break;
            }
        }
    }

    return (NO_POSITION);
}

// public
size_t String::findAny(size_t offset, const String &chars) const
{
    size_t size = getLength ();
    if (offset > size) {
        // detail::throw_xran(String("IDS_OFFSET_OFR"));
    }

    //size -= offset;
    size_t charsSize = chars.getLength ();
    const tchar_t *ptr = getMyPtr ();

    for (size_t pos = offset; pos < size; pos++) {
        for (size_t loop = 0; loop != charsSize; loop++) {
            if (ptr[pos] == chars[loop]) {
                return pos; // return the position
            }
        }
    }

    return NO_POSITION;
}

// public
size_t String::findNotOfAny(size_t offset, const String &chars) const
{
    size_t size = getLength ();
    if (offset > size) {
        // detail::throw_xran(String("IDS_OFFSET_OFR"));
    }

    //size -= offset;
    size_t charsSize = chars.getLength ();
    const tchar_t *ptr = getMyPtr ();

    for (size_t pos = offset; pos < size; pos++) {
        for (size_t loop = 0; loop != charsSize; loop++) {
            if (ptr[pos] != chars[loop]) {
                return pos; // return the position
            }
        }
    }

    return NO_POSITION;
}

// public
size_t String::findLastOfAny(size_t offset, const String &chars) const
{
    size_t size = getLength ();
    if (offset >= size) {
        // detail::throw_xran(String("IDS_OFFSET_OFR"));
    }

    size_t charsSize = chars.getLength ();
    const tchar_t *ptr = getMyPtr ();

    for (size_t pos = offset; pos != NO_POSITION; --pos) {
        for (size_t loop = 0; loop != charsSize; loop++) {
            if (ptr[pos] == chars[loop]) {
                return pos; // return the position
            }
        }
    }

    return NO_POSITION;
}

// public
size_t String::findLastNotOfAny(size_t offset, const String &chars) const
{
    size_t size = getLength ();
    if (offset >= size) {
        // detail::throw_xran(String("IDS_OFFSET_OFR"));
    }

    size_t charsSize = chars.getLength ();
    const tchar_t *ptr = getMyPtr ();

    for (size_t pos = offset; pos != NO_POSITION; --pos) {
        for (size_t loop = 0; loop != charsSize; loop++) {
            if (ptr[pos] != chars[loop]) {
                return pos; // return the position
            }
        }
    }

    return NO_POSITION;
}

// public
int String::compare(const String &source) const
{
    // compare [offset, offset + number) with [ptr, ptr + count)
    size_t number = getLength ();
    size_t count  = source.getLength ();

    size_t answer = 0;
    if (number != 0) {
        size_t tmp = number < count ? number : count;
        const tchar_t *first1 = getMyPtr ();
        const tchar_t *first2 = source.getMyPtr ();

        // compare [first1, first1 + tmp) with [first2, ...)
        for (; 0 < tmp; --tmp, ++first1, ++first2) {
            if (*first1 != *first2) {
                answer = (*first1 < *first2) ? -1 : 1;
                break;
            }
        }
    }

    return (answer != 0 ? (int)answer : number < count ? -1 : number == count ? 0 : +1);
}

// public
int String::compare(size_t offset, size_t number, const String &source, size_t roffset, size_t count) const
{
    // compare [offset, offset + number) with source [roffset, roffset + count)

    if (source.getLength() < roffset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset off end
    }

    if (source.getLength() - roffset < count) {
        count = source.getLength() - roffset; // trim count to size
    }

    return compare(offset, number, source.getMyPtr() + roffset, count);
}

// public
int String::compare(size_t offset, size_t number, const tchar_t *ptr, size_t count) const
{
    // compare [offset, offset + number) with [ptr, ptr + count)
    size_t size = getLength ();

    if (size < offset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset off end
    }

    if (size - offset < number) {
        number = size - offset; // trim number to size
    }

    size_t answer = 0;
    if (number != 0) {
        size_t tmp = number < count ? number : count;
        const tchar_t* first1 = getMyPtr() + offset;
        const tchar_t* first2 = ptr;

        // compare [first1, first1 + tmp) with [first2, ...)
        for (; 0 < tmp; --tmp, ++first1, ++first2) {
            if (*first1 != *first2) {
                answer = (*first1 < *first2) ? -1 : 1;
                break;
            }
        }
    }

    return (answer != 0 ? (int)answer : number < count ? -1 : number == count ? 0 : +1);
}

// public
int String::compareIgnoreCase(size_t offset, size_t number, const String &source, size_t roffset, size_t count) const
{
    // compare [offset, offset + number) with source [roffset, roffset + count)

    if (source.getLength() < roffset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset off end
    }

    if (source.getLength() - roffset < count) {
        count = source.getLength() - roffset; // trim count to size
    }

    return compareIgnoreCase (offset, number, source.getMyPtr() + roffset, count);
}

// public
int String::compareIgnoreCase(size_t offset, size_t number, const tchar_t* ptr, size_t count) const
{
    // compare [offset, offset + number) with [ptr, ptr + count)
    size_t size = getLength ();

    if (size < offset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset off end
    }

    if (size - offset < number) {
        number = size - offset; // trim number to size
    }

    size_t answer = 0;
    if (number != 0) {
        size_t         tmp    = number < count ? number : count;
        const tchar_t* first1 = getMyPtr () + offset;
        const tchar_t* first2 = ptr;

        // compare [first1, first1 + tmp) with [first2, ...)
        for (; 0 < tmp; --tmp, ++first1, ++first2) {
            if (*first1 != *first2) {
                // If characters don't match but case may be ignored,
                // try converting both characters to uppercase.
                // If the results match, then the comparison scan should
                // continue.
                tchar_t u1 = t_toupper (*first1);
                tchar_t u2 = t_toupper (*first2);

                if (u1 != u2) {
                    // Unfortunately, conversion to uppercase does not work properly
                    // for the Georgian alphabet, which has strange rules about case
                    // conversion.  So we need to make one last check before
                    // exiting.
                    u1 = t_tolower (u1);
                    u2 = t_tolower (u2);

                    if (u1 != u2) {
                        answer = (u1 < u2) ? -1 : 1;
                        break;
                    }
                }
            }
        }
    }

    return (answer != 0 ? (int)answer : number < count ? -1 : number == count ? 0 : +1);
}

// public
String &String::replace(tchar_t oldValue, tchar_t newValue, bool replaceAll /* = false */)
{
    if (oldValue != newValue) {
        size_t   size = getLength ();
        tchar_t* ptr  = getMyPtr ();

        for (size_t i = 0; i < size; ++i, ++ptr) {
            if (*ptr == oldValue) {
                *ptr = newValue;

                if (!replaceAll) {
                    break; //don't replace all char, break the loop.
                }
            }
        }
    }

    return (*this);
}

// public
String &String::replace (size_t offset, size_t number, const String& source, size_t roffset, size_t count)
{
    // replace [offset, offset + number) with source [roffset, roffset + count)
    size_t size = getLength();

    if (size < offset || source.getLength() < roffset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset or roffset off end
    }

    if (size - offset < number) {
        number = size - offset; // trim number to size
    }

    size_t num = source.getLength() - roffset;
    if (num < count) {
        count = num; // trim count to size
    }

    if (NO_POSITION - count <= size - number) {
        // detail::throw_xran(String("the number of string is too long.")); // result too long
    }

    _str.replace (offset, number, source._str, roffset, count);

    return *this;
}

// public
String &String::replace (size_t offset, size_t number, const tchar_t *ptr, size_t count)
{
    // replace [offset, offset + number) with [ptr, ptr + count)
    if (inside (ptr)) {
        return replace(offset, number, *this, ptr - getMyPtr(), count); // substring, replace carefully
    }

    size_t size = getLength ();
    if (size < offset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset off end
    }

    if (size - offset < number) {
        number = size - offset; // trim number to size
    }

    if (NO_POSITION - count <= size - number) {
        // detail::throw_xran(String("the number of string is too long.")); // result too long
    }

    _str.replace (offset, number, ptr, count);

    return *this;
}

String &String::replace(size_t offset, size_t number, size_t count, tchar_t ch)
{
    // replace [offset, offset + number) with count * ch
    size_t size = getLength ();

    if (size < offset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset off end
    }

    if (size - offset < number) {
        number = size - offset; // trim number to size
    }

    if (NO_POSITION - count <= size - number) {
        // detail::throw_xran(String("the number of string is too long.")); // result too long
    }

    _str.replace (offset, number, count, ch);

    return *this;
}

// public
String &String::insert(size_t offset, const String &source, size_t roffset, size_t count)
{
    // insert source [roffset, roffset + count) at offset
    size_t size = getLength ();

    if (size < offset || source.getLength () < roffset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset or roffset off end
    }

    size_t num = source.getLength () - roffset;
    if (num < count) {
        count = num; // trim count to size
    }

    if (NO_POSITION - size <= count) {
        // detail::throw_xran(String("the count is too long")); // result too long
    }

    _str.insert (offset, source._str, roffset, count);

    return *this;
}

// public
String &String::insert(size_t offset, const tchar_t *ptr, size_t count)
{
    // insert [ptr, ptr + count) at offset
    if (inside (ptr)) {
        return insert (offset, *this, ptr - getMyPtr(), count); // substring
    }

    size_t size = getLength ();

    if (size < offset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset off end
    }

    if (NO_POSITION - size <= count) {
        // detail::throw_xran(String("the count is too long")); // result too long
    }

    _str.insert (offset, ptr, count);

    return *this;
}

// public
String &String::insert(size_t offset, tchar_t ch, size_t count)
{
    // insert count * ch at offset
    size_t size = getLength();

    if (size < offset) {
        // detail::throw_xran(String("IDS_OFFSET_OFR")); // offset or roffset off end
    }

    if (NO_POSITION - size <= count) {
        // detail::throw_xran(String("the count is too long")); // result too long
    }

    _str.insert (offset, count, ch);

    return *this;
}

// public
void String::toUpper(void)
{
    tchar_t* ptr  = getMyPtr ();
    size_t   size = getLength ();

    for (; size > 0; --size, ++ptr) {
        *ptr = t_toupper (*ptr);
    }
}

// public
void String::toLower(void)
{
    tchar_t* ptr  = getMyPtr ();
    size_t   size = getLength ();

    for (; size > 0; --size, ++ptr) {
        *ptr = t_tolower (*ptr);
    }
}

// public
String String::trim(tchar_t ch) const
{
    if (this->isEmpty ()) {
        return (*this);
    }

    const tchar_t* ptr    = getMyPtr();
    const size_t   size   = getLength();
    size_t         preLen = 0;

    // traverse from the begining
    while (*ptr == ch && preLen < size) {
        ++preLen, ++ptr;
    }

    // traverse from the end
    size_t postLen = size;
    ptr            = getMyPtr () + size - 1;
    while (*ptr == ch && (postLen > preLen)) {
        --postLen, --ptr;
    }

    return String (getMyPtr () + preLen, postLen - preLen);
}

// public
String String::leftString(size_t count) const
{
    if (count > getLength ()) {
        // detail::throw_xran(String("the count is too long.")); // result too long
    }

    return String (*this, 0, count);
}

// public
String String::rightString (size_t count) const
{
    if (count > getLength ()) {
        // detail::throw_xran(String("the count is too long.")); // result too long
    }

    return String(*this, getLength() - count, getLength());
}

// public
bool String::startsWith(const tchar_t *prefix) const
{
    // first check if the beginning of the string matches the prefix: note
    // that we don't have to check that we don't run out of this string as
    // when we reach the terminating NUL, either prefix string ends too (and
    // then it's ok) or we break out of the loop because there is no match
    const tchar_t* ptr = getMyPtr();

    while (*prefix) {
        if (*prefix++ != *ptr++) {
            // no match
            return false;
        }
    }

    return true;
}

// public
int String::freq(tchar_t ch) const
{
    int            count  = 0;
    const tchar_t* ptr    = getMyPtr ();
    const tchar_t* ptrEnd = ptr + getLength ();

    for (; ptr < ptrEnd; ++ptr) {
        if (*ptr == ch) {
            ++count;
        }
    }

    return count;
}

// public
void String::format(const tchar_t* fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    formatv (fmt, ap);
    va_end (ap);
}

// public
void String::formatv(const tchar_t *fmt, va_list ap)
{
#ifdef __WINDOWS__
    va_list vaArgsCopy;
    va_copy(vaArgsCopy, ap);
    const int iLen = ::_vscwprintf(fmt, vaArgsCopy);
    va_end(vaArgsCopy);

    std::wstring tmp(iLen + 1, '\0');
    ::_vsnwprintf(&tmp[0], tmp.size(), fmt, ap);
    tmp.resize (iLen);
    _str.swap (tmp);
#else

    va_list vaArgsCopy;
    va_copy(vaArgsCopy, ap);
    const int iLen = ::vsnprintf(NULL, 0, fmt, vaArgsCopy);
    va_end(vaArgsCopy);

    std::string tmp(iLen + 1, '\0');
    ::vsnprintf(&tmp[0], tmp.size (), fmt, ap);
    tmp.resize(iLen);
    _str.swap(tmp);
#endif
}

// public
void String::split(const String &separators, std::vector<String> &substrs, bool oneInSeparators /*= true*/) const
{
    if (this->isEmpty ()) {
        return;
    }

    if (separators.isEmpty ()) {
        substrs.push_back (*this);
        return;
    }

    size_t off = 0;
    size_t index;
    if (oneInSeparators == true) {
        index = findAny(off, separators);
    }
    else {
        index = find(separators, off);
    }
    size_t sepslength = separators.getLength();
    while (true) {
        if (index == String::NO_POSITION) {
            substrs.push_back (this->subString (off));
            break;
        }
        else {
            substrs.push_back (this->subString (off, index - off));
        }

        if (oneInSeparators == true) {
            off = index + 1;
        }
        else {
            off = index + sepslength;
        }

        if (off >= getLength()) {
            // last is a seperator
            substrs.push_back("");
            break;
        }

        if (oneInSeparators == true) {
            index = findAny (off, separators);
        }
        else {
            index = find (separators, off);
        }
    }
}

// private
void String::init(size_t initsize /* = INIT_SIZE*/)
{
    _str.reserve(initsize);
}

String::~String(void)
{
}

#ifdef __WINDOWS__
// public
String &String::assign(const char *ptr)
{
    return assign (ptr, ptr ? strlen(ptr) : 0);
}

// public
String &String::assign(const char *ptr, size_t num)
{
    if (ptr == 0) {
        // detail::throw_xran(String("Null Pointer"));
    }

    if (NO_POSITION <= num) {
        // detail::throw_xran(String("the number of string is too long.")); // result too long
    }

    if (num > 0) {
        size_t   len;
        tchar_t *wBuf = detail::getUnicodeString(ptr, num, &len);

        assign(wBuf, len);
        delete[] wBuf;
    }

    return (*this);
}

// public
String &String::append(const char *ptr, size_t count)
{
    if (count > 0) {
        size_t   len;
        tchar_t *wBuf = detail::getUnicodeString(ptr, count, &len);

        append(wBuf, len);
        delete[] wBuf;
    }

    return *this;
}

// public
String &String::insert(size_t offset, const char *ptr, size_t count)
{
    if (count > 0) {
        size_t   len;
        tchar_t *wBuf = detail::getUnicodeString(ptr, count, &len);

        insert(offset, wBuf, len);
        delete[] wBuf;
    }

    return (*this);
}

// public
size_t String::find(const char *ptr, size_t offset, size_t count) const
{
    if (count > 0) {
        size_t   len;
        tchar_t *wBuf = detail::getUnicodeString(ptr, count, &len);

        size_t found = find(wBuf, offset, len);
        delete[] wBuf;

        return found;
    }

    return String::NO_POSITION;
}

// public
size_t String::rfind(const char *ptr, size_t offset, size_t count) const
{
    if (count > 0) {
        size_t   len;
        tchar_t *wBuf = detail::getUnicodeString(ptr, count, &len);

        size_t found = rfind(wBuf, offset, len);
        delete[] wBuf;

        return found;
    }

    return String::NO_POSITION;
}

// public
size_t String::findFirstOf(const char *ptr, size_t offset, size_t count) const
{
    if (count > 0) {
        size_t   len;
        tchar_t *wBuf = detail::getUnicodeString(ptr, count, &len);

        size_t found = findFirstOf(wBuf, offset, len);
        delete[] wBuf;

        return found;
    }

    return String::NO_POSITION;
}
// public
size_t String::findLastOf(const char *ptr, size_t offset, size_t count) const
{
    if (count > 0) {
        size_t   len;
        tchar_t *wBuf = detail::getUnicodeString(ptr, count, &len);

        size_t found = findLastOf(wBuf, offset, len);
        delete[] wBuf;

        return found;
    }

    return String::NO_POSITION;
}

// public
size_t String::findFirstNotOf(const char *ptr, size_t offset, size_t count) const
{
    if (count > 0) {
        size_t   len;
        tchar_t *wBuf = detail::getUnicodeString(ptr, count, &len);

        size_t found = findFirstNotOf(wBuf, offset, len);
        delete[] wBuf;

        return found;
    }

    return String::NO_POSITION;
}

// public
size_t String::findLastNotOf(const char *ptr, size_t offset, size_t count) const
{
    if (count > 0) {
        size_t   len;
        tchar_t *wBuf = detail::getUnicodeString(ptr, count, &len);

        size_t found = findLastNotOf(wBuf, offset, len);
        delete[] wBuf;

        return found;
    }

    return String::NO_POSITION;
}

// public
bool String::startsWith(const char *prefix) const
{
    size_t count = ::strlen(prefix);

    if (count > 0) {
        size_t   len;
        tchar_t *wBuf = detail::getUnicodeString(prefix, count, &len);

        bool ret = startsWith(wBuf);
        delete[] wBuf;

        return ret;
    }
    else {
        return false;
    }
}

void String::format(const char *fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    formatv (fmt, ap);
    va_end (ap);
}

void String::formatv(const char *fmt, va_list arg_list)
{
    char *fmt_msg = 0;

    // SUSv2 version doesn't work for buf NULL/size 0, so try printing
    // into a small buffer that avoids the double-rendering and alloca path too...
    char         short_buf[256];
    const size_t needed = vsnprintf(short_buf, sizeof short_buf, fmt, arg_list) + 1;
    if (needed <= sizeof short_buf) {
        fmt_msg = short_buf;
    }
    else {
#ifdef __GNUC__
#define alloca(size) __builtin_alloca (size)
        // need more space...
        char* p = static_cast<char*> (alloca (needed));
        vsnprintf(p, needed, fmt, arg_list);
        fmt_msg = p;
#endif
    }

    assign (fmt_msg);
}

std::wstring String::to_wstring(const std::string &str, UINT codepage)
{
    std::wstring ws;
    int          count = ::MultiByteToWideChar(codepage, 0, str.c_str(), -1, nullptr, 0);
    if (count == 0) {
        return ws;
    }
    ws.resize ((size_t)count);
    count = ::MultiByteToWideChar(codepage, 0, str.c_str (), -1, &ws[0], count);
    if (count == 0) {
        return ws;
    }
    ws.resize ((size_t)count - 1);
    return ws;
}

std::string String::to_string(const std::wstring &wstr, UINT codepage)
{
    std::string utf8;
    int         count = ::WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, nullptr, 0, 0, 0);
    if (count == 0) {
        return utf8;
    }
    utf8.resize ((size_t)count);
    count = ::WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, &utf8[0], count, 0, 0);
    if (count == 0) {
        return utf8;
    }
    utf8.resize((size_t)count - 1);
    return utf8;
}

std::wstring String::utf8_to_utf16(const std::string &utf8)
{
    return to_wstring(utf8, CP_UTF8);
}

std::string String::utf16_to_utf8(const std::wstring &utf16)
{
    return to_string(utf16, CP_UTF8);
}

std::wstring String::ascii_to_utf16(const std::string &ascii)
{
    return to_wstring(ascii, CP_ACP);
}

std::string String::utf16_to_ascii(const std::wstring &utf16)
{
    return to_string (utf16, CP_ACP);
}

std::string String::ascii_to_utf8(const std::string &ascii)
{
    auto ws = ascii_to_utf16(ascii);
    return utf16_to_utf8(ws);
}

std::string String::utf8_to_ascii(const std::string &utf8)
{
    auto ws = utf8_to_utf16(utf8);
    return utf16_to_ascii(ws);
}

#endif

} // namespace lang
} // namespace cpp
