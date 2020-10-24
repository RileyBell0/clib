/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

/*
 * For dealing with directories
 * 
 * Verified to work with Linux file systems
*/

#ifndef CLIB_STD_DIRECTORY_H
#define CLIB_STD_DIRECTORY_H

#include <dirent.h>

#include "path.h"
#include "string.h"
#include "list.h"
#include "fileIO.h"

char *getFileExtension(char *fileName);
char *removeFileExtension(char *fileName);

list_t getAllFiles(DIR *d);
list_t getAllDirectoryEntries(DIR *d);

list_t getFilesWithExtension(DIR *d, string_t extension);
list_t getFoldersInDir(DIR *d);

string_t getSubDirectory(string_t basePath, string_t pathSeperator, string_t dirName);
list_t getAllDirectoryEntryNames(DIR *d);

list_t getFilesWithExtensionRecursive(DIR *d, string_t path, string_t pathSeperator, string_t extension);

#endif