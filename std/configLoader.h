#ifndef CLIB_CONFIG_LOADER
#define CLIB_CONFIG_LOADER

/*
 * Load everything into arrays
 * Save everything with its variable name
*/
#include "fileIO.h"
#include "string.h"
#include "array.h"
#include "avlBinTree.h"
#include "sorting.h"

#define CONFIG_COMMENT_CHAR '#'
#define CONFIG_STRING_ENCLOSE_CHAR '"'
#define CONFIG_FIELD_DECLARATION_CHAR '='
#define CONFIG_ESCAPE_CHAR '\\'
#define CONFIG_ARRAY_START_CHAR '['
#define CONFIG_ARRAY_END_CHAR ']'
#define CONFIG_IGNORE_CHAR ' '
#define CONFIG_NO_ENTRY "NULL"

#define CONFIG_FIELD_NONE (-1)
#define CONFIG_FIELD_NAME 0
#define CONFIG_FIELD_DECLARATION 1

/*
 * Config Structure
 * 
 * Config_t
 *      - len
 *      - vars
 *          - elem 1
 *              - varName
 *              - len
 *              - data
 *                   - len
 *                   - str
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
    string_t* data;
    unsigned int len;
} config_var_t;

typedef struct config_t
{
    unsigned int len;
    config_var_t* vars;
} config_t;

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
config_t read_config_file(char *filePath);

int config_var_compare(const void* var1, const void* var2);

/*
 * Gets the requested varaible from the loaded config
*/
config_var_t *config_get_var(config_t* config, char* name);

/*
 * Free's all dynamically allocated data in a Config
*/
void config_destroy(config_t config);

/*
 * Prints all variables (and their contents) stored
 * within the given loaded config
*/
void config_print_vars(config_t config);

#endif