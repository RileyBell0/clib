#include "alist.h"

#define ARGPARSER_FIRST_ARG 1

typedef struct argparser_t {
    int num_args;
    alist_t args;
    alist_t arg_names;
    unsigned int required_args;
} argparser_t;

argparser_t new_argparser(int argc, char** argv);