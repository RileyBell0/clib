#include "../filter.h"

array_t *array_filter(array_t *arr, bool (*filter)(void *data, void *extra), void *extra)
{
  // Filter down to files with the matching extension
  vector_t res = vector_new(arr->element_size, NULL);
  for (size_t i = 0; i < arr->len; i++)
  {
    void *elem = array_get(arr, i);
    if (filter(elem, extra))
    {
      vector_append(&res, elem);
    }
    else
    {
      if (arr->destroy)
      {
        arr->destroy((void *)elem);
      }
    }
  }

  // Clear old array data
  void (*destroy)(void *elem) = arr->destroy;
  arr->destroy = NULL;
  array_destroy(arr);

  // Copy filtered data in
  *arr = vector_to_array(&res);
  arr->destroy = destroy;
  vector_destroy(&res);

  return arr;
}