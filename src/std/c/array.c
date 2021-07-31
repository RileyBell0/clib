#include "../array.h"

//////////////////////////////
// Initialisation
//////////////////////////////

array_t array_new(int elements, size_t element_size) {
  array_t array;

  if (elements < 0) {
    exit_error("Cannot initalise an array with a negative number of elements",
               "std/c/array.c", "array_new");
  }

  // Allocate memory for the array
  if (elements != 0) {
    array.data = safe_malloc(elements * element_size);
  } else {
    array.data = NULL;
  }

  array.len = elements;
  array.element_size = element_size;
  array.allocated = true;
  array.destroy_on_remove = true;
  array.destroy = NULL;

  return array;
}

//////////////////////////////
// Basic Operations
//////////////////////////////

void *array_get(array_t *array, int index) {
  index = index_convert_negative_safe(array->len, index);
  return array_generic_get(array->data, index, array->element_size);
}

void array_set(array_t *array, int index, void *data) {
  index = index_convert_negative_safe(array->len, index);
  assert(memcpy(array_get(array, index), data, array->element_size));
}

array_t array_wrap(void *array, int len, size_t element_size, 
                   bool was_allocated) {
  array_t wrapped;
  
  if (len < 0) {
    exit_error("Cannot initalise an array with a negative number of elements",
               "std/c/array.c", "array_wrap");
  }
  
  wrapped.data = array;
  wrapped.len = len;
  wrapped.element_size = element_size;
  wrapped.allocated = was_allocated;

  return wrapped;
}

//////////////////////////////
// High Level Functions
//////////////////////////////

void array_resize(array_t *array, int new_len, void* template) {
  if (new_len < 0) {
    exit_error("Error - Cannot resize an array to a negative value",
               "std/c/array.c", "array_resize");
  }

  if (array->len == new_len) {
    return;
  }
  else if (new_len == 0) {
    // Empty the array
    if (array->allocated) {
      destroy(array->data);
    }
    array->len = 0;
    array->data = NULL;

    return;
  }

  // Allocate space for the array
  void* new_mem;
  if (template) {
    new_mem = safe_calloc(array->element_size * new_len);
  }
  else {
    new_mem = safe_malloc(array->element_size * new_len);
  }

  // Determine how many elements to copy from the original list
  int elems_to_copy = array->len;
  if (new_len < array->len) {
    elems_to_copy = new_len;
  }

  // Copy in the new data
  if (elems_to_copy > 0) {
    assert(memcpy(new_mem, array->data, array->element_size * new_len));
  }

  // Write the 'template' to each new empty index
  if (template) {
    for (int index = array->len; index < new_len; index++) {
      void* elem = array_generic_get(array->data, index, array->element_size);
      assert(memcpy(elem, template, array->element_size));
    }
  }

  // Destroy the old memory and point to the new memory
  if (array->allocated) {
    destroy(array->data);
  }
  array->data = new_mem;
  array->allocated = true;
  array->len = new_len;
}

// // TODO implement this like in python
// array_t array_slice(array_t *arr, char* format) {
// }

//////////////////////////////
// Generic Arrays
//////////////////////////////

void *array_generic_get(void *array, unsigned int element,
                        size_t element_size) {
  return (void *)((char *)array + (element * element_size));
}

void array_generic_set(void *array, void *data, unsigned int element,
                       size_t element_size) {
  memcpy(((char *)array + (element_size * element)), data, element_size);
}

//////////////////////////////
// Cleanup
//////////////////////////////


// Wrapper function to destroy the contents of an array
void array_destroy(array_t *arr) {
  // Destroy elements if enabled
  if (arr->destroy_on_remove && arr->destroy) {
    for (int i = 0; i < arr->len; i++) {
      void* elem = array_get(arr, i);
      arr->destroy(elem);
    }
  }
  
  // Destroy allocated array space
  if (arr->allocated) {
    destroy(arr->data);
  }

  arr->allocated = true;
  arr->data = NULL;
  arr->len = 0;
}