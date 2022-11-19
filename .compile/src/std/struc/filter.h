#ifndef CLIB_STD_STRUC_FILTER_H
#define CLIB_STD_STRUC_FILTER_H

#include "./array.h"
#include "./vector.h"

/*
 * Removes all elements where the filter function returns false.
 * Extra is any extra data you want to pass to the filter function. Can be NULL
 */
array_t *array_filter(array_t *arr, bool (*filter)(void *data, void *extra), void *extra);

#endif