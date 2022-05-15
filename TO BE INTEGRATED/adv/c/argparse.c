#include "../argparse.h"

vector_t argparse_process(int argc, char **argv)
{
  // Setup vector to store all arguments as strings
  vector_t args = vector_new(sizeof(string_t));
  args.destroy = void_string_destroy;
  vector_set_capacity(&args, argc - FIRST_ARG);

  // Convert all args to string_t and store in args
  for (int i = FIRST_ARG; i < argc; i++)
  {
    string_t arg = string_make(argv[i]);
    vector_append(&args, &arg);
  }

  return args;
}


// Does not allow flags to have multiple values
dict_t argparse_handle_flags(int argc, char **argv) {
  dict_t dict = dict_new(sizeof(string_t), sizeof(string_t),
                       string_void_compare, void_string_destroy, 
                       void_string_destroy);
  void* last_key = NULL;

  vector_t arguments = argparse_process(argc, argv);
  arguments.destroy = NULL;

  string_t empty_value = empty_string();

  // For all args
  for (int i = 0; i < arguments.len; i++) {
    string_t* elem = vector_get(&arguments, i);
    char first_char = '\0';
    
    if (elem->len > 0) {
      first_char = cstr(elem)[0];
    }

    if (first_char == ARG_FLAG_IDENTIFIER) {
      if (last_key) {
        dict_insert(&dict, last_key, &empty_value);
      }
      // Load a new flag into the dict if it does not already exist in the dict
      last_key = elem;
    }
    else {
      if (last_key) {
        dict_insert(&dict, last_key, elem);
        last_key = NULL;
      }
      else {
        // If this value does not follow a flag delaration, make this its
        // own sort of argument
        dict_insert(&dict, elem, &empty_value);
      }
    }
  }

  // Flush the last element into the dict
  if (last_key) {
    dict_insert(&dict, last_key, &empty_value);
  }
  last_key = NULL;

  vector_destroy(&arguments);

  return dict;
}