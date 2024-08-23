/*
 * file name: base64.cpp
 */ 
#include <text/base64.h>

namespace cpp {
    namespace text {
        //static const char* base64EncTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        static const char base64EncTable[] = {
            'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
            'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
            '0','1','2','3','4','5','6','7','8','9',
            '+', '/'};

        static const unsigned char base64DecTable[] = {
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62,
            255, 255, 255, 63,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  255, 255, 255, 254, 255, 255, 255, 0,
            1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,
            23,  24,  25,  255, 255, 255, 255, 255, 255, 26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
            39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  255, 255, 255, 255, 255};

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // class #Base64
        static inline bool
        isBase64Char (char c)
        {
            return (isalnum ((unsigned char)c) || (c == '+') || (c == '/') || (c == '='));
        }

        static inline bool
        isBase64Str (const char* bytes, size_t len)
        {
            for (size_t i = 0; i < len; i++)
            {
                if (i < len - 2 && bytes[i] == '=')
                {
                    return false;
                }
                if (false == isBase64Char (bytes[i]))
                {
                    return false;
                }
            }
            return true;
        }

        std::string
        Base64::encode(const void *bytesToEncode, size_t len)
        {
            if (bytesToEncode == 0 || len <= 0) {
                return "";
            }

            std::string strEncode;
            const char* strSource = (const char*)bytesToEncode;
            size_t      srcLen    = len;
            size_t      leftlen   = srcLen;
            size_t      i         = 0;

            while (leftlen >= 3)
            {
                /*
                **
                ** (11111111) (11111111) (11111111) -> (00111111) (0011 1111) (001111 11) (00111111)
                **      1          2          3              1         2           3           4
                */
                strEncode += base64EncTable[((int)strSource[i] & 0xFC) >> 2];
                strEncode += base64EncTable[((int)strSource[i] & 0x03) << 4 | ((int)strSource[i + 1] & 0xF0) >> 4];
                strEncode += base64EncTable[((int)strSource[i + 1] & 0x0F) << 2 | ((int)strSource[i + 2] & 0xC0) >> 6];
                strEncode += base64EncTable[((int)strSource[i + 2] & 0x3F)];
                leftlen -= 3;
                i += 3;
            }
            if (leftlen == 2)
            {
                /*
                **
                ** (11111111) (11111111) (00000000) -> (00111111) (0011 1111) (001111 00) (00000000)
                **      1          2          3              1         2           3           4
                */
                strEncode += base64EncTable[((int)strSource[i] & 0xFC) >> 2];
                strEncode += base64EncTable[((int)strSource[i] & 0x03) << 4 | ((int)strSource[i + 1] & 0xF0) >> 4];
                strEncode += base64EncTable[((int)strSource[i + 1] & 0x0F) << 2];
                strEncode += "=";
            }
            else if (leftlen == 1)
            {
                /*
                **
                ** (11111111) (00000000) (00000000) -> (00111111) (0011 0000) (000000 00) (00000000)
                **      1          2          3              1         2           3           4
                */
                strEncode += base64EncTable[((int)strSource[i] & 0xFC) >> 2];
                strEncode += base64EncTable[((int)strSource[i] & 0x3) << 4];
                strEncode += "==";
            }

            return strEncode;
        }

        // public static
        std::string
        Base64::encode (const std::string &strToEncode)
        {
            if (strToEncode.empty ()) {
                return strToEncode;
            }

            return std::string(encode((const void*)strToEncode.c_str(), strToEncode.length()).c_str ());
        }

        std::string
        Base64::decode (const char* str, int len /*= 0*/)
        {
            std::string      strDecode;
            const char* strSource = str;
            size_t      srcLen    = len;
            if (len <= 0) {
                srcLen = strlen(strSource);
            }

            if (srcLen == 0) {
                return "";
            }

            //invalid size
            if (srcLen % 4 != 0) {
                return str;
            }

            if (false == isBase64Str (strSource, srcLen)) {
                return str;
            }
            size_t i;
            for (i = 0; i < srcLen - 4; i += 4)
            {
                unsigned char asc[4];
                asc[0] = base64DecTable[(int)strSource[i]];
                asc[1] = base64DecTable[(int)strSource[i + 1]];
                asc[2] = base64DecTable[(int)strSource[i + 2]];
                asc[3] = base64DecTable[(int)strSource[i + 3]];

                /*
                **
                ** (11111111) (11111111) (11111111) <- (00111111) (0011 1111) (001111 11) (00111111)
                **      1          2          3              1         2           3           4
                */
                strDecode += (char)(((asc[0] << 2) & 0xFC) | ((asc[1] >> 4) & 0x03));
                strDecode += (char)(((asc[1] << 4) & 0xF0) | ((asc[2] >> 2) & 0x0F));
                strDecode += (char)(((asc[2] << 6) & 0xC0) | ((asc[3] >> 0) & 0x3F));
            }

            unsigned char asc[4];
            asc[0] = base64DecTable[(int)strSource[i]];
            asc[1] = base64DecTable[(int)strSource[i + 1]];
            asc[2] = base64DecTable[(int)strSource[i + 2]];
            asc[3] = base64DecTable[(int)strSource[i + 3]];

            // if end with 2 padding "="
            if ('=' == strSource[i + 2] && '=' == strSource[i + 3])
            {
                if ((asc[1] & 0x0F) != 0)
                {
                    return str;
                }
                /*
                **
                ** (11111111) (00000000) (00000000) <- (00111111) (0011 0000) (000000 00) (00000000)
                **      1          2          3              1         2           3           4
                */
                strDecode += (char)(((asc[0] << 2) & 0xFC) | ((asc[1] >> 4) & 0x03));
            }
            // elif end with 1 padding "="
            else if ('=' == strSource[i + 3])
            {
                if ((asc[2] & 0x03) != 0)
                {
                    return str;
                }
                /*
                **
                ** (11111111) (11111111) (00000000) <- (00111111) (0011 1111) (001111 00) (00000000)
                **      1          2          3              1         2           3           4
                */
                strDecode += (char)(((asc[0] << 2) & 0xFC) | ((asc[1] >> 4) & 0x03));
                strDecode += (char)(((asc[1] << 4) & 0xF0) | ((asc[2] >> 2) & 0x0F));
            }
            // else no padding
            else
            {
                /*
                **
                ** (11111111) (11111111) (11111111) <- (00111111) (0011 1111) (001111 11) (00111111)
                **      1          2          3              1         2           3           4
                */
                strDecode += (char)(((asc[0] << 2) & 0xFC) | ((asc[1] >> 4) & 0x03));
                strDecode += (char)(((asc[1] << 4) & 0xF0) | ((asc[2] >> 2) & 0x0F));
                strDecode += (char)(((asc[2] << 6) & 0xC0) | ((asc[3] >> 0) & 0x3F));
            }
            return strDecode;
        }

        std::string
        Base64::decode (const std::string &encodedStr)
        {
            return decode (encodedStr.c_str (), (int)encodedStr.length ());
        }
    } // namespace test
} // namespace cpp