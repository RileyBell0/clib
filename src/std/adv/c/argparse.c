#include "../argparse.h"

vector_t argparse_vector(int argc, char** argv) {
  vector_t args = vector_new(sizeof(string_t));
  args.destroy = void_string_destroy;
  args.destroy_on_remove = true;

  for (int i = 0; i < argc; i++) {
    string_t arg = string_make(argv[i]);
    vector_append(&args, &arg);
  }

  return args;
}