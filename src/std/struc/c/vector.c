#include "../vector.h"

/*
 * Makes space for the given number of elements (if required)
 */
static void vector_make_space(vector_t *array, int req_len);

/*
 * Extends an array (if it is full)
 */
static void vector_extend(vector_t *array);

//////////////////////////////
// Initialisation
//////////////////////////////

vector_t vector_new(size_t elem_size) {
  vector_t array;

  array.elem_size = elem_size;
  array.data = NULL;
  array.len = 0;
  array.capacity = 0;
  array.was_allocated = false;
  array.destroy = NULL;
  array.destroy_on_remove = true;

  return array;
}

vector_t vector_wrap(void *data, int len, int capacity,
                                   size_t elem_size, bool was_allocated) {
  vector_t array;

  array.elem_size = elem_size;
  array.data = data;
  array.len = len;
  array.capacity = capacity;
  array.was_allocated = was_allocated;
  array.destroy = NULL;
  array.destroy_on_remove = true;

  return array;
}

//////////////////////////////
// Basic Operations
//////////////////////////////

void vector_append(vector_t *array, void *data) {
  vector_insert(array, array->len, data);
}

void vector_insert(vector_t *array, int index, void *data) {
  index = index_convert_negative(array->len, index);
  index = index_constrain(array->len + 1, index);
  vector_make_space(array, array->len + 1);

  void *elem = array_generic_get(array->data, index, array->elem_size);

  int elems_to_move = array->len - index;

  if (elems_to_move != 0) {
    void *one_up = array_generic_get(array, index + 1, array->elem_size);
    assert(memmove(one_up, elem, elems_to_move * array->elem_size));
  }

  assert(memcpy(elem, data, array->elem_size));

  array->len += 1;
}

void *vector_get(vector_t *array, int index) {
  index = index_convert_negative_safe(array->len, index);
  return offset(array->data, index * array->elem_size);
}

void vector_set(vector_t *array, int index, void *data) {
  index = index_convert_negative_safe(array->len, index);

  void *elem_start = vector_get(array, index);
  assert(memcpy(elem_start, data, array->elem_size));
}

bool vector_remove(vector_t *array, void *elem,
                          int (*key)(void *elem1, void *elem2)) {
  for (int i = 0; i < array->len; i++) {
    void *curr_elem = vector_get(array, i);
    if (key(curr_elem, elem) == 0) {
      // delete element data if enabled
      if (array->destroy_on_remove && array->destroy) {
        array->destroy(curr_elem);
      }

      if (i != array->len - 1) {
        void *next_elem = vector_get(array, i + 1);
        size_t bytes_to_move = (array->len - 1 - i) * array->elem_size;
        memmove(curr_elem, next_elem, bytes_to_move);
      }
      array->len -= 1;
      return true;
    }
  }
  return false;
}

void vector_remove_at(vector_t *array, int index) {
  // Converts negative indexes to positive and ensures the index is valid
  index = index_convert_negative_safe(array->len, index);

  void *curr_elem = vector_get(array, index);

  // delete element data if enabled
  if (array->destroy_on_remove && array->destroy) {
    array->destroy(curr_elem);
  }

  // Shift all elements past this index down
  if (index != array->len - 1) {
    void *next_elem = vector_get(array, index + 1);
    size_t bytes_to_move = (array->len - 1 - index) * array->elem_size;
    memmove(curr_elem, next_elem, bytes_to_move);
  }

  array->len -= 1;
}

void *vector_copy_elem(vector_t *array, int index) {
  void *elem = vector_get(array, index);
  void *copy = safe_malloc(array->elem_size);
  assert(memcpy(copy, elem, array->elem_size));
  return copy;
}

// TODO re-write description for function, as it does not allocate memory
void vector_pop(vector_t *array, void* dest, int index) {
  void *elem = vector_get(array, index);
  assert(memcpy(dest, elem, array->elem_size));
  vector_remove_at(array, index);
}

//////////////////////////////
// High Level Functoins
//////////////////////////////

array_t vector_to_array(vector_t *array) {
  array_t converted = array_new(array->len, array->elem_size);

  memcpy(converted.data, array->data, array->len * array->elem_size);

  return converted;
}

void vector_resize(vector_t *array, int new_len, void *template) {
  if (new_len == array->len) {
    return;
  }

  if (new_len < 0) {
    exit_error("Cannot resize a vector to a negative value",
               "std/c/vector.c", "vector_resize");
  }

  int old_len = array->len;

  if (new_len > old_len) {
    // Resize the array up
    // Add new elements which are copies of "template" in all new spots
    array->len = new_len;
    vector_make_space(array, new_len);
    if (template) {
      for (int i = old_len; i < new_len; i++) {
        void *elem = vector_get(array, i);
        assert(memcpy(elem, template, array->elem_size));
      }
    } else {
      // Set all new memory for the array past the old end of the array to 0
      void *elem = vector_get(array, old_len);
      memset(elem, 0, (new_len - old_len) * array->elem_size);
    }
  } else if (new_len < array->len) {
    // Shrink the array
    // Destroy elements removed by shrinking if enabled
    if (array->destroy_on_remove && array->destroy) {
      for (int i = new_len; i < old_len; i++) {
        array->destroy(vector_get(array, i));
      }
    }
    array->len = new_len;
  }
}

//////////////////////////////
// Utility Functions
//////////////////////////////

void vector_set_capacity(vector_t *array, int req_len) {
  // Ensure the requested capacity is valid for the array's state
  if (req_len < 0) {
    exit_error("Cannot set negative capacity in a vector",
               "std/c/vector.c", "vector_set_capacity");
  } else if (req_len < array->len) {
    exit_error("Cannot set vector's capacity to lower than its length",
               "std/c/vector.c", "vector_set_capacity");
  } else if (req_len == array->capacity || req_len == 0) {
    return;
  }

  size_t arr_size = array->len * array->elem_size;
  size_t new_size = req_len * array->elem_size;

  // Move the array to new memory of the requested size
  void *new_mem = safe_malloc(new_size);
  if (array->len > 0) {
    assert(memcpy(new_mem, array->data, arr_size));
  }
  if (array->was_allocated) {
    destroy(array->data);
  }
  array->data = new_mem;
  array->was_allocated = true;

  // Update capacity
  array->capacity = req_len;
}

static void vector_make_space(vector_t *array, int req_len) {
  if (array->capacity < req_len) {

    if (req_len > array->capacity + (array->capacity / 2)) {
      vector_set_capacity(array, req_len);
    } else {
      vector_extend(array);
    }
  }
}

static void vector_extend(vector_t *array) {
  if (array->len < array->capacity) {
    return;
  }

  // Increment the capacity by a factor of 1.5x
  int new_len = array->capacity + (array->capacity / 2);
  if (array->capacity <= 1) {
    new_len = (array->capacity + 1);
  }

  size_t new_size = new_len * array->elem_size;

  // Extend the array
  void* new_mem = safe_malloc(new_size);
  if (array->len > 0) {
    assert(memcpy(new_mem, array->data, array->len * array->elem_size));
  }
  if (array->was_allocated) {
    destroy(array->data);
  }

  array->data = new_mem;
  array->was_allocated = true;
  array->capacity = new_len;
}

//////////////////////////////
// Cleanup
//////////////////////////////

void vector_clear(vector_t *array) {
  vector_resize(array, 0, NULL);
}

void vector_destroy(vector_t *array) {
  // Destroy the elements in the array if enabled
  if (array->destroy_on_remove && array->destroy) {
    for (int i = 0; i < array->len; i++) {
      array->destroy(vector_get(array, i));
    }
  }

  // Destroy the array itself
  if (array->was_allocated) {
    destroy(array->data);
  }

  array->len = 0;
  array->capacity = 0;
  array->data = NULL;
  array->was_allocated = false;
}
