#ifndef CLIB_DIRECTORY_H
#define CLIB_DIRECTORY_H

#include <dirent.h>

#include "string.h"
#include "list.h"
#include "array.h"

#define EXTENSION_SEPERATING_CHAR '.'

char* getFileExtension(char *fileName);

dynamicArray getAllFiles(DIR* d);

dynamicArray getFilesWithExtension(DIR* d, string extension);

string getSubDirectory(string basePath, string pathSeperator, string dirName);


#endif