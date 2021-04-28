#include "../array.h"

static int dynamic_array_extend(dynamic_array_t *array);

void array_set_element(void *array, void *data, uint32_t element,
                       size_t element_size) {
  memcpy(((char *)array + (element_size * element)), data, element_size);
}

void *array_get_element(void *array, uint32_t element, size_t element_size) {
  return (void *)((char *)array + (element * element_size));
}

void *array_index(array_t *array, uint32_t index) {
  return array_get_element(array->data, index, array->element_size);
}

#include <stdio.h>
array_t new_array(uint32_t elements, size_t element_size) {
  array_t array;

  if (elements != 0) {
    array.data = safe_malloc(elements * element_size);
  } else {
    array.data = NULL;
  }

  array.len = elements;
  array.element_size = element_size;

  return array;
}

array_t array_wrap(void* array, size_t element_size, uint32_t len)
{
  array_t wrapped;
  wrapped.data = array;
  wrapped.len = len;
  wrapped.element_size = element_size;
  return wrapped;
}

int array_resize(array_t *array, uint32_t new_length) {
  // cannot resize to be smaller
  if (array == NULL || array->len >= new_length) {
    return FALSE;
  }

  // Resize
  array->data = realloc(array->data, array->element_size * new_length);
  array->len = new_length;

  // Assert the resizing worked
  assert(array->data);

  return TRUE;
}

dynamic_array_t new_dynamic_array(size_t element_size) {
  dynamic_array_t array = {0};

  array.element_size = element_size;

  return array;
}

int dynamic_array_append(dynamic_array_t *array, void *element) {
  // Extending the array if needed
  if (array->capacity == array->len) {
    dynamic_array_extend(array);
  }

  // Copies data from the address 'element' to the array
  memcpy(&((char *)array->data)[(array->len * array->element_size)], element,
         array->element_size);

  array->len += 1;

  return TRUE;
}

// Extends an array (if it is full)
static int dynamic_array_extend(dynamic_array_t *array) {
  if (array == NULL || array->len < array->capacity) {
    return FALSE;
  }
  uint32_t new_len;

  // Add one to the length if its less than 3 (after which point dividing makes
  // sense)
  if (array->capacity <= 3) {
    new_len = (array->capacity + 1);
  } else {
    // 1.5x extension factor (capacity + capacity/2)
    new_len = array->capacity + (array->capacity / 2);
  }

  // Extend the array
  array->data = safe_realloc(array->data, new_len * array->element_size);

  array->capacity = new_len;

  return TRUE;
}

int dynamic_array_safe_resize(dynamic_array_t *array, uint32_t new_len) {
  // If less than the current array length - FAIL
  if (new_len <= array->len) {
    return FALSE;
  }

  // If greater than the current array length but less than or equal
  // to the current maximum length of the array
  if (new_len <= array->capacity) {
    return TRUE;
  }

  // Resize the array
  array->data = realloc(array->data, array->element_size * new_len);
  assert(array->data);

  // Update the length of the array
  array->len = new_len;
  array->capacity = new_len;

  return TRUE;
}

void dynamic_array_set_element(dynamic_array_t *array, uint32_t element,
                               void *data) {
  // Gets a pointer to the start of the required element
  if (array->capacity < element) {
    // If the length of the array when extended is less than
    // the required length to place the new element at
    if (element > array->capacity + (array->capacity / 2)) {
      dynamic_array_safe_resize(array, element);
    } else {
      dynamic_array_extend(array);
    }
  }

  // Update the length of the array (if the element was placed past the current
  // length)
  if (element > array->len) {
    array->len = element;
  }

  // Set the element in the array at the given position to the data recieved
  memcpy((((char *)array->data) + (array->element_size * element)), data,
         array->element_size);
}

void array_set_index(array_t *array, uint32_t index, void* data) {
  assert(memcpy(array_index(array, index), data, array->element_size));
}

void *dynamic_array_get_element(dynamic_array_t *array, uint32_t element) {
  // If the required element is outside the bounds of the array
  if (element > array->len) {
    return NULL;
  }

  // Return a pointer to the start of the element in the array
  // by using pointer arithmetic to shift the pointer along the array
  return (void *)((char *)array->data + (element * array->element_size));
}

/*
 * Copies the contents of a dynamic array into a new array_t
 * with just enough space for all elements
 */
array_t dynamic_array_to_array(dynamic_array_t *array) {
  array_t converted = new_array(array->len, array->element_size);

  // Copy the memory across
  memcpy(converted.data, array->data, array->len * array->element_size);

  // Return the copied array
  return converted;
}

// Wrapper function to destroy the contents of an array
void array_destroy(array_t toDestroy) { destroy(toDestroy.data); }

// Wrapper function to destroy the contents of an array
void dynamic_array_destroy(dynamic_array_t toDestroy) {
  destroy(toDestroy.data);
}