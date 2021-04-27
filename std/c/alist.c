#include "../alist.h"

// NOTE - for fixing code
// alist_relative_ptr = int32_t dest - int32_t src

char alist_iterator_done(alist_iterator_t *iterator);
void* alist_iterator_first_node(alist_iterator_t* iterator);
void* alist_iterator_next_node(alist_iterator_t* iterator);
void* alist_iterator_prev_node(alist_iterator_t* iterator);

alist_t new_alist(size_t element_size) {
  alist_t list;

  list.list_start = NULL;
  list.destroy_on_remove = TRUE;
  list.first = ALIST_NULL;
  list.last = ALIST_NULL;
  list.size = 0;
  list.capacity = 0;
  list.element_size = element_size;
  list.block_size = element_size + sizeof(alist_node_t);
  list.compare = NULL;
  list.destroy = NULL;

  return list;
}

alist_iterator_t new_alist_iterator(alist_t* list, char from_start) {
  alist_iterator_t iterator;
  iterator.list = list;
  iterator.done = alist_iterator_done;
  iterator.index = ALIST_NULL;
  iterator.from_start = from_start;

  // Set direction dependent information
  if (from_start) {
    iterator.next_node_pos = list->first;
    iterator.next = alist_iterator_next_node;
  }
  else {
    iterator.next_node_pos = list->last;
    iterator.next = alist_iterator_prev_node;
  }

  // Hook the iterator up to the first node in the given direction
  iterator.first = alist_iterator_first_node;

  return iterator;
}

char alist_iterator_done(alist_iterator_t *iterator) {
  return iterator->curr_node_pos < 0;
}

void* alist_iterator_first_node(alist_iterator_t* iterator) {
  // Get the index of the starting node
  if (iterator->from_start) {
    iterator->next_node_pos = iterator->list->first;
  } else {
    iterator->next_node_pos = iterator->list->last;
  }

  // Get the first element
  void* element = iterator->next(iterator);

  // Get the index of the starting node
  if (iterator->from_start) {
    iterator->index = 0;
  } else {
    iterator->index = iterator->list->size - 1;
  }

  return element;
}

void* alist_iterator_next_node(alist_iterator_t* iterator) {
  // Moving to the next element
  iterator->curr_node_pos = iterator->next_node_pos;
  iterator->curr_node = array_get_element(iterator->list->list_start, 
                                          iterator->next_node_pos, 
                                          iterator->list->block_size);
  iterator->next_node_pos += iterator->curr_node->next;
  iterator->element = (void*)&iterator->curr_node[ALIST_ELEMENT];

  iterator->index += 1;
  // Returns non-null if not done
  return iterator->element;
}

void* alist_iterator_prev_node(alist_iterator_t* iterator) {
  // Moving to the next element
  iterator->curr_node_pos = iterator->next_node_pos;
  iterator->curr_node = array_get_element(iterator->list->list_start, 
                                          iterator->next_node_pos, 
                                          iterator->list->block_size);
  iterator->next_node_pos += iterator->curr_node->prev;
  iterator->element = (void*)&iterator->curr_node[ALIST_ELEMENT];

  iterator->index -= 1;
  // Returns non-null if not done
  return iterator->element;
}

// TODO this could be made a fair bit faster removing the bloat of the
// list iterator and just hard-coding it
void *alist_index(alist_t *list, int32_t index) {
  if (index < 0 || index >= list->size) {
    return NULL;
  }

  // Create the iterator
  alist_iterator_t it;
  if (index <= list->size / 2) {
    it = new_alist_iterator(list, TRUE);
  } else {
    it = new_alist_iterator(list, FALSE);
  }

  // return a pointer to the element at the given index
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    if (it.index == index) {
      return it.element;
    }
  }
  
  // This should only return null at this point if the list is invalid
  return NULL;
}

int32_t *alist_generate_complementary_ledger(alist_t *list) {
  if (list->capacity == 0) {
    return NULL;
  }

  int32_t *ledger = safe_malloc(sizeof(int32_t) * list->capacity);

  int32_t curr = list->first;
  if (curr != ALIST_NULL) {
    int32_t curr_node = 0;
    while (curr >= 0) {
      alist_node_t *node =
          array_get_element(list->list_start, curr, list->block_size);

      // Save the location of the current node to the ledger
      ledger[curr] = curr_node;

      // Move on to the next node
      curr += node->next;
      ++curr_node;
    }
  }

  return ledger;
}

int32_t *alist_generate_ledger(alist_t *list) {
  if (list->capacity == 0) {
    return NULL;
  }

  int32_t *ledger = safe_malloc(sizeof(int32_t) * list->capacity);

  int32_t curr = list->first;
  if (curr != ALIST_NULL) {
    int32_t curr_node = 0;
    while (curr >= 0) {
      alist_node_t *node =
          array_get_element(list->list_start, curr, list->block_size);

      // Save the location of the current node to the ledger
      ledger[curr_node] = curr;

      // Move on to the next node
      curr += node->next;
      ++curr_node;
    }
  }

  return ledger;
}

// TODO this doesnt combine the lists if list1 is empty
// it should in that case copy the list
// need to make that function
alist_t *alist_combine(alist_t *list1, alist_t *list2) {
  // Cannot combine two lists of different types
  // Note that this does not check types, only the size of types
  uint32_t combined_size = list1->size + list2->size;

  if (list1->element_size != list2->element_size || combined_size >= INT32_MAX) {
    return NULL;
  }
  if (list2->size == 0) {
    return list1;
  }

  // Allocate enough space to store the combined lists
  if (combined_size > list1->capacity) {
    alist_set_length(list1, combined_size);
  }


  alist_node_t *list1_end = array_get_element(list1->list_start, 
                                              list1->size, 
                                              list1->block_size);

  alist_node_t *list2_first = array_get_element(list1->list_start, 
                                                list1->size + list2->first, 
                                                list1->block_size);

  alist_node_t *list1_last = array_get_element(list1->list_start, 
                                               list1->last, 
                                               list1->block_size);
    
  // Copy list2's data across and ensure the copy succeeds
  assert(memcpy(list1_end, list2->list_start, list2->block_size * list2->size));

  list2_first->prev = list1->last - list1->size - list2->first;
  list1_last->next = list1->size + list2->first - list1->last;

  // New last element
  list1->last = list1->size + list2->last;

  // New list size
  list1->size = combined_size;

  return list1;
}

void alist_extend(alist_t *list) {
  // Calculate the new length, realloc factor set to be 1.5x
  int32_t new_len = list->capacity + list->capacity / 2;

  // Increment to avoid new_len getting stuck at 0 or 1
  if (new_len <= 1) {
    ++new_len;
  }

  // Convert new_len from num elements into num bytes
  size_t new_capacity = (sizeof(alist_node_t) + list->element_size) * new_len;

  // Extend the list's memory and update its capacity
  list->list_start = safe_realloc(list->list_start, new_capacity);
  list->capacity = new_len;
}

void alist_set_length(alist_t *list, int32_t new_len) {
  // Can only increase the list's length
  if (new_len <= list->capacity) {
    return;
  }

  // Convert new_len from num elements into num bytes
  size_t new_capacity = list->block_size * new_len;

  // Extend the list's memory and update its capacity
  list->list_start = safe_realloc(list->list_start, new_capacity);
  list->capacity = new_len;
}

void alist_append(alist_t *list, void *data) {
  // Make space for the new element
  if (list->capacity <= list->size) {
    alist_extend(list);
  }

  alist_node_t *newNode = (alist_node_t *)array_get_element(
      list->list_start, list->size, list->block_size);

  // Patching reference so the new node is appended
  if (list->size) {
    /*
     * Point the last element in the list to the new appended element
     */
    alist_node_t *last = (alist_node_t *)array_get_element(
        list->list_start, list->last, list->block_size);

    last->next = list->size - list->last;
    newNode->prev = list->last - list->size;
  } else {
    newNode->prev = ALIST_NULL;
    list->first = 0;
  }

  list->last = list->size;
  newNode->next = ALIST_NULL;

  // Copy the data in and Ensure the copy succeeds
  assert(memcpy(&newNode[ALIST_ELEMENT], data, list->element_size));

  list->size += 1;
}

/*
 * so we want an array which is instantly accessable
 * and yet also changes
 * is there a way we can do this?
 * im not sure if its even possible
 * im starting to think that it may not be
 * if we delete an element in a list,
 * fixing the ledger is easy
 */

void alist_remove_node(alist_t *list, alist_node_t *node, uint32_t curr) {
  // Calculate the positions of the surrounding nodes
  int32_t next = curr + node->next; // alist min
  int32_t prev = curr + node->prev; // 1
  int32_t list_final = list->size - 1; // 2

  // Make the nodes around the removed node point at eachother
  if (prev >= 0) {
    alist_node_t *prevnode = ((alist_node_t *)array_get_element(
        list->list_start, prev, list->block_size));
    if (next >= 0) {
      prevnode->next = next - prev;
    } else {
      prevnode->next = ALIST_NULL;
    }

    // Fix overall list references
    if (list->last == curr) {
      list->last = prev;
    }
    else {
      list->last = ALIST_NULL;
    }
  }
  if (next >= 0) {
    alist_node_t *nextnode = ((alist_node_t *)array_get_element(
        list->list_start, next, list->block_size));
    if (prev >= 0) {
      nextnode->prev = prev - next;
    } else {
      nextnode->prev = ALIST_NULL;
    }

    // Fix overall list references
    if (list->first == curr) {
      list->first = next;
    } else {
      list->first = ALIST_NULL;
    }
  }

  alist_node_t *moved_node =
      array_get_element(list->list_start, curr, list->block_size);

  // Delete the data from the node if requested
  if (list->destroy_on_remove && list->destroy) {
    list->destroy(&moved_node[ALIST_ELEMENT]);
  }

  // Fill the empty spot in the array (if one exists)
  if (curr != list_final) {

    // Get the last (array[-1]) element in the array
    int32_t node_orig_pos = list_final;
    alist_node_t *base_node =
        array_get_element(list->list_start, node_orig_pos, list->block_size);

    int32_t base_next = base_node->next;
    int32_t base_prev = base_node->prev;

    // Fill new empty spot in the array with the current final element
    array_set_element(list->list_start, base_node, curr, list->block_size);

    // Update the pointers of the moved node to match its new position
    if (base_next != ALIST_NULL) {
      moved_node->next = list_final + base_next - curr;
    }
    if (base_prev != ALIST_NULL) {

      moved_node->prev = list_final + base_prev - curr;
    }

    // Fix the surrounding node pointers
    if (moved_node->prev != ALIST_NULL) {
      int32_t prev_node_pos = node_orig_pos + base_prev;
      ((alist_node_t *)array_get_element(list->list_start, prev_node_pos,
                                         list->block_size))
          ->next = curr - prev_node_pos;
    }
    if (moved_node->next != ALIST_NULL) {
      int32_t next_node_pos = node_orig_pos + base_next;
      ((alist_node_t *)array_get_element(list->list_start, next_node_pos,
                                         list->block_size))
          ->prev = curr - next_node_pos;
    }
  }

  // Patch the overall list first/last references to match the new list info
  if (list_final == list->last) {
    list->last = curr;
  }
  if (list_final == list->first) {
    list->first = curr;
  }

  list->size -= 1;
}

int alist_remove(alist_t *list, void *element) {
  if (list->size == 0) {
    return FALSE;
  }

  // Find the first occurance of the given element in the list and remove it
  alist_iterator_t iterator = new_alist_iterator(list, TRUE);
  for (void* node_data = iterator.first(&iterator); !iterator.done(&iterator); node_data = iterator.next(&iterator)) {
    if (list->compare(element, node_data) == 0) {
      alist_remove_node(list, iterator.curr_node, iterator.next_node_pos - iterator.curr_node->next);
      return TRUE;
    }
  }

  return FALSE;
}

void *alist_get_element(alist_t *list, uint32_t element) {
  alist_node_t *node = (alist_node_t *)array_get_element(
      list->list_start, element, list->element_size + sizeof(alist_node_t));
  return (void *)&node[ALIST_ELEMENT];
}

void alist_destroy(alist_t *list) {
  // Check if any memory has been allocated for the list
  if (list->capacity == 0) {
    return;
  }

  // Check if there is a method for destroying the elements in the list
  if (list->destroy) {

    // Go through the list and destroy each element
    for (uint32_t i = 0; i < list->size; i++) {

      // Get a reference to and destroy the current element in the list
      alist_node_t *node =
          array_get_element(list->list_start, i, list->block_size);
      list->destroy(&node[ALIST_ELEMENT]);
    }
  }

#ifdef ALIST_SAFE_DESTROY
  // Reset the list, meaning if someone tries to use it again
  // it doesnt break anything
  list->capacity = 0;
  list->first = ALIST_NULL;
  list->last = ALIST_NULL;
#endif

  // Free the memory occupied by the list itself
  free(list->list_start);
  list->list_start = NULL;
}
