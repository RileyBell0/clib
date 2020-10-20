#ifndef CLIB_STD_DIRECTORY_H
#define CLIB_STD_DIRECTORY_H

#include <dirent.h>

#include "string.h"
#include "list.h"

#define EXTENSION_SEPERATING_CHAR '.'

char* getFileExtension(char *fileName);
char* removeFileExtension(char *fileName);

list getAllFiles(DIR* d);
list getAllDirectoryEntries(DIR *d);

list getFilesWithExtension(DIR* d, string extension);
list getFoldersInDir(DIR* d);

string getSubDirectory(string basePath, string pathSeperator, string dirName);
list getAllDirectoryEntryNames(DIR *d);

list getFilesWithExtensionRecursive(DIR *d, string path, string pathSeperator, string extension);

#endif