/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

#ifndef CLIB_STD_PATH_H
#define CLIB_STD_PATH_H

#include "string.h"

#ifdef _WIN32
#define PATH_SEPERATOR_CHAR '\\'
#define PATH_SEPERATOR "\\"

#else
// Assume Linux's file seperating char as a default
#define PATH_SEPERATOR_CHAR '/'
#define PATH_SEPERATOR "/"

#endif


/*
 * Returns a *new* string which is the concatenation
 * of basePath + pathSeperator + dirName
 * 
 * More efficient than doing each concatenation seperately as 
 * all required space is allocated at once
*/
string_t path_enter_directory(string_t basePath, string_t dirName);

string_t path_enter_directory(string_t basePath, string_t dirName)
{
    string_t pathSeperator = string_from_cstring(PATH_SEPERATOR);
    string_t sourceFiles = new_string(basePath.len + pathSeperator.len + dirName.len);

    string_write(&sourceFiles, &basePath, &pathSeperator, &dirName, NULL);

    return sourceFiles;
}

#endif