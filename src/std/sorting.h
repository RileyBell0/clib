#ifndef CLIB_SORT_H
#define CLIB_SORT_H

#include "avlBinTree.h"

array_t tree_sort(array_t data, unsigned int elementSize,
                  int (*compareFunc)(const void *first, const void *second));

#endif