/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

#ifndef CLIB_STD_GENERAL_H
#define CLIB_STD_GENERAL_H

#include <stdlib.h>
#include <assert.h>
#include "bool.h"

void *safe_malloc(unsigned int size);

void *safe_calloc(unsigned int size);

void destroy(void *toDestroy);

void ptr_destroy(void *data);

#endif