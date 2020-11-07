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

/*
 * Config Structure
 * 
 * Config_t
 *      - Elements
 *      - Dat
 *          - elem 1
 *              - name
 *              - dat
 *                   - len
 *                   - dat
 *                          - "Value1"
 *                          - "Value2"
 *                          ...
 *                          - "ValueN"
 *          ...
 *          - elem n
 *      
*/
typedef struct config_var_t
{
    string_t varName;
    array_t data;
} config_var_t;

/*
 * Returns a string containing the next valid field in the string 'str'
 * starting from position 'pos', storing the field in 'dest'.
 * 
 * complexField will be set to true if the field read was enclosed by quotes
*/
int extract_field(string_t str, unsigned int *pos, string_t *dest, int *complexField);

/*
 * Loads all variables in a suitable config file into
 * an array
*/
array_t read_config_file(char *filePath);

/*
 * Free's all dynamically allocated data in a Config
*/
void config_destroy(array_t config);

#endif