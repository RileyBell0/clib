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
 * 
 * im reworking all of this
*/

#ifndef CLIB_STD_DIRECTORY_H
#define CLIB_STD_DIRECTORY_H

#include <dirent.h>
#include <sys/stat.h>
#include "alist.h"
#include "fileIO.h"

typedef struct categorised_dirent_t 
{
    alist_t block;
    alist_t character;
    alist_t directory;
    alist_t fifo_pipe;
    alist_t link;
    alist_t regular;
    alist_t socket;
    alist_t unknown;
} categorised_dirent_t ;

unsigned char get_file_type(char *path);

/*
 * returns a list of type (struct dirent*)
 * of all entries in the directory at the given path
 * 
 * destroy with list_destroy(list, null)
*/
alist_t dir_all_entries_list(string_t* path);

/*
 * Returns a list of type (struct dirent*) of all
 * entries of the given type in the directory of the
 * given path
 * 
 * destroy with list_destroy(list, null)
*/
alist_t dir_all_entries_of_type(string_t* path, unsigned char type);

categorised_dirent_t new_categorised_dirent_t();

void categorised_dirent_insert(categorised_dirent_t *ordered, struct dirent *entry);

void categorised_dirent_destroy(categorised_dirent_t* ordered);

categorised_dirent_t dir_all_entries_categorised(string_t* path);

alist_t dir_files_with_extension_recur(string_t* path, string_t* extension);

int is_normal_dir(string_t* dirName);

struct dirent* ldirentnode(list_node_t *node);

string_t *lstrnode(list_node_t *node);

#endif