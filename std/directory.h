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
#include <sys/stat.h>
#include "path.h"
#include "string.h"
#include "list.h"
#include "fileIO.h"

typedef struct ordered_dirent_t 
{
    list_t block;
    list_t character;
    list_t directory;
    list_t fifo_pipe;
    list_t link;
    list_t regular;
    list_t socket;
    list_t unknown;
} ordered_dirent_t ;


struct dirent* ldirentnode(list_node_t *node)
{
    return (struct dirent*)node->data;
}

string_t *lstrnode(list_node_t *node)
{
    return (string_t *)node->data;
}
/*
 * returns a list of type (struct dirent*)
 * of all entries in the directory at the given path
 * 
 * destroy with list_destroy(list, null)
*/
list_t dir_all_entries_list(string_t path);

/*
 * Returns a list of type (struct dirent*) of all
 * entries of the given type in the directory of the
 * given path
 * 
 * destroy with list_destroy(list, null)
*/
list_t dir_all_entries_of_type(string_t path, unsigned char type);

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