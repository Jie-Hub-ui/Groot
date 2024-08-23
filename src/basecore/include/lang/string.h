/***********************************************************************************************
string.h:
    C++ Foundation Library header files
    Copyright (c) Eisoo Software, Inc.(2004)

Purpose:
    String class declares, used UNICODE UTF16 char set

Author:
    Lei jie

Creating Time:
    2024-08-14
    
***********************************************************************************************/
#ifndef __CPP_LANG_STRING_H__
#define __CPP_LANG_STRING_H__
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <stdarg.h>

#define t_strncmp   strncmp
#define t_strlen    strlen
#define t_snwprintf snprintf
#define t_toupper   toupper
#define t_tolower   tolower
#define t_isdigit   isdigit
#define t_isspace   isspace
#define T_CHAR_MIN  CHAR_MIN
#define T_CHAR_MAX  CHAR_MAX

typedef char                tchar_t;
typedef unsigned char       utchar_t;
typedef long long           int64;
typedef unsigned long long  uint64;

#if !defined(_T)
#define _T(str) str
#endif

namespace cpp {
    namespace lang {
        /*
         * The String class represents characters as a series of Unicode(UTF-16) string.
         * Unlike the String class on .NET and J2SE, this class is mutable because it provides
         * a series of methods to change its members.
         */
        class String
        {
        public:
            /*
            * Initializes the empty string.
            *
            */
            explicit String()
            {
                init();
            }

            /*
            * Copy constructor, initialize this string with another string.
            *
            * @param source 	Used to initialized this string.
            */
            String(const String &source)
            {
                _str.assign(source._str);
            }

        #ifdef __ENABLE_CXX11__
            String (String &&source)
            {
                _str = std::move(source._str);
            }
        #endif

            /*
            * Initialize this string with another this from specified offset and count.
            *
            * @param source 	Used to initialized this string.
            * @param offset 	The index of a character in @param source string that 
            *					is the first to  be used to initialize character values 
            *					for the string being constructed. 
            * @param count 	The number of characters to be initialized. The default 
            *					value is  String::NO_POSITION which means "all remaining 
            *					characters".
            *
            * @throw OutOfRangeException If @offset is over than the length of @source.
            */
            String(const String &source, size_t offset, size_t count = NO_POSITION)
            {
                init(source.size());
                assign(source, offset, count);
            }

            /*
            * Initializes this string that contains @param count copies of @param ch
            *
            * @param ch 		The character value to be copied into the string being constructed.
            * @param count 	The number of characters to be initialized. 
            */
            explicit String(tchar_t ch, size_t count)
            {
                init(count);
                assign(ch, count);
            }

            /*
            * Initializes this string with a UNICODE C-style string.	
            *
            * @param ptr 		The UNICODE C-style string, if it is invalid, 
            *					construct an empty string.
            */
            String(const tchar_t* ptr)
            {
                size_t len = INIT_SIZE;
                if (ptr != 0) {
                    len = t_strlen(ptr);
                    init(len);
                    _assign(ptr, len);
                }
                else {
                    init(len);
                }
            }

            /*
            * Initializes this string with a UNICODE C-style string, the count is specified.	
            *
            * @param ptr 		The UNICODE C-style string, if it is invalid,
            *					construct an empty string.
            * @param count 	The number of characters to be initialized. 
            * @param alloc		The memory allocator for this string.
            *					The default value is null which means
            *					to use the CFL allocator.
            */
            String(const tchar_t *ptr, size_t count)
            {
                init(count);
                _assign(ptr, count);
            }

            /*
            * Destroy current string instance and release its resources.
            */
            ~String();

        public:
            /*
            * Returns the length of this string. The length is equal to the number of 16-bit
            * Unicode characters in the string.
            *
            * @return  the length of the sequence of characters represented by this string.
            */
            size_t getLength() const
            {
                return _str.length();
            }

            /*
            * Returns the largest number of elements that could be stored in a string without 
            * increasing the memory allocation of the string
            *
            * @return the size of storage currently allocated in memory to hold the string.
            */
            size_t getCapacity() const
            {
                return _str.capacity();
            }

            /*
            * Returns the maximum number of characters a string could contain.
            *
            * @return The maximum number of characters a string could contain.
            */
            size_t getMaxSize() const
            {
                return NO_POSITION;
            }

            /*
            * Tests whether the string is contains characters or not.
            * @return true if the string object contains no characters; 
            * false if it has at least one character.
            */
            bool isEmpty() const
            {
                return _str.empty();
            }

            /*
            * Assigns new string to the contents of this string.
            *
            * @param source 	The source string whose characters are to be assigned 
            *					to this string.
            * @return A reference to this string object that is being assigned. 
            */
            String &assign (const String &source);

            /*
            * Assigns new string with specified offset and count to the contents of this string.
            *
            * @param source 	The source string whose characters are to be assigned to this 
            *					string.
            * @param offset 	The index of a character in @param source string that is the 
            *					first to be used to assign character values for the string 
            *					being constructed. 
            * @param count 	The number of characters to be assigned. 
            * 
            * @throw OutOfRangeException If @offset is over than the length of @source.
            * @return A reference to this string object that is being assigned.
            */
            String &assign(const String &source, size_t offset, size_t count);

            String &_assign(const String &source, size_t offset, size_t count);

            /*
            * Assigns new UNICODE c-style string to the contents of this string.
            *
            * @param ptr 	The UNICODE c-style string whose characters are to be 
            *				assigned to this string.
            * @param num 	The number of characters to be initialized. 
            *
            * @throw ArgumentException If @param num is over than String::NO_POSITION.
            * @return A reference to this string object that is being assigned.
            */
            String &assign (const tchar_t *ptr, size_t num);

            String &_assign (const tchar_t *ptr, size_t num);

            /*
            * Assigns new UNICODE c-style string to the contents of this string.
            *
            * @param ptr 	The UNICODE c-style string whose characters are to be 
            *				assigned to this string.
            * @return A reference to this string object that is being assigned.
            * @throw ArgumentException If the ptr is null.
            */
            String &assign (const tchar_t *ptr);

            /*
            * Assigns this string that contains @param count copies of @param ch
            * @param ch 	The character value to be copied into the string being 
            *				constructed.
            * @param count The number of characters to be initialized. 
            * @return A reference to this string object that is being assigned.
            */
            String &assign (tchar_t ch, size_t count);

        #ifdef __ENABLE_CXX11__
            /*
            * Move assigns new string to the contents of this string.
            *
            * @param source 	The source string whose characters are to be assigned 
            *					to this string.
            * @return A reference to this string object that is being assigned. 
            */
            String &assign (String &&source);
        #endif

            /*
            * Adds a string to the end of this string.
            * @param source 	The string whose characters are to be appended. 
            * @return A reference to this string object that is being appended.
            */
            String &append(const String &source)
            {
                return append(source, 0, NO_POSITION);
            }

            /*
            * Adds a string with specified offset and count to the end of this string.
            *
            * @param source 	The string whose characters are to be appended. 
            * @param offset 	The index of the part of the source string supplying the 
            *					characters to be appended. 
            * @param count 	The number of characters to be appended, at most, from 
            *					the source string. 
            *
            * @throw OutOfRangeException If @offset is over than the length of @source.
            * @return A reference to this string object that is being appended.
            */
            String &append(const String &source, size_t offset, size_t count);

            /*
            * Adds a UNICODE c-style string with specified count to the end of this string.
            *
            * @param ptr 	The UNICODE c-style string to be appended. 
            * @param count The number of characters to be appended. 
            *
            * @throw ArgumentException if @num is over than String::NO_POSITION.
            * @return A reference to this string object that is being appended.
            */
            String &append(const tchar_t *ptr, size_t count);

            /*
            * Adds a UNICODE c-style string to the end of this string.
            *
            * @param ptr 	The UNICODE c-style string to be appended. 
            * @return A reference to this string object that is being appended.
            */
            String &append(const tchar_t *ptr)
            {
                return append(ptr, t_strlen(ptr));
            }

            /*
            * Adds @param ch which @param count copies to the end of this string.
            * @param ch 	The character value to be added into the string.
            * @param count The number of characters to be added. 
            *
            * @return A reference to this string object that is being added.
            */
            String &append(tchar_t ch, size_t count);

            /*
            * Returns the character value with a specified index in a string.
            * @param n 	The index of the position of the element to be returned. 
            *				the base index is 0.
            *
            * @throw OutOfRangeException if @n is over than the length of this string.
            */
            tchar_t at(size_t n) const; // read access

            /*
            * Provides a reference to the character with a specified index in a string.
            * @param n 	The index of the position of the element to be referenced. 
            *				the base index is 0.
            *
            * @throw OutOfRangeException If @n is over than the length of this string.
            * @return 	A reference to the character of the string at the position specified  
            * 			by the parameter index. The reference can be used to read/write access.
            */
            tchar_t &at(size_t n);

            /*
            * Returns the contents of a string as a C-style, null-terminated, string.
            * @return The C-style string pointer. The type is wide character (UTF16).
            */
            const tchar_t *getCStr() const
            {
                return getMyPtr();
            }

            /*
            * Remove string contents from the specified offset and count, 
            *	used the default parameter will empty all contents
            *
            * @param offset 	The index of this string begin to remove.
            * @param count 	The count character to remove for this string.
            *
            * @throw OutOfRangeException If @offset is over than the length of this string.
            * @return A reference to this string object that is being removed.
            */
            String &remove (size_t offset = 0, size_t count = NO_POSITION);

            String &_remove (size_t offset = 0, size_t count = NO_POSITION);

            /*
            * Remove the string and free memory. If just want remove the string,
            * use method remove().
            */
            void clear()
            {
                _str.clear();
            }

            /*
            * exchange contents of this string with source.
            * @param source 	The source string whose elements are to be exchanged with  
            *					those in the destination string.
            */
            void swap(String &source);

            /*
            * Sets the capacity of the string to a number at least as great as a 
            *	specified number.
            *
            * @param newSiz 	The number of characters for which memory is being reserved. 		 
            */
            void reserve(size_t newSiz = 0);

            /*
            * Copies at most a specified number of characters from an indexed position 
            * in a source string to a target Unicode character array. This method don't
            * append NULL terminate to the array.
            *
            * @param ptr 		The target character array to which the elements are to 
            *					be copied.
            * @param count 	The number of characters to be copied, at most, from the 
            *					source string. 
            * @param offset 	The beginning position in the source string from which 
            *					copies are to be made. 
            *
            * @throw OutOfRangeException If @offset is over than the length of this string.
            * @return The number of characters actually copied.
            */
            size_t copy(tchar_t *ptr, size_t count, size_t offset = 0) const;

            /*
            * Specifies a new size for a string, appending or erasing elements as required.
            * 
            * @param newSiz 	The new size of the string. 
            * @param ch 		The value that appended characters are initialized with if 
            *					additional elements are required. The default value is 0.
            */
            void resize(size_t newSiz, tchar_t ch = 0)
            {
                // determine new length, padding with ch elements as needed
                if (newSiz <= _str.size ()) {
                    _remove (newSiz);
                }
                else {
                    append (ch, newSiz - _str.size ());
                }
            }

            /*
            * Searches a Unicode string in a forward direction for the first occurrence of 
            * a substring that matches a specified sequence of characters.
            *
            * @param ptr 		The C-string for which the member function is to search
            * @param offset 	Index of the position at which the search is to begin.
            * @return 	The index of the first character of the substring searched for
            *			when successful; otherwise NO_POSITION.
            */
            size_t find(const tchar_t *ptr, size_t offset = 0) const
            {
                // look for [ptr, <null>) beginning at or after offset
                return find(ptr, offset, t_strlen (ptr));
            }

            /*
            * Searches a character in a forward direction for the first occurrence of 
            * a substring that matches a specified sequence of characters.
            *
            * @param ch 		The character value for which the member function is to search.
            * @param offset 	Index of the position at which the search is to begin.
            * @return 	The index of the first character of the substring searched for
            *			when successful; otherwise NO_POSITION.
            */
            size_t find(tchar_t ch, size_t offset = 0) const
            {
                return find((const tchar_t*)&ch, offset, 1);
            }

            /*
            * Searches a string in a forward direction for the first occurrence of 
            * a substring that matches a specified sequence of characters.
            *
            * @param source 		The string for which the member function is to search.
            * @param offset 		Index of the position at which the search is to begin.
            * @return 	The index of the first character of the substring searched for
            *			when successful; otherwise NO_POSITION.
            */
            size_t find(const String &source, size_t offset = 0) const
            {
                // look for _Right beginnng at or after _Off
                return find(source.getMyPtr(), offset, source.getLength());
            }

            /*
            * Searches a unicode string in a forward direction for the first occurrence of 
            * a substring that matches a specified sequence of characters.
            *
            * @param ptr 		The C-string for which the member function is to search
            * @param offset 	Index of the position at which the search is to begin.
            * @param count 	The number of characters, counting forward from the first  
            *					character, in the C-string for which the member function 
            *					is to search. 
            *
            * @return 	The index of the first character of the substring searched for
            *			when successful; otherwise NO_POSITION.
            */
            size_t find(const tchar_t *ptr, size_t offset, size_t count) const;

            /*
            * Searches a UNICODE string in a backward direction for the first occurrence 
            * of a substring that matches a specified sequence of characters.
            * 
            * @param ptr 		The UNICODE C-string for which the member function is to search.
            * @param offset 	The Index of the position at which the search is to begin. 
            *
            * @return 	The index of the last occurrence, when searched backwards, of the 
            *			first character of the substring when successful; otherwise NO_POSITION.
            */
            size_t rfind(const tchar_t *ptr, size_t offset = NO_POSITION) const
            {
                // look for [ptr, <null>) beginning before offset
                return rfind(ptr, offset, t_strlen (ptr));
            }

            /*
            * Searches a character in a backward direction for the first occurrence 
            * of a substring that matches a specified sequence of characters.
            * 
            * @param ch 		The character value for which the member function is to search. 
            * @param offset 	The Index of the position at which the search is to begin. 
            *
            * @return 	The index of the last occurrence, when searched backwards, of the 
            *			first character of the substring when successful; otherwise NO_POSITION.
            */
            size_t rfind(tchar_t ch, size_t offset = NO_POSITION) const
            {
                // look for ch before offset
                return rfind((const tchar_t*)&ch, offset, 1);
            }

            /*
            * Searches a string in a backward direction for the first occurrence 
            * of a substring that matches a specified sequence of characters.
            * 
            * @param source 	The string for which the member function is to search. 
            * @param offset 	The Index of the position at which the search is to begin. 
            *
            * @return 	The index of the last occurrence, when searched backwards, of the 
            *			first character of the substring when successful; otherwise NO_POSITION.
            */
            size_t rfind(const String &source, size_t offset = NO_POSITION) const
            {
                // look for source beginning before offset
                return rfind(source.getMyPtr(), offset, source.getLength());
            }

            /*
            * Searches a UNICODE string in a backward direction for the first occurrence 
            * of a substring that matches a specified sequence of characters.
            * 
            * @param ptr 		The UNICODE C-string for which the member function is to search.
            * @param offset 	The Index of the position at which the search is to begin. 
            * @param count 	The number of characters, counting forward from the first character, 
            *					in the C-string for which the member function is to search. 
            *
            * @return 	The index of the last occurrence, when searched backwards, of the 
            *			first character of the substring when successful; otherwise NO_POSITION.
            */
            size_t rfind(const tchar_t *ptr, size_t offset, size_t count) const;

            /*
            * Searches through a string for the first character that matches any 
            * 	element of a specified UNICODE string.
            *
            * @param ptr 		The UNICODE C-string for which the member function is to search. 
            * @param offset  	Index of the position at which the search is to begin.
            *
            * @return 	The index of the first character of the substring searched for 
            *			when successful; otherwise NO_POSITION.
            */
            size_t findFirstOf(const tchar_t *ptr, size_t offset = 0) const
            {
                // look for one of [ptr, <null>) at or after offset
                return findFirstOf(ptr, offset, t_strlen (ptr));
            }

            size_t findFirstOf(tchar_t ch, size_t offset = 0) const
            {
                // look for ch at or after offset
                return find((const tchar_t*)&ch, offset, 1);
            }

            size_t findFirstOf(const String &source, size_t offset = 0) const
            {
                // look for one of source at or after offset
                return findFirstOf(source.getMyPtr(), offset, source.getLength());
            }

            size_t findFirstOf(const tchar_t *ptr, size_t offset, size_t count) const;

            /*
            * Searches through a string for the last character that is an 
            *	element of a specified UNICODE string.
            *
            * @param ptr 		The UNICODE C-string for which the member function is to search. 
            * @param offset 	Index of the position at which the search is to finish. 
            * 
            * @return 	The index of the last character of the substring searched for when 
            *			successful; otherwise NO_POSITION.
            */
            size_t findLastOf(const tchar_t *ptr, size_t offset = NO_POSITION) const
            {
                // look for one of [ptr, <null>) before offset
                return findLastOf(ptr, offset, t_strlen(ptr));
            }

            size_t findLastOf(tchar_t ch, size_t offset = NO_POSITION) const
            {
                // look for ch before offset
                return rfind((const tchar_t*)&ch, offset, 1);
            }

            size_t findLastOf(const String &source, size_t offset = NO_POSITION) const
            {
                // look for one of source before offset
                return findLastOf(source.getMyPtr(), offset, source.getLength());
            }

            size_t findLastOf(const tchar_t *ptr, size_t offset, size_t count) const;

            /*
            * Searches through a string for the first character that 
            * is not any element of a specified UNICODE string.
            *
            * @param ptr 		The UNICODE C-string for which the member function is to search.
            * @param offset 	Index of the position at which the search is to begin.
            * 
            * @return 	The index of the first character of the substring searched for 
            *			when successful; otherwise NO_POSITION.
            */
            size_t findFirstNotOf(const tchar_t *ptr, size_t offset = 0) const
            {
                // look for one of [ptr, <null>) at or after offset
                return findFirstNotOf(ptr, offset, t_strlen(ptr));
            }

            size_t findFirstNotOf(tchar_t ch, size_t offset = 0) const
            {
                // look for non ch at or after offset
                return findFirstNotOf((const tchar_t*)&ch, offset, 1);
            }

            size_t findFirstNotOf(const String &source, size_t offset = 0) const
            {
                // look for none of source at or after offset
                return findFirstNotOf(source.getMyPtr(), offset, source.getLength());
            }

            size_t findFirstNotOf(const tchar_t *ptr, size_t offset, size_t count) const;

            /*
            * Searches through a string for the last character that is not any element of 
            *	a specified UNICODE string.
            *
            * @param ptr 		The UNICODE C-string for which the member function is to search. 
            * @param offset 	Index of the position at which the search is to finish.
            *
            * @return 	The index of the first character of the substring searched 
            *			for when successful; otherwise NO_POSITION.
            */
            size_t findLastNotOf(const tchar_t *ptr, size_t offset = NO_POSITION) const
            {
                // look for none of [ptr, <null>) before offset
                return findLastNotOf(ptr, offset, t_strlen(ptr));
            }

            size_t findLastNotOf(tchar_t ch, size_t offset = NO_POSITION) const
            {
                // look for non ch before offset
                return findLastNotOf((const tchar_t*)&ch, offset, 1);
            }

            size_t findLastNotOf(const String &source, size_t offset = NO_POSITION) const
            {
                // look for none of source before offset
                return findLastNotOf(source.getMyPtr(), offset, source.getLength());
            }

            size_t findLastNotOf(const tchar_t *ptr, size_t offset, size_t count) const;

            /*
            * Reports the index of the first occurrence in this string of any character string.
            * 
            * @param chars 	A character string containing one or more characters to seek. 
            * @return 	The index position of the first occurrence in this instance where any 
            *		character in @chars was found; otherwise, -1 if no character in @chars was found.
            */
            size_t findAny(const String &chars) const
            {
                return findAny(0, chars);
            }

            /*
            * Reports the index of the first occurrence in this string of any character string.
            * 
            * @param offset 	Index of the position at which the search is to begin.
            * @param chars 	A character string containing one or more characters to seek. 
            *
            * @throw OutOfRangeException If offset is over the length of this string.
            * @return 	The index position of the first occurrence in this string where any 
            *			character in @chars was found; otherwise, NO_POSITION if no character in
            *			@chars was found.
            */
            size_t findAny(size_t offset, const String &chars) const;

            /*
            * Reports the index of the first not occurrence in this string of any character string.
            * 
            * @param chars 	A character string containing one or more characters to seek. 
            * @return 	The index position of the first not occurrence in this instance where any 
            *		character in @chars was found; otherwise, -1 if no character in @chars was found.
            */
            size_t findNotOfAny(const String &chars) const
            {
                return findNotOfAny(0, chars);
            }

            /*
            * Reports the index of the first occurrence in this string of any character string.
            * 
            * @param offset 	Index of the position at which the search is to begin.
            * @param chars 	A character string containing one or more characters to seek. 
            *
            * @throw OutOfRangeException If offset is over the length of this string.
            * @return 	The index position of the first occurrence in this string where any 
            *			character in @chars was found; otherwise, NO_POSITION if no character in
            *			@chars was found.
            */
            size_t findNotOfAny(size_t offset, const String &chars) const;

            /*
            * Reports the index of the last occurrence in this string of any character string.
            *
            * @param chars 	A character string containing one or more characters to seek.
            * @return 	The index position of the last occurrence in this string where any 
            *			character in @chars was found; otherwise, NO_POSITION if no character in 
            *			@chars was found.
            */
            size_t findLastOfAny(const String &chars) const
            {
                return findLastOfAny(getLength() - 1, chars);
            }

            /*
            * Reports the index of the last occurrence in this string of any character string.
            * 
            * @param offset 	Index of the position at which the search is to begin.
            * @param chars 	A character string containing one or more characters to seek. 
            *
            * @throw OutOfRangeException If offset is over the length of this string.
            * @return 	The index position of the last occurrence in this string where any 
            *			character in @chars was found; otherwise, NO_POSITION if no character in
            *			@chars was found.
            */
            size_t findLastOfAny(size_t offset, const String &chars) const;

            /*
            * Reports the index of the last not occurrence in this string of any character string.
            *
            * @param chars 	A character string containing one or more characters to seek.
            * @return 	The index position of the last not occurrence in this string where any 
            *			character in @chars was found; otherwise, NO_POSITION if no character in 
            *			@chars was found.
            */
            size_t findLastNotOfAny(const String &chars) const
            {
                return findLastNotOfAny(getLength() - 1, chars);
            }

            /*
            * Reports the index of the last not occurrence in this string of any character string
            *	from the specified starting index.
            *
            * @param chars 	A character string containing one or more characters to seek.
            * @param index 	Index of the position at which the search is to begin. 
            *
            * @throw OutOfRangeException If the offset is over the length of this string.
            * @return 	The index position of the last not occurrence in this string where any 
            *			character in @chars was found; otherwise, NO_POSITION if no character 
            *			in @chars was found.
            */
            size_t findLastNotOfAny(size_t offset, const String &chars) const;

            /*
            * Copies a substring of at most some number of characters from a string 
            *	beginning from a specified position.
            *
            * @param offset 	An index locating the element at the position from which 
            *					the copy of the string is made, with a default value of 0. 
            * @param count 	The number of characters that are to be copied if they are present. 
            *
            * @throw OutOfRangeException If @offset is over than the length of this string.
            * @return 	A substring object that is a copy of elements of the string operand 
            *			beginning at the position specified by the first argument.
            */
            String subString(size_t offset = 0, size_t count = NO_POSITION) const
            {
                return String(*this, offset, count);
            }

            /*
            * Compares a string with a specified string to determine if the two strings are equal 
            * 	or if one is lexicographically less than the other.
            *
            * @param source The string that is to be compared to this string.
            * @return 	A < 0 value if this string is less than the parameter string; 
            *			= 0 if the two strings are equal; 
            *			or a > 0 value if this string is greater than the parameter string.
            */
            int compare (const String& source) const;

            /*
            * Compares a string with a specified string to determine if the two strings are equal 
            * 	or if one is lexicographically less than the other.
            *
            * @param offset 	The index of this string at which the comparison begins.
            * @param number 	The maximum number of characters from this string to be compared. 
            * @param source 	The string that is to be compared to this string.
            *
            * @throw OutOfRangeException If the offset is over the length of this string.
            * @return 	A < 0 value if this string is less than the parameter string; 
            *			= 0 if the two strings are equal; 
            *			or a > 0 value if this string is greater than the parameter string.
            */
            int compare (size_t offset, size_t number, const String& source) const
            {
                // compare [offset, offset + number) with source
                return compare (offset, number, source.getMyPtr (), source.getLength ());
            }

            /*
            * Compares a string with a specified string to determine if the two strings are equal 
            * 	or if one is lexicographically less than the other.
            *
            * @param offset 	The index of this string at which the comparison begins.
            * @param number 	The maximum number of characters from this string to be compared. 
            * @param source 	The string that is to be compared to this string.
            * @param roffset 	The index of the parameter string at which the comparison begins. 
            * @param count 	The maximum number of characters from the parameter string to be 
            *					compared.
            *
            * @throw OutOfRangeException If the offset is over the length of this string.
            * @return 	A < 0 value if this string is less than the parameter string; 
            *			= 0 if the two strings are equal; 
            *			or a > 0 value if this string is greater than the parameter string.
            */
            int compare (size_t offset, size_t number, const String& source, size_t roffset, size_t count) const;

            /*
            * Compares a string with a UNICODE C-string to determine if the two strings are equal 
            * 	or if one is lexicographically less than the other.
            *
            * @param ptr The UNICODE C-string to be compared to this string. 
            *
            * @return 	A < 0 value if this string is less than the parameter string; 
            *			= 0 if the two strings are equal; 
            *			or a > 0 value if this string is greater than the parameter string.
            */
            int compare (const tchar_t* ptr) const
            {
                // compare [0, size) with [ptr, <null>)
                return compare (0, size (), ptr, t_strlen (ptr));
            }

            int compare (const tchar_t* ptr, size_t len) const
            {
                // compare [0, size) with [ptr, <null>)
                return compare (0, size (), ptr, len);
            }

            /*
            * Compares a string with a UNICODE C-string to determine if the two strings are equal 
            * 	or if one is lexicographically less than the other.
            *
            * @param offset The index of this string at which the comparison begins.
            * @param number The maximum number of characters from this string to be compared. 
            * @param ptr 	 The UNICODE C-string to be compared to this string. 
            *
            * @return 	A < 0 value if this string is less than the parameter string; 
            *			= 0 if the two strings are equal; 
            *			or a > 0 value if this string is greater than the parameter string.
            */
            int compare (size_t offset, size_t number, const tchar_t* ptr) const
            {
                // compare [offset, offset + number) with [ptr, <null>)
                return compare (offset, number, ptr, t_strlen (ptr));
            }

            /*
            * Compares a string with a UNICODE C-string to determine if the two strings are equal 
            * 	or if one is lexicographically less than the other.
            *
            * @param offset The index of this string at which the comparison begins.
            * @number The maximum number of characters from this string to be compared. 
            * @param ptr The UNICODE C-string to be compared to this string. 
            * @param count The maximum number of characters from the UNICODE C-string to be compared.
            *
            * @return A < 0 value if this string is less than the parameter string; 
            *	= 0 if the two strings are equal; 
            *	or a > 0 value if this string is greater than the parameter string.
            */
            int compare (size_t offset, size_t number, const tchar_t* ptr, size_t count) const;

            /*
            * Compares a string with a specified string and ignore case to determine if the 
            * 	two strings are equal or if one is lexicographically less than the other.
            *
            * @param source The string that is to be compared to this string.
            * @return 	A < 0 value if this string is less than the parameter string; 
            *			= 0 if the two strings are equal; 
            *			or a > 0 value if this string is greater than the parameter string.
            */
            int compareIgnoreCase (const String& source) const
            {
                // compareIgnoreCase [0, size) with source
                return compareIgnoreCase (0, size (), source.getMyPtr (), source.getLength ());
            }

            /*
            * Compares a string with a specified string and ignore case to determine if the 
            *	two strings are equal or if one is lexicographically less than the other.
            *
            * @param offset 	The index of this string at which the comparison begins.
            * @param number 	The maximum number of characters from this string to be compared. 
            * @param source 	The string that is to be compared to this string.
            *
            * @throw OutOfRangeException If the offset is over the length of this string.
            * @return 	A < 0 value if this string is less than the parameter string; 
            *			= 0 if the two strings are equal; 
            *			or a > 0 value if this string is greater than the parameter string.
            */
            int compareIgnoreCase (size_t offset, size_t number, const String& source) const
            {
                // compareIgnoreCase [offset, offset + number) with source
                return compareIgnoreCase (offset, number, source.getMyPtr (), source.getLength ());
            }

            /*
            * Compares a string with a specified string and ignore case to determine if the 
            *	two strings are equal or if one is lexicographically less than the other.
            *
            * @param offset 	The index of this string at which the comparison begins.
            * @param number 	The maximum number of characters from this string to be 
            *					compared. 
            * @param source 	The string that is to be compared to this string.
            * @param roffset 	The index of the parameter string at which the comparison begins. 
            * @param count 	The maximum number of characters from the parameter string to be 
            *					compared.
            *
            * @throw OutOfRangeException If the offset is over the length of this string.
            * @return 	A < 0 value if this string is less than the parameter string; 
            *			= 0 if the two strings are equal; 
            *			or a > 0 value if this string is greater than the parameter string.
            */
            int compareIgnoreCase (size_t offset, size_t number, const String& source, size_t roffset, size_t count) const;

            /*
            * Compares a string with a UNICODE C-string and ignore case to determine if the 
            *	two strings are equal or if one is lexicographically less than the other.
            *
            * @param ptr The UNICODE C-string to be compared to this string. 
            *
            * @return 	A < 0 value if this string is less than the parameter string; 
            *			= 0 if the two strings are equal; 
            *			or a > 0 value if this string is greater than the parameter string.
            */
            int compareIgnoreCase (const tchar_t* ptr) const
            {
                // compareIgnoreCase [0, size) with [ptr, <null>)
                return compareIgnoreCase (0, size (), ptr, t_strlen (ptr));
            }

            /*
            * Compares a string with a UNICODE C-string and ignore case to determine if the 
            *	two strings are equal or if one is lexicographically less than the other.
            *
            * @param offset 	The index of this string at which the comparison begins.
            * @param number 	The maximum number of characters from this string to be compared. 
            * @param ptr 		The UNICODE C-string to be compared to this string. 
            *
            * @return 	A < 0 value if this string is less than the parameter string; 
            *			= 0 if the two strings are equal; 
            *			or a > 0 value if this string is greater than the parameter string.
            */
            int compareIgnoreCase (size_t offset, size_t number, const tchar_t* ptr) const
            {
                // compareIgnoreCase [offset, offset + number) with [ptr, <null>)
                return compareIgnoreCase (offset, number, ptr, t_strlen (ptr));
            }

            /*
            * Compares a string with a UNICODE C-string to determine if the two strings are equal 
            * 	or if one is lexicographically less than the other.
            *
            * @param offset 	The index of this string at which the comparison begins.
            * @param number 	The maximum number of characters from this string to be compared. 
            * @param ptr 		The UNICODE C-string to be compared to this string. 
            * @param count 	The maximum number of characters from the UNICODE C-string to 
            *					be compared.
            *
            * @return 	A < 0 value if this string is less than the parameter string; 
            *			= 0 if the two strings are equal; 
            *			or a > 0 value if this string is greater than the parameter string.
            */
            int compareIgnoreCase (size_t offset, size_t number, const tchar_t* ptr, size_t count) const;

            /*
            * Replaces character in this string with specified character.
            *
            * @param oldValue 	The character to be replaced.
            * @param newValue 	The character to be copied into.
            * @param replaceAll 	Flag to determine whether to replace all @oldValue.
            *
            * @return A reference to this string object that be replaced. 
            */
            String& replace (tchar_t oldValue, tchar_t newValue, bool replaceAll = false);

            /*
            * Replaces characters in this string at a specified position with specified string.
            *
            * @param offset 	The index of this string at which the replacement begins.
            * @param number 	The maximum number of characters to be replaced in this string. 
            * @param source 	The string that is to be a source of characters for this string. 
            *
            * @return A reference to this string object that be replaced. 
            */
            String& replace (size_t offset, size_t number, const String& source)
            {
                // replace [offset, offset + number) with source
                return replace (offset, number, source, 0, NO_POSITION);
            }

            /*
            * Replaces characters in this string at a specified position with specified string.
            *
            * @param offset 	The index of this string at which the replacement begins.
            * @param number 	The maximum number of characters to be replaced in this string. 
            * @param source 	The string that is to be a source of characters for this string. 
            * @param roffset 	The index of the parameter string at which the copying begins. 
            * @param count 	The maximum number of characters to be used from the parameter 
            *					string.
            *
            * @throw OutOfRangeException if offset or roffset is over the length of the string.
            * @return A reference to this string object that be replaced. 
            */
            String& replace (size_t offset, size_t number, const String& source, size_t roffset, size_t count);

            /*
            * Replaces characters in this string at a specified position with specified string.
            *
            * @param offset 	The index of this string at which the replacement begins.
            * @param number 	The maximum number of characters to be replaced in this string. 
            * @param ptr 		The UNICODE C-string that is to be a source of characters for 
            *					this string. 		 
            * @param count 	The maximum number of characters to be used from the parameter 
            *					C-string.
            *
            * @throw OutOfRangeException If the offset is over the length of the string.
            * @return A reference to this string object that be replaced. 
            */
            String& replace (size_t offset, size_t number, const tchar_t* ptr, size_t count);

            /*
            * Replaces characters in this string at a specified position with specified string.
            *
            * @param offset 	The index of this string at which the replacement begins.
            * @param number 	The maximum number of characters to be replaced in this string. 
            * @param ptr 		The UNICODE C-string that is to be a source of characters for 
            *					this string. 		 		 
            *
            * @throw OutOfRangeException If the offset is over the length of the string.
            * @return A reference to this string object that be replaced. 
            */
            String& replace (size_t offset, size_t number, const tchar_t* ptr)
            {
                // replace [offset, offset + number) with [ptr, <null>)
                return replace (offset, number, ptr, t_strlen (ptr));
            }

            /*
            * Replaces characters in this string at a specified position with specified character.
            *
            * @param offset 	The index of this string at which the replacement begins.
            * @param number 	The maximum number of characters to be replaced in this string. 
            * @param count 
            * @param ch 		The character that is to be a source for this string.
            *
            * @throw OutOfRangeException If the offset is over the length of the string.
            * @return A reference to this string object that be replaced. 
            */
            String& replace (size_t offset, size_t number, size_t count, tchar_t ch);

            String& insert (size_t offset, const String& source)
            {
                // insert source at offset
                return insert (offset, source, 0, NO_POSITION);
            }

            String& insert (size_t offset, const String& source, size_t roffset, size_t count);

            String& insert (size_t offset, const tchar_t* ptr, size_t count);

            String& insert (size_t offset, const tchar_t* ptr)
            {
                // insert [ptr, <null>) at offset
                return insert (offset, ptr, t_strlen (ptr));
            }

            String& insert (size_t offset, tchar_t ch, size_t count);

            /*
            * Convert to upper case in place.
            */
            void toUpper (void);

            /*
            * Convert to lower case in place.
            */
            void toLower (void);

            /*
            * Removes all occurrences of a set of specified character from the 
            *	beginning and end of this string.
            *
            * @param ch 	The character specifies to remove
            * @return The string that has been trimmed.  
            */
            String trim (tchar_t ch =  (_T(' '))) const;

            /*
            * Get first count characters.
            */
            String leftString (size_t count) const;

            /*
            * Get last count characters.
            */
            String rightString (size_t count) const;

            /*
            * Get all characters before the first occurrence of ch.
            * @return The whole string if ch not found
            */
            String beforeFirst (tchar_t ch) const
            {
                size_t offset = findFirstOf (ch);
                return offset == String::NO_POSITION ? *this : String (*this, 0, offset);
            }

            /*
            * Get all characters before the last occurrence of ch.
            * @return The empty string if ch not found.
            */
            String beforeLast (tchar_t ch) const
            {
                size_t offset = findLastOf (ch);
                return offset == String::NO_POSITION ? _T("") : String (*this, 0, offset);
            }

            /*
            * Get all characters after the first occurrence of ch.
            * @return The empty string if ch not found.
            */
            String afterFirst (tchar_t ch) const
            {
                size_t offset = findFirstOf (ch);
                return offset == String::NO_POSITION ? _T("") : String (getMyPtr () + offset + 1, getLength () - offset - 1);
            }

            /*
            * Get all characters after the last occurrence of ch.
            * @return the whole string if ch not found.
            */
            String afterLast (tchar_t ch) const
            {
                size_t offset = findLastOf (ch);

                return offset == String::NO_POSITION ? *this : String (getMyPtr () + offset + 1, getLength () - offset - 1);
            }

            /*
            * Check that the string starts with prefix.
            */
            bool startsWith (const String& prefix) const
            {
                return startsWith (prefix.getMyPtr ());
            }

            bool startsWith (const tchar_t* prefix) const;

            /*
            * Minimize the string's memory
            */
            void shrink (void)
            {
                String tmp (*this, 0, this->getLength ());
                swap (tmp);
            }

            /*
            * Count the number of characters
            */
            int freq (tchar_t ch) const;

            /*
            * Creates an array of strings by splitting this string with a 
            *	specified separator string.
            * The separator is searched for and, if found, the substring 
            *	preceding the occurrence is stored as the first element in 
            *	the array of strings. It is continued in this manner by searching 
            *	the substring that follows the occurrence.
            * On the other hand, if the separator is not found, the array of 
            *	strings will contain this instance as its only element. 
            *
            * @param separators	The specified separator string. 
            * @param substrs		The vector returns the separated strings.
            * @param oneInSeparators Flags whether only use any of one character in @separators.
            */
            void split (const String& separators, std::vector<String>& substrs, bool oneInSeparators = true) const;

            /*
            * Separates this string with the specified separator.
            *
            * @param separator		The specified separator character.
            * @param substrs		The vector returns the separated strings.
            */
            void split (tchar_t separator, std::vector<String>& substrs) const
            {
                split (String (separator, 1), substrs);
            }

            // public operator
        public:
            String& operator= (const String& source)
            {
                return assign (source);
            }

            String& operator= (const std::basic_string<tchar_t>& source)
            {
                _str = source;
                return *this;
            }

        #ifdef __ENABLE_CXX11__
            String& operator= (String&& source)
            {
                return assign (std::move (source));
            }

            String& operator= (std::basic_string<tchar_t>&& source)
            {
                _str = std::move (source);
                return *this;
            }
        #endif

            String& operator= (const tchar_t* ptr)
            {
                return assign (ptr);
            }

            String& operator= (tchar_t ch)
            {
                return assign (ch, 1);
            }

            String& operator+= (const String& source)
            {
                return append (source);
            }

            String& operator+= (const tchar_t* ptr)
            {
                return append (ptr);
            }

            String& operator+= (tchar_t ch)
            {
                return append (ch, (size_t)1);
            }

            /*
            * Operator version of at
            */
            tchar_t operator[] (size_t n) const
            {
                return at (n);
            }
            tchar_t& operator[] (size_t n)
            {
                return at (n);
            }

            /*
            * Append an bool into string
            */
            String& operator<< (bool b)
            {
                if (b) {
                    return append (_T("True"), 4);
                }
                return append (_T("False"), 5);
            }

            /*
            * Append an int into string
            */
            String& operator<< (int v)
            {
                std::string str = std::to_string (v);
                return append (str.data (), str.size ());
            }

            /*
            * Append an unsigned int into string
            */
            String& operator<< (unsigned int v)
            {
                std::string str = std::to_string (v);
                return append (str.data (), str.size ());
            }

            /*
            * Append a int64 into string
            */
            String& operator<< (const int64& v)
            {
                std::string str = std::to_string (v);
                return append (str.data (), str.size ());
            }

            /*
            * Append an unsigned int64 into string
            */
            String& operator<< (const uint64& v)
            {
                std::string str = std::to_string (v);
                return append (str.data (), str.size ());
            }

            /*
            * Append a float into string
            */
            String& operator<< (float v)
            {
                std::string str = std::to_string (v);
                return append (str.data (), str.size ());
            }

            /*
            * Append a double into string
            */
            String& operator<< (double v)
            {
                std::string str = std::to_string (v);
                return append (str.data (), str.size ());
            }

            String& operator<< (const String& source)
            {
                return append (source);
            }

            String& operator<< (const tchar_t* ptr)
            {
                return append (ptr);
            }

            String& operator<< (tchar_t ch)
            {
                return append (ch, (size_t)1);
            }

            /*
            * This method writes formatted data to a String in the same way that sprintf 
            *	formats data into a C-style character array.
            *
            * @param fmt 	Specifies a format-control string.
            * @throw FormatException If cannot format the string.
            */
            void format (const tchar_t* fmt, ...);

            /*
            * This method writes formatted data to a String in the same way that vprintf 
            *	formats data into a C-style character array.
            *
            * @param fmt 	Specifies a format-control string.
            * @param ap 	Pointer of list of arguments.
            * @throw FormatException If cannot format the string.
            */
            void formatv (const tchar_t* fmt, va_list ap);

            // for std::string compat

            String (const std::basic_string<tchar_t>& str)
                : _str (str)
            {
            }

        #ifdef __ENABLE_CXX11__
            String (std::basic_string<tchar_t>&& str)
                : _str (std::move (str))
            {
            }
        #endif

            std::basic_string<tchar_t>& str ()
            {
                return _str;
            }

            const std::basic_string<tchar_t>& str () const
            {
                return _str;
            }

            operator std::basic_string<tchar_t> () const
            {
                return _str;
            }

            operator std::basic_string<tchar_t>& ()
            {
                return _str;
            }

            size_t size () const
            {
                return getLength ();
            }
            size_t length () const
            {
                return getLength ();
            }

            tchar_t* data ()
            {
                return getMyPtr ();
            }

            const tchar_t* data () const
            {
                return getCStr ();
            }

            const tchar_t* c_str () const
            {
                return getCStr ();
            }

            bool empty () const
            {
                return isEmpty ();
            }

            String substr (size_t pos, size_t count = NO_POSITION) const
            {
                return subString (pos, count);
            }

            size_t find_first_of (const String& str, size_t pos = NO_POSITION) const
            {
                return findAny (min_value (pos, size () - 1), str);
            }

            size_t find_first_of (const tchar_t ch, size_t pos = NO_POSITION) const
            {
                return find_first_of (String (ch, (size_t)1), pos);
            }

            size_t find_first_not_of (const String& str, size_t pos = NO_POSITION) const
            {
                return findNotOfAny (min_value (pos, size () - 1), str);
            }

            size_t find_first_not_of (const tchar_t ch, size_t pos = NO_POSITION) const
            {
                return find_first_not_of (String (ch, (size_t)1), pos);
            }

            size_t find_last_of (const String& str, size_t pos = NO_POSITION) const
            {
                return findLastOfAny (min_value (pos, size () - 1), str);
            }

            size_t find_last_of (const tchar_t ch, size_t pos = NO_POSITION) const
            {
                return find_last_of (String (ch, (size_t)1), pos);
            }

            size_t find_last_not_of (const String& str, size_t pos = NO_POSITION) const
            {
                return findLastNotOfAny (min_value (pos, size () - 1), str);
            }

            size_t find_last_not_of (const tchar_t ch, size_t pos = NO_POSITION) const
            {
                return find_last_not_of (String (ch, (size_t)1), pos);
            }

            // windows extra
        #ifdef __WINDOWS__
            String (const char* ptr)
            {
                size_t len = INIT_SIZE;
                if (ptr != 0)
                {
                    len = strlen (ptr) * 2;
                    init (len);
                    assign (ptr);
                }
                else
                {
                    init (len);
                }
            }
            String (const char* ptr, size_t count)
            {
                init (count * 2);
                assign (ptr, count);
            }
            String& operator= (const string& str)
            {
                _str = utf8_to_utf16 (str);
                return *this;
            }
            String& operator= (const char* ptr)
            {
                return assign (ptr);
            }
            String& operator+= (const char* ptr)
            {
                return append (ptr);
            }
            String& operator<< (const char* ptr)
            {
                return append (ptr);
            }
            String& assign (const char* ptr, size_t num);
            String& assign (const char* ptr);
            String& append (const char* ptr, size_t count);
            String& append (const char* ptr)
            {
                return append (ptr, strlen (ptr));
            }
            size_t find (const char* ptr, size_t offset = 0) const
            {
                return find (ptr, offset, ::strlen (ptr));
            }
            size_t find (const char* ptr, size_t offset, size_t count) const;
            size_t rfind (const char* ptr, size_t offset = 0) const
            {
                return rfind (ptr, offset, ::strlen (ptr));
            }
            size_t rfind (const char* ptr, size_t offset, size_t count) const;
            size_t findFirstOf (const char* ptr, size_t offset = 0) const
            {
                return findFirstOf (ptr, offset, ::strlen (ptr));
            }
            size_t findFirstOf (const char* ptr, size_t offset, size_t count) const;
            size_t findLastOf (const char* ptr, size_t offset = 0) const
            {
                return findLastOf (ptr, offset, ::strlen (ptr));
            }
            size_t findLastOf (const char* ptr, size_t offset, size_t count) const;
            size_t findFirstNotOf (const char* ptr, size_t offset = 0) const
            {
                return findFirstNotOf (ptr, offset, ::strlen (ptr));
            }
            size_t findFirstNotOf (const char* ptr, size_t offset, size_t count) const;
            size_t findLastNotOf (const char* ptr, size_t offset = 0) const
            {
                return findLastNotOf (ptr, offset, ::strlen (ptr));
            }
            size_t  findLastNotOf (const char* ptr, size_t offset, size_t count) const;
            bool    startsWith (const char* prefix) const;
            String& insert (size_t offset, const char* ptr, size_t count);
            String& insert (size_t offset, const char* ptr)
            {
                return insert (offset, ptr, ::strlen (ptr));
            }
            void format (const char* fmt, ...);
            void formatv (const char* fmt, va_list ap);

            static std::wstring to_wstring (const std::string& str, UINT codepage);
            static std::string  to_string (const std::wstring& str, UINT codepage);
            static std::string  utf16_to_utf8 (const std::wstring& utf16);
            static std::wstring utf8_to_utf16 (const std::string& utf8);
            static std::string  utf16_to_ascii (const std::wstring& utf16);
            static std::wstring ascii_to_utf16 (const std::string& ascii);
            static std::string  utf8_to_ascii (const std::string& utf8);
            static std::string  ascii_to_utf8 (const std::string& ascii);

            operator std::string () const
            {
                return utf16_to_utf8 (_str);
            }

            String (const std::string& str, bool is_ascii = false)
            {
                if (is_ascii)
                {
                    _str = ascii_to_utf16 (str);
                }
                else
                {
                    _str = utf8_to_utf16 (str);
                }
            }
        #endif

            // static method
        public:
            // number to string
            static String toString (bool b)
            {
                String str;
                str << b;

                return str;
            }

            static String toString (short s)
            {
                String str;
                str << s;

                return str;
            }

            static String toString (unsigned short us)
            {
                String str;
                str << us;

                return str;
            }

            static String toString (int i)
            {
                String str;
                str << i;

                return str;
            }

            static String toString (unsigned int ui)
            {
                String str;
                str << ui;

                return str;
            }

            static String toString (const int64& l)
            {
                String str;
                str << l;

                return str;
            }

            static String toString (const uint64& ul)
            {
                String str;
                str << ul;

                return str;
            }

            static String toString (float f)
            {
                String str;
                str << f;

                return str;
            }

            static String toString (double d)
            {
                String str;
                str << d;

                return str;
            }

            // internal methods
        private:
            enum
            {
                /* From the pooled allocator, this init size always used
                    the third slot(24) fully */
                INIT_SIZE = 9
            };

            void init (size_t initsize = INIT_SIZE);

            tchar_t* getMyPtr (void)
            {
                // determine current pointer to buffer for mutable string
                return const_cast<tchar_t*> (_str.data ());
            }

            const tchar_t* getMyPtr (void) const
            {
                // determine current pointer to buffer for mutable string
                return _str.data ();
            }

            bool inside (const tchar_t* ptr)
            {
                // test if ptr points inside string
                if (ptr < getMyPtr () || getMyPtr () + getLength () <= ptr)
                    return (false); // don't ask
                else
                    return (true);
            }

            template<class T>
            const T& min_value (const T& a, const T& b) const
            {
                return (b < a) ? b : a;
            }
            // Data members
        private:
            std::basic_string<tchar_t> _str;

        public:
            static const size_t NO_POSITION;
            /*
            * The empty string instance.
            */
            static const String EMPTY;

        }; // End class String

        const size_t String::NO_POSITION = (size_t)(-1);

        /////////////////////////////////////////////////////////////////////////////////////////////////
        // String comparison functions: operator versions are always case sensitive

        inline bool
        operator== (const String& s1, const String& s2)
        {
            return (s1.getLength () == s2.getLength ()) && (s1.compare (s2) == 0);
        }

        inline bool
        operator== (const String& s1, const tchar_t* s2)
        {
            return (s1.compare (s2) == 0);
        }

        inline bool
        operator== (const tchar_t* s1, const String& s2)
        {
            return (s2.compare (s1) == 0);
        }

        inline bool
        operator!= (const String& s1, const String& s2)
        {
            return (s1.getLength () != s2.getLength ()) || (s1.compare (s2) != 0);
        }

        inline bool
        operator!= (const String& s1, const tchar_t* s2)
        {
            return (s1.compare (s2) != 0);
        }

        inline bool
        operator!= (const tchar_t* s1, const String& s2)
        {
            return (s2.compare (s1) != 0);
        }

        inline bool
        operator< (const String& s1, const String& s2)
        {
            return (s1.compare (s2) < 0);
        }

        inline bool
        operator< (const String& s1, const tchar_t* s2)
        {
            return (s1.compare (s2) < 0);
        }

        inline bool
        operator< (const tchar_t* s1, const String& s2)
        {
            return (s2.compare (s1) > 0);
        }

        inline bool
        operator> (const String& s1, const String& s2)
        {
            return (s1.compare (s2) > 0);
        }

        inline bool
        operator> (const String& s1, const tchar_t* s2)
        {
            return (s1.compare (s2) > 0);
        }

        inline bool
        operator> (const tchar_t* s1, const String& s2)
        {
            return (s2.compare (s1) < 0);
        }

        inline bool
        operator<= (const String& s1, const String& s2)
        {
            return (s1.compare (s2) <= 0);
        }

        inline bool
        operator<= (const String& s1, const tchar_t* s2)
        {
            return (s1.compare (s2) <= 0);
        }

        inline bool
        operator<= (const tchar_t* s1, const String& s2)
        {
            return (s2.compare (s1) >= 0);
        }

        inline bool
        operator>= (const String& s1, const String& s2)
        {
            return (s1.compare (s2) >= 0);
        }

        inline bool
        operator>= (const String& s1, const tchar_t* s2)
        {
            return (s1.compare (s2) >= 0);
        }

        inline bool
        operator>= (const tchar_t* s1, const String& s2)
        {
            return (s2.compare (s1) <= 0);
        }

        inline String
        operator+ (const String& str1, const String& str2)
        {
            String s = str1;
            s += str2;

            return s;
        }

        inline String
        operator+ (const String& str, tchar_t ch)
        {
            String s = str;
            s += ch;

            return s;
        }

        inline String
        operator+ (tchar_t ch, const String& str)
        {
            String s (ch, 1);
            s += str;

            return s;
        }

        inline String
        operator+ (const String& str, const tchar_t* ptr)
        {
            String s = str;
            s += ptr;

            return s;
        }

        inline String
        operator+ (const tchar_t* ptr, const String& str)
        {
            String s (ptr);
            s += str;

            return s;
        }

        #ifdef __WINDOWS__
        inline bool
        operator== (const String& s1, const char* s2)
        {
            return (s1.compare (s2) == 0);
        }

        inline bool
        operator== (const char* s1, const String& s2)
        {
            return (s2.compare (s1) == 0);
        }

        inline bool
        operator!= (const String& s1, const char* s2)
        {
            return (s1.compare (s2) != 0);
        }

        inline bool
        operator!= (const char* s1, const String& s2)
        {
            return (s2.compare (s1) != 0);
        }

        inline bool
        operator< (const String& s1, const char* s2)
        {
            return (s1.compare (s2) < 0);
        }

        inline bool
        operator< (const char* s1, const String& s2)
        {
            return (s2.compare (s1) > 0);
        }

        inline bool
        operator> (const String& s1, const char* s2)
        {
            return (s1.compare (s2) > 0);
        }

        inline bool
        operator> (const char* s1, const String& s2)
        {
            return (s2.compare (s1) < 0);
        }

        inline bool
        operator<= (const String& s1, const char* s2)
        {
            return (s1.compare (s2) <= 0);
        }

        inline bool
        operator<= (const char* s1, const String& s2)
        {
            return (s2.compare (s1) >= 0);
        }

        inline bool
        operator>= (const String& s1, const char* s2)
        {
            return (s1.compare (s2) >= 0);
        }

        inline bool
        operator>= (const char* s1, const String& s2)
        {
            return (s2.compare (s1) <= 0);
        }

        inline String
        operator+ (const String& str, const char* ptr)
        {
            String s = str;
            s += ptr;

            return s;
        }

        inline String
        operator+ (const char* ptr, const String& str)
        {
            String s (ptr);
            s += str;

            return s;
        }
        #endif
    } // namespace lang
} // namespace cpp

namespace std {
    template<>
    struct hash<cpp::lang::String>
    {
        size_t operator() (const cpp::lang::String &_Str) const
        { // hash string to size_t value
            size_t _Val  = (size_t)0xdeadbeef;
            size_t _Size = _Str.getLength ();
            if (0 < _Size)
            { // add one or more elements
                size_t _Stride = (_Size / 16) + 1;
                _Size -= _Stride; // protect against _Size near _Str.max_size()
                for (size_t _Idx = 0; _Idx <= _Size; _Idx += _Stride)
                    _Val += (size_t)_Str[(int)_Idx];
            }
            return (_Val);
        }
    };

} // namespace std

#endif // __CPP_LANG_STRING_H__
