#ifndef CLIB_STD_CIRCULAR_ARRAY_H
#define CLIB_STD_CIRCULAR_ARRAY_H


#include "array.h"
#include "memory.h"
#include <stdlib.h>

// TODO
/*
 * is this just better than a dynamic array?
 * like we can get elements in constant time
 * and if we're appending its still o(1)
 * and removal would be relative to a quarter of the size of the array rather
 * than half cause removing random eleents from a dynamic array is on average
 * n/2 but it takes longer if it's near the start of the array, up to n so we
 * can reduce that to n/2 or on average 1/4 maybe or something?
 *
 * so when extending, we want to minimize the amount of copying, except heres
 * the thing is that we have to copy everything anyway so we may as well
 * re-organise the memory
 */
#define CIRCULAR_CAPACITY_DEFAULT 2
#define CIRCULAR_EXTENSION_FACTOR 2

typedef struct circular_t {
  int start;
  int end;
  int len;
  int capacity;
  size_t elem_size;
  void *data;
  void (*destroy)(void* data);
} circular_t;

//////////////////////////////
// Initialisation
//////////////////////////////

circular_t circular_new(size_t elem_size, int capacity);

//////////////////////////////
// Basic Operations
//////////////////////////////

void circular_append(circular_t *circular, void *data);

void circular_prepend(circular_t *circular, void *data);

bool circular_pop_last(circular_t *circular, void *dest);

bool circular_pop_first(circular_t *circular, void *dest);

void *circular_peek(circular_t *circular, int index);

//////////////////////////////
// Utility
//////////////////////////////

int circular_add_pos(int len, int start, int add);

void circular_extend(circular_t *circular);

//////////////////////////////
// Utility
//////////////////////////////

void circular_destroy(circular_t *circular);

#endif

// TODO write this function below
/*
void* circular_insert(circular_t* circular, int index, void* data) {
  if (!index == circular->len) {
    index = index_convert_negative_safe(circular->len, index);
  }
  circular_extend(circular);

  int chunk_start[4];
  int chunk_len[4];


  if (!circular->len) {
    // No chunks exist
    for (int i = 0; i < 4; i++) {
      chunk_start[i] = -1;
      chunk_len[i] = -1;
    }
  }
  else {
    if (index)

  }
  int elements_before = circular->len - index;
  int elements_after = circular->len - elements_before;

  index = (circular->start + index) % circular->capacity;

  void* elem = array_generic_get(circular->data, index, circular->elem_size);
  // can we make this easier?

  if (elements_before < elements_after) {
    int start_of_before = circular->start;
    int end_of_before = (start_of_before + elements_before) %
circular->capacity; if (end_of_before < start_of_before) {
      // Elements are in two chunks, one at the end of the list, one at the
start

      // This means that the array is something like 4 GAP 1 2 3
      // if we're inserting at 2 (which means where the three is)
      // means we shift the one and the two back one position
      // then

      // Get the position of the start to the end of the list
      // the position of the next chunk is 0
      //    Get the length of the next chunk
      // Move the chunk at the end of the list back one
      // move the first element back one
      // move the rest of the chunk back one if tehre is more than one element
in teh second chunk

    }
    else {
      // Elements are in one chunk
      if (circular->start == 0) {
        int first_pos;
        int elements_in_chunk;
        int prev_pos;
        void* first;
        void* prev;
        if (first_pos == 0) {
          assert(memcpy(prev, first, circular->elem_size));
          // Now copy everything past the first element back one if there are
more elements
        }
        else {
          // You can just move everything back to prev
          size_t chunk_size;
          assert(memmove(prev, first, chunk_size));
        }

        circular->start = prev_pos;
        // Move the last element to the end of the list,
        // Move the rest of the elements back one if there are multiple
        // make the list point at the last element
      }
      else {
        // Move everything back one
      }
    }
  }
  assert(memcpy(elem, data, circular->elem_size));

  circular->end = (circular->end + 1) % circular->capacity;
  circular->len += 1;
}
*/
