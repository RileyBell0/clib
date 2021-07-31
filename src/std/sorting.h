#ifndef CLIB_SORTING_H
#define CLIB_SORTING_H

#include "avlBinTree.h"

array_t tree_sort(array_t data, unsigned int elementSize,
                  int (*compareFunc)(const void *first, const void *second));

#endif