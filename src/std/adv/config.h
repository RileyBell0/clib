#ifndef CLIB_STD_CONFIG_H
#define CLIB_STD_CONFIG_H

/*
 * Load everything into arrays
 * Save everything with its variable name
 */
#include "../path.h"
#include "../fileIO.h"
#include "sorting.h"
#include "../struc/array.h"
#include "../struc/vector.h"

#define CONFIG_COMMENT_CHAR '#'
#define CONFIG_STRING_ENCLOSE_CHAR '"'
#define CONFIG_FIELD_DECLARATION_CHAR '='
#define CONFIG_ESCAPE_CHAR '\\'
#define CONFIG_ARRAY_START_CHAR '['
#define CONFIG_ARRAY_END_CHAR ']'
#define CONFIG_IGNORE_CHAR ' '
#define CONFIG_NO_ENTRY "null"

#define CONFIG_STANDARD_DESCRIPTION                                            \
  "\
# Use '#' to define single line comments\n\
# \n\
# Declare variables as such:\n\
# \n\
# VariableName = Value \n\
# \n\
# or if you want to include spaces, surround in quotation marks:\n\
# \n\
# \"My Variable Name\" = \"My Variable\"\n\
# \n\
# To include Quotation marks within a variable or name, put \n\
# a backslash directly before them: \n\
# \n\
# Name = \"James \\\"Jimmy\\\" Jones\"\n\
# \n\
# If you wish to end a string with a backslash, go to the next line \n\
# without adding a final quotation mark \n\
# \n\
# If you wish to declare a variable with an array of associated values: \n\
# \n\
# myArray = [ \n\
#     \"First Value\"\n\
#     \"Second Value\"\n\
# \n\
#     ...\n\
# \n\
#     \"Final Value\"\n\
# \n\
# ]\n\
# \n\n"

#define CONFIG_FIELD_NONE (-1)
#define CONFIG_FIELD_NAME 0
#define CONFIG_FIELD_DECLARATION 1

typedef struct config_var_t {
  // Name associated with the variable
  string_t var_name;

  // Values associated with the variable
  array_t values;
} config_var_t;

typedef struct config_t {
  array_t variables;
  string_t path_to_cfg;

  bool modified;
} config_t;

config_var_t config_var_new(string_t *var_name);

/*
 * Returns a string containing the next valid field in the string 'str'
 * starting from position 'pos', storing the field in 'dest'.
 *
 * complexField will be set to true if the field read was enclosed by quotes
 */
bool extract_field(string_t *str, unsigned int *pos, string_t *dest,
                  bool *complexField);

/*
 * Loads all variables in a suitable config file into
 * an array
 */
config_t read_config_file(char *filePath);

int config_var_compare(const void *var1, const void *var2);

/*
 * Gets the requested varaible from the loaded config
 * prints an error message on fail
 */
config_var_t *config_get_var(config_t *config, char *name);

/*
 * Free's all dynamically allocated data in a Config
 */
void config_destroy(config_t config);

bool config_save(config_t config);
void config_encode(string_t *dest, string_t *toEncode);

/*
 * Prints all variables (and their contents) stored
 * within the given loaded config
 */
void config_print_vars(config_t config);

#endif