#include "alist.h"
#include "string.h"
#include <stdio.h>

#define ARGPARSER_FIRST_ARG 1

/*
 * TODO
 * argparser should make sure
 * that when you're adding new arguments
 * that no two arguments have the same name
 * This should be a check done during add_arg or arg_parse
 * This can be done really easily if we have an atree
 *
 * TODO make argparse use an atree (yet to code) 03/06/2021
 * and have it throw a fit if any element is already present when
 * inserting it into the tree
 */

typedef struct argparser_t {
  int num_args;
  int have_args_been_parsed;
  alist_t args;
  alist_t arg_names;
  alist_t arg_help;
  char *proc_name;
  unsigned int required_args;
} argparser_t;

argparser_t new_argparser(int argc, char **argv);