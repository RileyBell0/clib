#ifndef CLIB_STD_ADV_ARGPARSE_H
#define CLIB_STD_ADV_ARGPARSE_H

#include "../struc/vector.h"
#include "../string.h"

//TODO come up with use for this file

/*
 * Converts input arguments into strings, and saves them into a vector
 * in the same order they were recieved
*/
vector_t argparse_vector(int argc, char** argv);

#endif