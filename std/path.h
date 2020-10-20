#ifndef CLIB_STD_PATH_H
#define CLIB_STD_PATH_H

#ifdef _WIN32
    #define PATH_SEPERATOR_CHAR '\\'
    #define PATH_SEPERATOR "\\"
#else
    #define PATH_SEPERATOR_CHAR '/'
    #define PATH_SEPERATOR "/"
#endif

#endif