/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

/*
 * For dealing with directories
 * 
 * Verified to (probably) work with Linux file systems
 * I mean, i havent really done extensive testing on this
 * code but it's worked with what ive needed it for
*/

#ifndef CLIB_STD_DIRECTORY_H
#define CLIB_STD_DIRECTORY_H

#include <dirent.h>

#include "path.h"
#include "string.h"
#include "list.h"
#include "fileIO.h"

typedef struct dirent_info_t {
    string_t d_name;
    ino_t d_ino;
    mode_t d_namlen;
    mode_t d_reclen;
    ino_t d_seekoff;
    unsigned char d_type;
} dirent_info_t;

/*
 * Returns a pointer to the start of the file extension
 * within the given string.
 * 
 * Returns NULL on empty string
*/
char *getFileExtension(char *fileName);

/*
 * Copies a dirent struct from the stack to the heap
 * Also converts d_name to a string_t from a char*
*/
dirent_info_t dirent_copy(struct dirent *d);

/*
 * Modifies the given string, removing
 * the extension at the end of the string
 * if there is an extension
 * 
 * DOES NOT ALLOCATE MEMORY
*/
char *removeFileExtension(char *fileName);

/*
 * returns a list of struct dirent containing
 * all directory entries of type file in the given
 * directory
 * 
 * Ensure the list is destroyed with list_destroy(&list, NULL)
 * when you're done with it
*/
list_t getAllFiles(DIR *d);


/*
 * Returns a list of type (struct dirent*)
 * 
 * destroy with list_destroy(list, null)
*/
list_t dir_all_entries_list(DIR *d);

/*
 * returns a list of struct dirent containing
 * all directory entries of type file in the given
 * directory where the file has the extension 'extension'
 * 
 * If extension is a string of length 0, all files without
 * extensions are returned
 * 
 * Ensure the list is destroyed with list_destroy(&list, NULL)
 * when you're done with it
*/
list_t getFilesWithExtension(DIR *d, string_t extension);

/*
 * returns a list of struct dirent containing
 * all directory entries of type directory in the given
 * directory
 * 
 * Ensure the list is destroyed with list_destroy(&list, NULL)
 * when you're done with it
*/
list_t getFoldersInDir(DIR *d);

/*
 * Returns a *new* string which is the concatenation
 * of basePath + pathSeperator + dirName
 * 
 * More efficient than doing each concatenation seperately as 
 * all required space is allocated at once
*/
string_t getSubDirectory(string_t basePath, string_t pathSeperator, string_t dirName);

/*
 * returns a list of c strings (char*) containing
 * the names of all directory entries in the given
 * directory
 * 
 * Ensure the list is destroyed with list_destroy(&list, ptr_destroy)
 * when you're done with it
*/
list_t getAllDirectoryEntryNames(DIR *d);

/*
 * returns a list of string_t structs containing
 * the paths with the name of all files in the 
 * given directory and all of its sub-directories with the
 * specified extension.
 * 
 * Ensure the list is destroyed with list_destroy(&list, void_string_destroy)
 * when you're done with it
*/
list_t getFilesWithExtensionRecursive(DIR *d, string_t path, string_t pathSeperator, string_t extension);

#endif