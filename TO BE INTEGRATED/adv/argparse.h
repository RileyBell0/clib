#ifndef CLIB_STD_ADV_ARGPARSE_H
#define CLIB_STD_ADV_ARGPARSE_H

#include "../string.h"
#include "../struc/tree_dict.h"

#define FIRST_ARG 1
/* Here, flags are defined as a '-' followed by any number of characters */
#define MIN_ARG_FLAG_LEN 2
#define ARG_FLAG_IDENTIFIER '-'

// A structure representing an identifier-value argument pair
typedef struct args_t {
  dict_t dict;
  void* last_key;
} args_t;

// Converts command line arguments into a vector of string_t
vector_t argparse_process(int argc, char **argv);

// TODO write desc
dict_t argparse_handle_flags(int argc, char **argv);

// Destroys dynamically allocated memory in an arg_var_t
void argparse_destroy_arg_var(void *arg_var);


#endif