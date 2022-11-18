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

dict_t config_read(char *path);

#endif