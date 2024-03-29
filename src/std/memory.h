#ifndef CLIB_STD_MEMORY_H
#define CLIB_STD_MEMORY_H
#define BITS_PER_BYTE 8

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * Wrapper for malloc which calls assert on the malloc'd data
 * and crashes the program if the malloc fails
 */
void *safe_malloc(size_t size);

/*
 * Wrapper for calloc which calls assert on the calloc'd data
 * and crashes the program if the malloc fails
 *
 * All data is initialised to zero
 */
void *safe_calloc(size_t size);

/*
 * Makes sure a realloc succeeds
 */
void *safe_realloc(void *ptr, size_t size);

/*
 * Wrapper for the 'free' function
 * which attempts to free the given pointer
 * if and only if the pointer is non-null
 */
void destroy(void *toDestroy);

/*
 * Treats the recieved pointer as a pointer to another pointer.
 *
 * Dereferences the recieved pointer, and if both the recieved pointer
 * and the dereferenced version of the recieved pointer are both non-null
 * the data pointed to is free'd
 */
void ptr_destroy(void *data);

/*
 * Offsets the given memory address by the given value
 */
void *offset(void *ptr, size_t num);

#endif