#include "../array.h"

//////////////////////////////
// Initialisation
//////////////////////////////

array_t array_new(size_t len, size_t element_size, void (*destroy)(void *elem))
{
  if (len <= 0)
  {
    exit_error("Array must be initialised with a size of at least 1",
               "std/c/array.c", "array_new");
  }

  array_t array;

  array.data = safe_malloc(len * element_size);
  array.len = len;
  array.element_size = element_size;
  array.destroy = destroy;

  return array;
}

//////////////////////////////
// Basic Operations
//////////////////////////////

void *array_get(array_t *array, size_t index)
{
  return array_generic_get(array->data, index, array->element_size);
}

void array_set(array_t *array, size_t index, void *data)
{
  assert(memcpy(array_get(array, index), data, array->element_size));
}

//////////////////////////////
// Generic Arrays
//////////////////////////////

void *array_generic_get(void *array, size_t index,
                        size_t element_size)
{
  return (void *)((char *)array + (index * element_size));
}

void array_generic_set(void *array, void *elem, size_t index,
                       size_t element_size)
{
  memcpy(((char *)array + (element_size * index)), elem, element_size);
}

//////////////////////////////
// Cleanup
//////////////////////////////

// Wrapper function to destroy the contents of an array
void array_destroy(array_t *arr)
{
  // Destroy elements if enabled
  if (arr->destroy)
  {
    for (size_t i = 0; i < arr->len; i++)
    {
      void *elem = array_get(arr, i);
      arr->destroy(elem);
    }
  }

  destroy(arr->data);

  arr->data = NULL;
  arr->len = 0;
}