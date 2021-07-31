#include "../argparse.h"

int argparse_compare(const void* input1, const void* input2);

//////////////////////////////
// Initialisation
//////////////////////////////

argparser_t argparser_new(int argc, char **argv) {
  argparser_t parser;

  parser.enough_args = true;
  parser.argc = argc;
  parser.argv = argv;
  parser.args_received = argc - 1;
  parser.proc_name = string_make(argv[ARGPARSER_PROC_NAME_ARG]);
  parser.arg_values = queue_new(sizeof(string_t), parser.args_received);
  parser.arg_names = queue_new(sizeof(string_t), parser.args_received);
  parser.arg_help = queue_new(sizeof(string_t), parser.args_received);
  parser.arg_values.destroy = void_string_destroy;
  parser.arg_names.destroy = void_string_destroy;
  parser.arg_help.destroy = void_string_destroy;
  parser.required_args = 0;

  parser.args = avl_tree_new(sizeof(argparse_arg_info_t), argparse_compare);
  
  // Read in all input args into the queue 'args'
  for (int i = ARGPARSER_FIRST_ARG; i < parser.args_received; i++) {
    string_t arg = string_make(argv[i]);
    queue_enqueue(&parser.arg_values, &arg);
  }

  return parser;
}

//////////////////////////////
// Basic Operations
//////////////////////////////

void argparse_add(argparser_t *parser, char *arg_name, char *arg_help) {
  string_t name = string_make(arg_name);
  string_t help = string_make(arg_help);

  queue_enqueue(&parser->arg_names, &name);
  queue_enqueue(&parser->arg_help, &help);

  parser->required_args += 1;
}

string_t* argparse_get_arg(argparser_t *parser, char *name) {
  // Ensure arguments have been parsed
  if (!parser->enough_args) {
    exit_error("Not enough input arguments received", "std/c/argparse.c",
               "argparse_get_arg");
  }
  
  // TODO write this function

  // Find an argument with the matching name to 'name'
  // TODO this would be faster with a tree, which means we should make a package
  // deal of three strings, the arg itself, the argname and the arghelp
  // then we'll throw a fit while they're trying to add the args themselves
  return NULL;
}

//////////////////////////////
// Finalization
//////////////////////////////

void argparse_parse_args(argparser_t *parser) {
  if (parser->args_received < parser->required_args) {
    exit_error("Not enough input arguments", "std/c/argparse.c",
               "argparse_parse_args");
  }

  int args_read = 0;
  while (args_read < parser->required_args) {
    argparse_arg_info_t info;

    queue_dequeue(&parser->arg_values, &info.arg);
    queue_dequeue(&parser->arg_names, &info.arg_name);
    queue_dequeue(&parser->arg_help, &info.arg_help);

    avl_tree_insert(&parser->args, (void*)&info);

    ++args_read;
  }
}

//////////////////////////////
// Utility
//////////////////////////////

/*
 * Returns the compare value of the names of the two recieved arguments
*/
int argparse_compare(const void* input1, const void* input2) {
  argparse_arg_info_t *arg1 = (argparse_arg_info_t *)input1;
  argparse_arg_info_t *arg2 = (argparse_arg_info_t *)input2;

  // compare based on argument name
  return strcmp(cstr(&arg1->arg_name), cstr(&arg2->arg_name));
}

void argparse_destroy(argparser_t *parser) {
  // TODO write function
}