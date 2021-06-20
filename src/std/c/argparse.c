#include "../argparse.h"

argparser_t new_argparser(int argc, char **argv) {
  argparser_t parser;

  parser.num_args = argc - 1;
  parser.have_args_been_parsed = false;

  // Load in arguments
  parser.args = new_alist(sizeof(char *));
  parser.args.destroy = void_string_destroy;
  for (unsigned int arg = ARGPARSER_FIRST_ARG; arg < argc; arg++) {
    alist_append(&parser.args, &argv[arg]);
  }

  // Setup arg_names, ready to receive new names
  parser.arg_names = new_alist(sizeof(char *));
  parser.arg_names.destroy = void_string_destroy;

  parser.required_args = 0;

  return parser;
}

void argparse_add_arg(argparser_t *parser, char *arg_name, char *arg_help) {
  // TODO check that the argument name is not already in the
  // parser, -> variables should not have duplicate names

  parser->required_args += 1;
  alist_append(&parser->arg_names, arg_name);
  alist_append(&parser->arg_help, arg_help);
}

void argparse_parse_args(argparser_t *parser) {
  // Ensure enough valid arguments have been received
  if (parser->required_args < parser->num_args) {
    // unsigned int starting_arg = parser->num_args;

    fprintf(stderr, "Error - Not enough input arguments.\n");
    fprintf(stderr, "\tSource: std/c/argparse.c: argparse_parse_args\n");
    exit(EXIT_FAILURE);
  }
  parser->have_args_been_parsed = true;
}

char *argparse_get_arg(argparser_t *parser, char *name) {
  // Ensure arguments have been parsed
  if (!parser->have_args_been_parsed) {
    fprintf(stderr, "Error - Input arguments not parsed.\n");
    fprintf(stderr, "\tSource: std/c/argparse.c: argparse_get_arg\n");
    exit(EXIT_FAILURE);
  }

  // Find an argument with the matching name to 'name'
  int found_arg = false;
  alist_iterator_t it = new_alist_iterator(&parser->arg_names, true);
  for (char *arg_name = it.first(&it); !it.done(&it); arg_name = it.next(&it)) {
    if (strcmp(arg_name, name) == CSTR_EQUAL) {
      found_arg = true;
      break;
    }
  }

  // Return the argument if found
  if (found_arg) {
    return (char *)alist_get(&parser->args, it.index);
  } else {
    fprintf(stderr, "Error - Could not find provided argument.\n");
    fprintf(stderr, "Source: std/c/argparse.c: argparse_get_arg\n");
    exit(EXIT_FAILURE);
  }
}

void argparse_destroy(argparser_t *parser) {
  alist_destroy(&parser->arg_names);
  alist_destroy(&parser->args);
  alist_destroy(&parser->arg_help);
  parser->num_args = 0;
  parser->required_args = 0;
}