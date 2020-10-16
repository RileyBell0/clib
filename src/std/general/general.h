#ifndef CLIB_GENERAL_H
#define CLIB_GENERAL_H

#include <stdlib.h>
#include <assert.h>
#include "../bool/bool.h"

void *safe_malloc(unsigned int size);

void *safe_calloc(unsigned int size);

void destroy(void *toDestroy);

#endif