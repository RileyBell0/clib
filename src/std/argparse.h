// #ifndef CLIB_STC_ARGPARSE_H
// #define CLIB_STC_ARGPARSE_H

// #include "avl_tree.h"
// #include "error.h"
// #include "queue.h"
// #include "string.h"
// #include <stdio.h>

// #define ARGPARSER_FIRST_ARG 1
// #define ARGPARSER_PROC_NAME_ARG 0

// /*
//  * TODO
//  * argparser should make sure
//  * that when you're adding new arguments
//  * that no two arguments have the same name
//  * - This is doable with the tree
//  * This should be a check done during add_arg or arg_parse
//  * This can be done really easily if we have an atree
//  *
//  * TODO make argparse use an atree (yet to code) 03/06/2021
//  * and have it throw a fit if any element is already present when
//  * inserting it into the tree
//  */

// typedef struct argparse_arg_info_t {
//   string_t arg;
//   string_t arg_name;
//   string_t arg_help;
// } argparse_arg_info_t;

// // TODO put the
// typedef struct argparser_t {
//   bool enough_args;
//   int required_args;
//   int argc;
//   int args_received;
//   char **argv;
//   string_t proc_name;

//   avl_tree_t args;
//   queue_t arg_values;
//   queue_t arg_names;
//   queue_t arg_help;
// } argparser_t;

// //////////////////////////////
// // Initialisation
// //////////////////////////////

// argparser_t argparser_new(int argc, char **argv);

// //////////////////////////////
// // Basic Operations
// //////////////////////////////

// void argparse_add(argparser_t *parser, char *arg_name, char *arg_help);

// //////////////////////////////
// // Finalization
// //////////////////////////////

// void argparse_parse(argparser_t *parser);

// //////////////////////////////
// // Cleanup
// //////////////////////////////

// void argparse_destroy(argparser_t *parser);

// #endif