#include "../alist.h"

// NOTE - for fixing code
// alist_relative_ptr = int dest - int src

char alist_iterator_done(alist_iterator_t *iterator);
void *alist_iterator_first_node(alist_iterator_t *iterator);
void *alist_iterator_next_node(alist_iterator_t *iterator);
void *alist_iterator_prev_node(alist_iterator_t *iterator);
void alist_iterator_move(alist_iterator_t *iterator, unsigned int index);

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

alist_iterator_t new_alist_iterator(alist_t *list, char from_start) {
  alist_iterator_t iterator;
  iterator.index = ALIST_NULL;
  iterator.list = list;
  iterator.from_start = from_start;

  // Set direction dependent information
  if (from_start) {
    iterator.next_node_pos = list->first;
    iterator.next = alist_iterator_next_node;
  } else {
    iterator.next_node_pos = list->last;
    iterator.next = alist_iterator_prev_node;
  }
  iterator.done = alist_iterator_done;
  iterator.first = alist_iterator_first_node;

  return iterator;
}

char alist_iterator_done(alist_iterator_t *iterator) {
  return iterator->curr_node_pos < 0;
}

void *alist_iterator_first_node(alist_iterator_t *iterator) {
  // Get the index of the starting node
  if (iterator->from_start) {
    iterator->next_node_pos = iterator->list->first;
  } else {
    iterator->next_node_pos = iterator->list->last;
  }

  // Get the first element
  void *element = iterator->next(iterator);

  // Get the index of the starting node
  if (iterator->from_start) {
    iterator->index = 0;
  } else {
    iterator->index = iterator->list->size - 1;
  }

  return element;
}

void *alist_iterator_next_node(alist_iterator_t *iterator) {
  // Moving to the next element
  iterator->curr_node_pos = iterator->next_node_pos;

  if (iterator->next_node_pos < 0) {
    return NULL;
  }

  iterator->curr_node =
      array_get_element(iterator->list->list_start, iterator->next_node_pos,
                        iterator->list->block_size);
  iterator->next_node_pos += iterator->curr_node->next;
  iterator->element = (void *)&iterator->curr_node[ALIST_ELEMENT];

  iterator->index += 1;
  // Returns non-null if not done
  return iterator->element;
}

void *alist_iterator_prev_node(alist_iterator_t *iterator) {
  // Moving to the next element
  iterator->curr_node_pos = iterator->next_node_pos;

  if (iterator->next_node_pos < 0) {
    return NULL;
  }

  iterator->curr_node =
      array_get_element(iterator->list->list_start, iterator->next_node_pos,
                        iterator->list->block_size);
  iterator->next_node_pos += iterator->curr_node->prev;
  iterator->element = (void *)&iterator->curr_node[ALIST_ELEMENT];

  iterator->index -= 1;
  // Returns non-null if not done
  return iterator->element;
}

// TODO this could be made a fair bit faster removing the bloat of the
// list iterator and just hard-coding it
// Index is an int instead of unsigned int to demonstrate the limited size of an
void *alist_index(alist_t *list, int index) {
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

alist_t alist_copy(alist_t *list) {
  alist_t new_list;

  // The list being copied is empty, so return an empty list of the same
  // element size
  if (list->size == 0) {
    new_list = new_alist(list->element_size);
    return new_list;
  }

  new_list.block_size = list->block_size;
  new_list.capacity = list->capacity;
  new_list.compare = list->compare;
  new_list.destroy = list->destroy;
  new_list.destroy_on_remove = list->destroy_on_remove;
  new_list.element_size = list->element_size;
  new_list.first = list->first;
  new_list.last = list->last;
  new_list.size = list->size;
  new_list.list_start = safe_malloc(new_list.block_size * new_list.size);
  assert(memcpy(new_list.list_start, list->list_start,
                new_list.block_size * new_list.size));

  return new_list;
}

alist_t *alist_combine(alist_t *list1, alist_t *list2) {
  if (!list1 || !list2) {
    return list1;
  }
  // Cannot combine two lists of different types
  // Note that this does not check types, only the size of types
  unsigned int combined_size = list1->size + list2->size;

  if (list1->element_size != list2->element_size ||
      combined_size >= INT32_MAX) {
    return list1;
  } else if (list2->size == 0) {
    return list1;
  }

  // Allocate enough space to store the combined lists
  if (combined_size > list1->capacity) {
    alist_set_length(list1, combined_size);
  }

  char has_last = TRUE;
  if (list1->last == ALIST_NULL) {
    has_last = FALSE;
  }

  alist_node_t *list1_end =
      array_get_element(list1->list_start, list1->size, list1->block_size);

  // Copy list2's data across and ensure the copy succeeds
  assert(memcpy(list1_end, list2->list_start, list2->block_size * list2->size));

  alist_node_t *list2_first = array_get_element(
      list1->list_start, list1->size + list2->first, list1->block_size);

  if (has_last) {
    alist_node_t *list1_last =
        array_get_element(list1->list_start, list1->last, list1->block_size);
    list1_last->next = list1->size + list2->first - list1->last;
    list2_first->prev = list1->last - list1->size - list2->first;
  } else {
    list2_first->prev = ALIST_NULL;
    list1->first = list2->first;
  }

  // New last element
  list1->last = list1->size + list2->last;

  // New list size
  list1->size = combined_size;

  return list1;
}

void alist_extend(alist_t *list) {
  // Calculate the new length, realloc factor set to be 1.5x
  int new_len = list->capacity + (list->capacity / 2);

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

void alist_set_length(alist_t *list, int new_len) {
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

array_t alist_to_array(alist_t *list) {
  array_t array = new_array(list->size, list->element_size);

  // Populate the array
  alist_iterator_t it = new_alist_iterator(list, TRUE);
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    array_set_index(&array, it.index, it.element);
  }

  return array;
}

void alist_remove_node(alist_t *list, alist_node_t *node, unsigned int curr) {
  // Calculate the positions of the surrounding nodes
  int next = curr + node->next;
  int prev = curr + node->prev;
  int list_final = list->size - 1;

  // Make the nodes around the removed node point at eachother
  if (prev >= 0) {
    alist_node_t *prevnode = ((alist_node_t *)array_get_element(
        list->list_start, prev, list->block_size));

    // Have the previous node point at the next node
    if (next >= 0) {
      prevnode->next = next - prev;
    } else {
      prevnode->next = ALIST_NULL;
    }
  }
  if (next >= 0) {
    alist_node_t *nextnode = ((alist_node_t *)array_get_element(
        list->list_start, next, list->block_size));

    // Have the next node point at the previous node
    if (prev >= 0) {
      nextnode->prev = prev - next;
    } else {
      nextnode->prev = ALIST_NULL;
    }
  }

  // Fix overall list references
  if (list->first == curr) {
    if (next >= 0) {
      list->first = next;
    } else {
      list->first = ALIST_NULL;
    }
  }

  // Fix overall list references
  if (list->last == curr) {
    if (prev >= 0) {
      list->last = next;
    } else {
      list->last = ALIST_NULL;
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
    int node_orig_pos = list_final;
    alist_node_t *base_node =
        array_get_element(list->list_start, node_orig_pos, list->block_size);

    int base_next = base_node->next;
    int base_prev = base_node->prev;

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
      int prev_node_pos = node_orig_pos + base_prev;
      ((alist_node_t *)array_get_element(list->list_start, prev_node_pos,
                                         list->block_size))
          ->next = curr - prev_node_pos;
    }
    if (moved_node->next != ALIST_NULL) {
      int next_node_pos = node_orig_pos + base_next;
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
  for (void *node_data = iterator.first(&iterator); !iterator.done(&iterator);
       node_data = iterator.next(&iterator)) {
    if (list->compare(element, node_data) == 0) {
      alist_remove_node(list, iterator.curr_node,
                        iterator.next_node_pos - iterator.curr_node->next);
      return TRUE;
    }
  }

  return FALSE;
}

void *alist_get_element(alist_t *list, unsigned int element) {
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
  if (list->destroy_on_remove && list->destroy) {

    // Go through the list and destroy each element
    for (unsigned int i = 0; i < list->size; i++) {

      // Get a reference to and destroy the current element in the list
      alist_node_t *node =
          array_get_element(list->list_start, i, list->block_size);
      list->destroy(&node[ALIST_ELEMENT]);
    }
  }

  destroy(list->list_start);

  // Reset the list, meaning if someone tries to use it again
  // it doesnt break anything
  list->capacity = 0;
  list->first = ALIST_NULL;
  list->last = ALIST_NULL;
  list->list_start = NULL;
}
