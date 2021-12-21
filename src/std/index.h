#ifndef CLIB_STD_INDEX_H
#define CLIB_STD_INDEX_H

#include "error.h"
#include <stdbool.h>
#include <stdlib.h>

/*
 * Given the length of an iterable and an index within it, converts a negative
 * index into a positive index and returns the value
 *
 * Exits with EXIT_FAILURE if the index is invalid for the size of the iterable
 */
int index_convert_negative_safe(int len, int index);

/*
 * Given the length of an iterable and an index within it, adds len to index to
 * try and convert a negative into a positive index and returns the value
*/
int index_convert_negative(int len, int index);

/*
 * Given an index, and the size of an iterable, returns true if the index is
 * within the range of an iterable, false otherwise
 *
 * Does not support negative indicies
 */
bool index_is_valid(int len, int index);

/*
 * returns true if the given index is closer to the start than the end of the
 * list
*/
bool index_closer_to_start(int len, int index);

/*
 * Given an index, and the length of the object it is to be contained in
 * brings the index to the closest bound if outside the bounds of the object,
 * else, returns the index
*/
int index_constrain(int len, int index);

#endif