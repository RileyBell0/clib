#include "../circular_array.h"

//////////////////////////////
// Initialisation
//////////////////////////////

circular_t circular_new(size_t elem_size, int capacity) {
  circular_t circular;

  circular.start = -1;
  circular.end = -1;
  circular.len = 0;
  circular.elem_size = elem_size;

  // Setup queue capacity
  if (capacity <= 0) {
    circular.data = NULL;
    circular.capacity = 0;
  } else {
    circular.data = safe_malloc(elem_size * capacity);
    circular.capacity = capacity;
  }

  circular.destroy = NULL;

  return circular;
}
//////////////////////////////
// Basic Operations
//////////////////////////////

void circular_append(circular_t *circular, void *data) {
  circular_extend(circular);

  if (circular->len) {
    int circular_new_end = (circular->end + 1) % circular->capacity;
    array_generic_set(circular->data, data, circular_new_end,
                      circular->elem_size);
    circular->end = circular_new_end;
  } else {
    array_generic_set(circular->data, data, 0, circular->elem_size);
    circular->start = 0;
    circular->end = 0;
  }

  circular->len += 1;
}

void circular_prepend(circular_t *circular, void *data) {
  circular_extend(circular);

  if (circular->len) {
    int circular_new_start = (circular->start - 1) % circular->capacity;
    array_generic_set(circular->data, data, circular_new_start,
                      circular->elem_size);
    circular->start = circular_new_start;
  } else {
    array_generic_set(circular->data, data, 0, circular->elem_size);
    circular->start = 0;
    circular->end = 0;
  }

  circular->len += 1;
}

bool circular_pop_last(circular_t *circular, void *dest) {
  if (!circular->len) {
    return false;
  }

  void *last =
      array_generic_get(circular->data, circular->end, circular->elem_size);
  assert(memcpy(dest, last, circular->elem_size));

  circular->len -= 1;

  if (circular->len) {
    circular->end = (circular->end - 1) % circular->capacity;
  } else {
    circular->start = -1;
    circular->end = -1;
  }

  return true;
}

bool circular_pop_first(circular_t *circular, void *dest) {
  if (!circular->len) {
    return false;
  }

  void *first =
      array_generic_get(circular->data, circular->start, circular->elem_size);
  assert(memcpy(dest, first, circular->elem_size));

  circular->len -= 1;

  if (circular->len) {
    circular->start = (circular->start + 1) % circular->capacity;
  } else {
    circular->start = -1;
    circular->end = -1;
  }

  return true;
}

void *circular_peek(circular_t *circular, int index) {
  // Convert negative integers into array indexes
  index = index_convert_negative_safe(circular->len, index);

  // Convert array indexes into their positions in the circular array
  index = (circular->start + index) % circular->capacity;

  return array_generic_get(circular->data, index, circular->elem_size);
}

//////////////////////////////
// Utility
//////////////////////////////

int circular_add_pos(int len, int start, int add) {
  return (start + add) % len;
}

void circular_extend(circular_t *circular) {
  if (circular->len < circular->capacity) {
    return;
  }

  // Determine the new capacity
  int new_capacity = CIRCULAR_EXTENSION_FACTOR * circular->capacity;
  if (new_capacity == 0) {
    circular->capacity = CIRCULAR_CAPACITY_DEFAULT;
  }

  // TODO use realloc

  // Copy the elements across to the new memory block
  void *new_mem = safe_malloc(new_capacity * circular->elem_size);
  void *new_mem_start = new_mem;
  int seg_1_len = circular->capacity - circular->start;
  void *seg_1_pos =
      array_generic_get(circular->data, circular->start, circular->elem_size);
  size_t seg_1_size = seg_1_len * circular->elem_size;
  assert(memcpy(new_mem, seg_1_pos, seg_1_size));

  // Determine if there's a second segment, and if so, add it to the end of
  // the copied memory from the first segment
  int seg_2_len = circular->len - seg_1_len;
  if (seg_2_len != 0) {
    size_t seg_2_size = seg_2_len * circular->elem_size;
    new_mem_start = offset(new_mem_start, seg_1_size);
    assert(memcpy(new_mem_start, circular->data, seg_2_size));
  }

  destroy(circular->data);
  circular->data = new_mem;
  circular->start = 0;
  circular->end = circular->len - 1;
  circular->capacity = new_capacity;
}

//////////////////////////////
// Cleanup
//////////////////////////////

void circular_destroy(circular_t *circular) {
  if (circular->destroy) {
    int elem_pos = 0;
    while (elem_pos < circular->len) {
      int index = (circular->start + elem_pos + 1) % circular->capacity;
      void *elem =
          array_generic_get(circular->data, index, circular->elem_size);
      circular->destroy(elem);
    }
  }
  destroy(circular->data);
  circular->start = -1;
  circular->end = -1;
  circular->data = NULL;
  circular->len = 0;
  circular->capacity = 0;
}

// TODO ensure dynamic array is destroying its array data and is setting
// capacity to zero