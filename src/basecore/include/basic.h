#ifndef __NC_BASIC_H__
#define __NC_BASIC_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#ifdef __WINDOWS__
#ifndef _WINDOWS_
// System header files.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if !defined(_WINSOCK2API_) && !defined(_WINSOCKAPI_) && !defined(_WINSOCK2API_)
#include <winsock2.h>
#endif
#undef WIN32_LEAN_AND_MEAN

// STL header files.
#include <vector>
#include <map>
#include <set>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <cmath>

#else // __WINDOWS__
// System header files.
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>

// STL header files.
#include <vector>
#include <map>
#include <set>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <cmath>

#endif // __LINUX__

using namespace std;

inline int getLastError (void)
{
#ifdef __WINDOWS__
    return ::GetLastError ();
#else
    return errno;
#endif
}

inline void setLastError (int err)
{
#ifdef __WINDOWS__
    ::SetLastError (err);
#else
    errno = err;
#endif
}

#ifdef __WINDOWS__
    #define t_strncmp   wcsncmp
    #define t_strlen    wcslen
    #define t_snwprintf _snwprintf
    #define t_toupper   towupper
    #define t_tolower   towlower
    #define t_isdigit   iswdigit
    #define t_isspace   iswspace
    #define T_CHAR_MIN  WCHAR_MIN
    #define T_CHAR_MAX  WCHAR_MAX
#else // __LINUX__
    #define t_strncmp   strncmp
    #define t_strlen    strlen
    #define t_snwprintf snprintf
    #define t_toupper   toupper
    #define t_tolower   tolower
    #define t_isdigit   isdigit
    #define t_isspace   isspace
    #define T_CHAR_MIN  CHAR_MIN
    #define T_CHAR_MAX  CHAR_MAX
#endif

#ifdef __WINDOWS__
    typedef wchar_t tchar_t;
    typedef wchar_t utchar_t;

    #define tmain wmain

    #ifndef INT64_HAVE_DEFINED
    typedef int64_t int64;
    #define INT64_HAVE_DEFINED 1
    #endif

    #ifndef UINT64_HAVE_DEFINED
    typedef uint64_t uint64;
    #define UINT64_HAVE_DEFINED 1
    #endif

#else
    typedef char          tchar_t;
    typedef unsigned char utchar_t;

    #define tmain main

    #ifndef INT64_HAVE_DEFINED
    typedef int64_t int64;
    #define INT64_HAVE_DEFINED 1
    #endif

    #ifndef UINT64_HAVE_DEFINED
    typedef uint64_t uint64;
    #define UINT64_HAVE_DEFINED 1
    #endif

#endif

#if !defined(_T)
#define _T(str) str
#endif

#endif // !__NC_BASIC_H__