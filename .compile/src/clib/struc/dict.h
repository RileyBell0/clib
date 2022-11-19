#ifndef CLIB_STD_STRUC_DICT_H
#define CLIB_STD_STRUC_DICT_H

#include "./vector.h"
#include "../memory.h"
#include "../string.h"
#include <stdlib.h>

typedef struct dict_node_t
{
  string_t key;
  void *value;
} dict_node_t;

typedef struct dict_t
{
  vector_t data;
  size_t value_size;
  void (*value_destroy)(void *data);
} dict_t;

/*
 * Creates a new dict, requires the element size of a value, element size of a
 * key, and a function to compare keys (with return '0' for equality)
 */
dict_t dict_new(size_t value_size, void (*value_destroy)(void *elem));

void *dict_get(dict_t *dict, char *key);

void dict_set(dict_t *dict, char *key, void *value);

bool dict_contains(dict_t *dict, char *key);

void dict_destroy(dict_t *dict);

#endif