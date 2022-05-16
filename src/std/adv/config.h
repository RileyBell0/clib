#ifndef CLIB_STD_CONFIG_H
#define CLIB_STD_CONFIG_H

/*
 * Load everything into arrays
 * Save everything with its variable name
 */
#include "../path.h"
#include "../fileIO.h"
#include "../struc/array.h"
#include "../struc/vector.h"

#define CONFIG_COMMENT_CHAR '#'
#define CONFIG_FIELD_ENCLOSE_CHAR '"'
#define CONFIG_FIELD_DECLARATION_CHAR '='
#define CONFIG_ESCAPED_NEW_LINE 'n'
#define CONFIG_ESCAPED_CARRIAGE_RETURN 'r'
#define CONFIG_ESCAPE_CHAR '\\'

#define CONFIG_PADDING_CHARS " \n\r"
#define CONFIG_ARRAY_PADDING_CHARS " \n\r,"

// TODO determine which ascii characters should be ignored
#define CONFIG_FIELD_IGNORE_CHARS "\r"

#define CONFIG_ARRAY_START_CHAR '['
#define CONFIG_ARRAY_END_CHAR ']'
#define CONFIG_NO_ENTRY "null"

#define CONFIG_STANDARD_DESCRIPTION                                            \
  "\
# Use '#' to define single line comments\n\
# \n\
# Declare variables as such:\n\
# \n\
# \"VariableName\" = \"Value\" \n\
# \n\
# To include Quotation marks within a variable or name, put \n\
# a backslash directly before them: \n\
# \n\
# \"Name\" = \"Dwayne \\\"The Rock\\\" Johnson\"\n\
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
# \n\
# Or\n\
# \"Vars\" = [\"Var1\" \"Var2\" \"Var3\"]\n\
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

config_t config_read(char *file_path);

#endif