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

unsigned char get_file_type(char *path);

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

ordered_dirent_t new_ordered_dirent_t();

void ordered_dirent_insert(ordered_dirent_t *ordered, struct dirent *entry);

void ordered_dirent_destroy(ordered_dirent_t* ordered);

ordered_dirent_t dir_all_entries_categorised(string_t path);

list_t dir_files_with_extension_recur(string_t path, string_t extension);

struct dirent* ldirentnode(list_node_t *node);

string_t *lstrnode(list_node_t *node);

#endif