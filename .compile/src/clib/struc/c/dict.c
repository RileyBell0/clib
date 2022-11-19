#include "../dict.h"

dict_node_t dict_node_new(dict_t *dict, char *key, void *value);

/*
 * Creates a new dict, requires the element size of a value, element size of a
 * key, and a function to compare keys (with return '0' for equality)
 */
dict_t dict_new(size_t value_size, void (*value_destroy)(void *elem))
{
  dict_t dict;

  dict.data = vector_new(sizeof(dict_node_t), NULL);
  dict.value_size = value_size;
  dict.value_destroy = value_destroy;

  return dict;
}

dict_node_t dict_node_new(dict_t *dict, char *key, void *value)
{
  dict_node_t node;

  node.key = string_new(key);
  node.value = safe_malloc(dict->value_size);
  memcpy(node.value, value, dict->value_size);

  return node;
}

static void *dict_vector_find(vector_t vec, char *key)
{
  for (size_t i = 0; i < vec.len; i++)
  {
    dict_node_t *node = (dict_node_t *)vector_get(&vec, i);
    if (strcmp(cstr(&node->key), key) == 0)
    {
      return node->value;
    }
  }

  return NULL;
}

void *dict_get(dict_t *dict, char *key)
{
  void *val = dict_vector_find(dict->data, key);
  if (!val)
  {
    exit_error("Invalid dict access -> key does not exist", "std/struc/c/dict.c", "dict_get");
  }
  return val;
}

void dict_set(dict_t *dict, char *key, void *value)
{
  void *val = dict_vector_find(dict->data, key);
  if (val)
  {
    // Replace the existing value
    if (dict->value_destroy)
    {
      dict->value_destroy(val);
    }
    memcpy(val, value, dict->value_size);
  }
  else
  {
    // Add a new entry to the dictionary
    dict_node_t node = dict_node_new(dict, key, value);
    vector_append(&dict->data, (void *)&node);
  }
}

bool dict_contains(dict_t *dict, char *key)
{
  if (dict_vector_find(dict->data, key))
  {
    return true;
  }
  return false;
}

static void dict_destroy_func(dict_node_t *node, void (*value_destroy)(void *value))
{
  string_destroy(&node->key);
  if (value_destroy)
  {
    value_destroy(node->value);
  }
  free(node->value);
}

void dict_destroy(dict_t *dict)
{
  for (size_t i = 0; i < dict->data.len; i++)
  {
    dict_destroy_func(vector_get(&dict->data, i), dict->value_destroy);
  }
  vector_destroy(&dict->data);
}