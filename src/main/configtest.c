#include <stdio.h>
#include "../std/adv/config.h"
#include "../std/adv/argparse.h"

int main(int argc, char** argv) {
  vector_t args = argparse_vector(argc, argv);

  char* file_name = cstr((string_t*)vector_get(&args, 1));

  config_t cfg = config_read(file_name);
  
  for (int i = 0; i < cfg.variables.len; i++) {
    config_var_t var = *(config_var_t*)array_get(&cfg.variables, i);
    printf("\"%s\"\n", cstr(&var.var_name));
    for (int j = 0; j < var.values.len; j++) {
      printf("\t\"%s\"\n", cstr((string_t*)array_get(&var.values, j)));
    }
  }

}
