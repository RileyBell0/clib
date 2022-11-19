#ifndef CLIB_STD_SYSTEM_H
#define CLIB_STD_SYSTEM_H

// use OS_TYPE preprocessor definition to determine which OS you are on

#define OS_TYPE_UNKNOWN 0
#define OS_TYPE_WINDOWS 1
#define OS_TYPE_MAC 2
#define OS_TYPE_LINUX 3

// On WINDOWS
#if defined(_WIN32) || defined(_WIN64)
#define OS_TYPE OS_TYPE_WINDOWS
#define PATH_SEP "\\"
#define PATH_SEP_CHAR '\\'

// On Mac
#elif defined(__APPLE__) || defined(__MACH__)
#define OS_TYPE OS_TYPE_MAC
#define PATH_SEP "/"
#define PATH_SEP_CHAR '/'

#elif defined(__linux__)
#define OS_TYPE OS_TYPE_LINUX
#define PATH_SEP "/"
#define PATH_SEP_CHAR '/'

#else
#define OS_TYPE OS_TYPE_UNKNOWN

#endif

#endif