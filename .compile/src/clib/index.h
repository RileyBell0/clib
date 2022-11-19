#ifndef CLIB_STD_INDEX_H
#define CLIB_STD_INDEX_H

#include <stdbool.h>

/*
 * returns true if the given index is closer to the start than the end of the
 * list
*/
bool index_closer_to_start(int len, int index);

#endif