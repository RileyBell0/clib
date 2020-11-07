#ifndef CLIB_CONFIG_LOADER
#define CLIB_CONFIG_LOADER

/*
 * Load everything into arrays
 * Save everything with its variable name
*/
#include "fileIO.h"
#include "string.h"
#include "array.h"

#define CONFIG_COMMENT_CHAR '#'
#define CONFIG_STRING_ENCLOSE_CHAR '"'
#define CONFIG_FIELD_DECLARATION_CHAR '='
#define CONFIG_ESCAPE_CHAR '\\'
#define CONFIG_ARRAY_START_CHAR '['
#define CONFIG_ARRAY_END_CHAR ']'
#define CONFIG_IGNORE_CHAR ' '

#define CONFIG_FIELD_NAME 0
#define CONFIG_FIELD_DECLARATION 1


typedef struct config_var_t
{
    string_t varName;
    array_t data;
} config_var_t;

typedef struct config_t
{
    unsigned int numEntries;
    config_var_t* entries;
} config_t;

/*
 * Lets read the file in one go
*/

int extract_field(string_t str, unsigned int *pos, string_t* dest);
array_t read_config_file(char *filePath);

#endif