#include "../error.h"

void exit_error(char *message, char *file, char *function) {
  // Print error messages
  fprintf(stderr, "Error: %s\n", message);

  fprintf(stderr, "Source:\n");
  fprintf(stderr, "\tFile: %s\n", file);
  fprintf(stderr, "\tFunction: %s\n", function);

  exit(EXIT_FAILURE);
}