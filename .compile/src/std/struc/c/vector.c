#include "../vector.h"

/*
 * Makes space for the given number of elements (if required)
 */
static void vector_make_space(vector_t *array, size_t req_len);

/*
 * Extends an array (if it is full)
 */
static void vector_extend(vector_t *array);

//////////////////////////////
// Initialisation
//////////////////////////////

vector_t vector_new(size_t elem_size, void (*destroy)(void *data))
{
  vector_t vec;

  vec.data = NULL;
  vec.elem_size = elem_size;
  vec.len = 0;
  vec.capacity = 0;
  vec.destroy = destroy;

  return vec;
}

//////////////////////////////
// Basic Operations
//////////////////////////////

void vector_append(vector_t *vec, void *data)
{
  vector_make_space(vec, vec->len + 1);
  vec->len += 1;
  vector_set(vec, vec->len - 1, data);
}

void *vector_get(vector_t *vec, size_t index)
{
  return array_generic_get(vec->data, index, vec->elem_size);
}

void vector_set(vector_t *vec, size_t index, void *data)
{
  array_generic_set(vec->data, data, index, vec->elem_size);
}

bool vector_remove(vector_t *vec, void *elem,
                   int (*compare)(void *elem1, void *elem2))
{
  for (size_t i = 0; i < vec->len; i++)
  {
    void *curr_elem = vector_get(vec, i);
    if (compare(curr_elem, elem) == 0)
    {
      // delete element data if enabled
      if (vec->destroy)
      {
        vec->destroy(curr_elem);
      }

      // Shift the vector data down to fill the gap
      if (i != vec->len - 1)
      {
        void *next_elem = vector_get(vec, i + 1);
        size_t bytes_to_move = (vec->len - 1 - i) * vec->elem_size;
        memmove(curr_elem, next_elem, bytes_to_move);
      }

      vec->len -= 1;

      return true;
    }
  }
  return false;
}

void vector_remove_at(vector_t *vec, size_t index)
{
  void *curr_elem = vector_get(vec, index);

  // delete element data if enabled
  if (vec->destroy)
  {
    vec->destroy(curr_elem);
  }

  // Shift all elements past this index down
  if (index != vec->len - 1)
  {
    void *next_elem = vector_get(vec, index + 1);
    size_t bytes_to_move = (vec->len - 1 - index) * vec->elem_size;
    memmove(curr_elem, next_elem, bytes_to_move);
  }

  vec->len -= 1;
}

void vector_pop(vector_t *vec, void *dest, size_t index)
{
  void *elem = vector_get(vec, index);
  if (dest)
  {
    // Copy the elem out of the vector
    assert(memcpy(dest, elem, vec->elem_size));
  }
  else if (vec->destroy)
  {
    // Destroy the elem (don't copy)
    vec->destroy(elem);
  }

  vector_remove_at(vec, index);
}

//////////////////////////////
// High Level Functoins
//////////////////////////////

array_t vector_to_array(vector_t *vec)
{
  array_t converted = array_new(vec->len, vec->elem_size, vec->destroy);

  if (vec->len > 0)
  {
    memcpy(converted.data, vec->data, vec->len * vec->elem_size);
  }

  return converted;
}

//////////////////////////////
// Utility Functions
//////////////////////////////

/*
 * Ensures the vector has at lesat req_len total capacity
 */
void vector_set_capacity(vector_t *vec, size_t req_len)
{
  // Ensure the requested capacity is valid for the array's state
  if (req_len < 0)
  {
    exit_error("Cannot set negative capacity in a vector",
               "std/c/vector.c", "vector_set_capacity");
  }
  else if (req_len < vec->len)
  {
    exit_error("Cannot set vector's capacity to lower than its length",
               "std/c/vector.c", "vector_set_capacity");
  }
  else if (req_len == vec->capacity || req_len == 0)
  {
    return;
  }

  // Move the array to new memory of the requested size
  size_t arr_size = vec->len * vec->elem_size;
  size_t new_size = req_len * vec->elem_size;
  void *new_mem = safe_malloc(new_size);
  if (vec->len > 0)
  {
    assert(memcpy(new_mem, vec->data, arr_size));
  }
  if (vec->data)
  {
    free(vec->data);
  }

  // Update vector details
  vec->data = new_mem;
  vec->capacity = req_len;
}

// This function is good for combining vectors
static void vector_make_space(vector_t *vec, size_t req_len)
{
  if (vec->capacity < req_len)
  {

    if (req_len > vec->capacity + (vec->capacity / 2))
    {
      // If requested space is greater than a single auto extension
      vector_set_capacity(vec, req_len);
    }
    else
    {
      vector_extend(vec);
    }
  }
}

static void vector_extend(vector_t *vec)
{
  if (vec->len < vec->capacity)
  {
    return;
  }

  // Increment the capacity by a factor of 1.5x
  int new_len = vec->capacity + (vec->capacity / 2);
  if (vec->capacity <= 1)
  {
    new_len = (vec->capacity + 1);
  }

  // Extend the vector
  size_t new_size = new_len * vec->elem_size;
  void *new_mem = safe_malloc(new_size);
  if (vec->len > 0)
  {
    assert(memcpy(new_mem, vec->data, vec->len * vec->elem_size));
  }
  if (vec->data)
  {
    free(vec->data);
  }

  // Update vector information
  vec->data = new_mem;
  vec->capacity = new_len;
}

vector_t *vector_combine(vector_t *vec, vector_t *ext)
{
  // Make space for the new elems
  vector_set_capacity(vec, vec->len + ext->len);

  // Append "ext" to the end of "vec"
  void *vec_end = array_generic_get(vec->data, vec->len, vec->elem_size);
  assert(memcpy(vec_end, ext->data, ext->elem_size * ext->len));
  vec->len = vec->len + ext->len;

  // Cleanup
  ext->len = 0;
  vector_clear(ext);
  return vec;
}

//////////////////////////////
// Cleanup
//////////////////////////////

void vector_clear(vector_t *vec)
{
  // Destroy dynamically allocated data
  if (vec->destroy)
  {
    for (size_t i = 0; i < vec->len; i++)
    {
      vec->destroy(vector_get(vec, i));
    }
  }

  // Clear the vector array
  if (vec->data)
  {
    free(vec->data);
    vec->data = NULL;
  }

  // Update vector information
  vec->capacity = 0;
  vec->len = 0;
}

void vector_destroy(vector_t *vec)
{
  vector_clear(vec);
}
