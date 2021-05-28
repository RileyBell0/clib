#include "../argparse.h"

argparser_t new_argparser(int argc, char **argv) {
  argparser_t parser;

  parser.num_args = argc - 1;

  // Load in arguments
  parser.args = new_alist(sizeof(string_t));
  parser.args.destroy = void_string_destroy;
  for (unsigned int arg = ARGPARSER_FIRST_ARG; arg < argc; arg++) {
    string_t argument = string_make(argv[arg]);
    alist_append(&parser.args, &argument);
  }

  // Setup arg_names, ready to receive new names
  parser.arg_names = new_alist(sizeof(string_t));
  parser.arg_names.destroy = void_string_destroy;

  parser.required_args = 0;

  return parser
}

void argparse_add_arg(argparser_t *parser, string_t *arg_name) {
  parser->required_args += 1;
  alist_append(&parser->arg_names, arg_name);
}

string_t argparse_get_arg(argparser_t *parser, string_t *arg_name) {}