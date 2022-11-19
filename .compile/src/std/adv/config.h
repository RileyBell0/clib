#ifndef CLIB_STD_ADV_CONFIG_H
#define CLIB_STD_ADV_CONFIG_H

#include "../string.h"
#include "../fileIO.h"
#include "../struc/dict.h"
#include <stdbool.h>

#define CONFIG_COMMENT '#'
#define CONFIG_ESCAPE '\\'
#define CONFIG_STRING '"'
#define CONFIG_EQUALS '='

#define CONFIG_LOOKING_FOR_KEY 1
#define CONFIG_LOOKING_FOR_DECLARATION 2
#define CONFIG_LOOKING_FOR_VALUE 3
#define CONFIG_LOOKING_FOR_VALUE_ARRAY 4

/*
 * Returns a mapping of string_t to array_t[string_t] of key to value(s)
 */
dict_t config_read(char *path);

#endif