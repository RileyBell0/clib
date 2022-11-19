#ifndef CLIB_STD_DIRECTORY_H
#define CLIB_STD_DIRECTORY_H

#define CLIB_STD_DIRECTORY_H_DIRENT_H_INCLUDED

#ifdef CLIB_STD_DIRECTORY_H_DIRENT_H_INCLUDED

#include "./system.h"
#include "../string.h"
#include "../struc/vector.h"
#include "../struc/array.h"
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

/*
 * Returns an array of type (struct dirent) of all entries within the
 * given directory 'path'
 * Memory is allocated
 */
array_t dir_all_entries(char *path);

/*
 * Returns an array of type (string_t) containing the path to all files within
 * the given path
 */
array_t dir_all_files_recur(char *path);

bool dir_entry_is_reg_file(char *entry_name);

#endif
#endif
