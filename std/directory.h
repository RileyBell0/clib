#ifndef CLIB_STD_DIRECTORY_H
#define CLIB_STD_DIRECTORY_H

#include <dirent.h>

#include "string.h"
#include "list.h"
#include "array.h"

#define EXTENSION_SEPERATING_CHAR '.'

char* getFileExtension(char *fileName);
char* removeFileExtension(char *fileName);

dynamicArray getAllFiles(DIR* d);
dynamicArray array_GetFilesWithExtension(DIR* d, string extension);

list list_GetFilesWithExtension(DIR* d, string extension);

string getSubDirectory(string basePath, string pathSeperator, string dirName);


#endif