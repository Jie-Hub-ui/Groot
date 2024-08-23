/*
 * file name: base64.h
 */ 
#ifndef __CPP_TEXT_BASE64_H__
#define __CPP_TEXT_BASE64_H__
#include <iostream>
#include <string.h>
namespace cpp {
    namespace text {
        class Base64
        {
        public:
            Base64() {}
            virtual ~Base64(void) {}

        public:
            /**
             * Converts a sequence of arbitrary bytes to a base64_encoded 
             * string object. 
             *
             * @param bytesToEncode         The pointer to the bytes to convert.
             * @param len                   The length of the bytes
             *
             */
            static std::string encode (const void* bytesToEncode, size_t len);
            static std::string encode (const std::string& bufToEncode);

            /**
             * Converts the base64_encoded String object to 
             *    its origin form. 
             *
             * @param encodedStr            The base64_encoded string to decode.
             *
             */
            static std::string decode (const char* bytesToDecode, int len = 0);
            static std::string decode (const std::string& encodedStr);

        }; // End class Base64
    } // namespace text
} // namespace cpp

#endif // __CPP_TEXT_BASE64_H__