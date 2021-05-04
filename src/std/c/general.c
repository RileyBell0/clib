#include "../general.h"

void *safe_malloc(size_t size) {
  void *newData = malloc(size);
  assert(newData);
  return newData;
}

void *safe_calloc(size_t size) {
  void *newData = calloc(1, size);
  assert(newData);
  return newData;
}

void *safe_realloc(void *ptr, size_t size) {
  void *result = realloc(ptr, size);
  assert(result);
  return result;
}

void destroy(void *toDestroy) {
  if (toDestroy) {
    free(toDestroy);
  }
}

void ptr_destroy(void *data) {
  if (data) {
    void **data2 = (void **)data;
    destroy(*data2);
  }
}

void *offset(void *ptr, unsigned int num) { return (((char *)ptr) + num); }
