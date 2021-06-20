#include "../alist.h"

// NOTE - for fixing code
// alist_relative_ptr = int dest - int src

/*
 * TODO
 * since the location of elements and nodes within an alist are not guaranteed
 * we should instead have two methods:
 * 
 * alist_view(list, index) which just shows you whats currently in that position
 * which is great and quick and wouldnt allocate memory
 * 
 * alist_get(list, index) which gets and returns new memory containing the element
 * in the given position
*/

/*
 * Iterator Functions
 */
bool alist_iterator_done(alist_iterator_t *iterator);
void *alist_iterator_first_node(alist_iterator_t *iterator);
void *alist_iterator_next_node(alist_iterator_t *iterator);
void *alist_iterator_prev_node(alist_iterator_t *iterator);

int alist_relative_distance(int source, int dest);
void alist_join_nodes(alist_t* list, int left_node, int right_node);
/*
 * Extends the alist by the extension factor to make space for mode data
 */
void alist_extend(alist_t *list);

/*
 * Given a node in an alist, removes it
 */
void alist_remove_node(alist_t *list, alist_node_t *node, int curr);

/*
 * Returns a pointer to the node at the given index in the alist
 */
alist_node_t *alist_get_node(alist_t *list, int element);

//////////////////////////////
// Initialisation
//////////////////////////////

alist_t new_alist(size_t element_size) {
  alist_t list;

  list.list_start = NULL;
  list.destroy_on_remove = true;
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

alist_iterator_t new_alist_iterator(alist_t *list, bool from_start) {
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

//////////////////////////////
// Iterator
//////////////////////////////

bool alist_iterator_done(alist_iterator_t *it) {
  return it->curr_node_pos < 0;
}

void *alist_iterator_first_node(alist_iterator_t *it) {
  // Get the index of the starting node
  if (it->from_start) {
    it->next_node_pos = it->list->first;
  } else {
    it->next_node_pos = it->list->last;
  }

  // Get the first element
  void *element = it->next(it);

  // Get the index of the starting node
  if (it->from_start) {
    it->index = 0;
  } else {
    it->index = it->list->size - 1;
  }

  return element;
}

void *alist_iterator_next_node(alist_iterator_t *it) {
  // Moving to the next element
  it->curr_node_pos = it->next_node_pos;

  if (it->next_node_pos < 0) {
    return NULL;
  }

  it->curr_node = alist_get_node(it->list, it->next_node_pos);
  it->next_node_pos += it->curr_node->next;
  it->element = (void *)&it->curr_node[ALIST_ELEMENT];

  it->index += 1;
  // Returns non-null if not done
  return it->element;
}

void *alist_iterator_prev_node(alist_iterator_t *it) {
  // Moving to the next element
  it->curr_node_pos = it->next_node_pos;

  if (it->next_node_pos < 0) {
    return NULL;
  }

  it->curr_node = alist_get_node(it->list, it->next_node_pos);
  it->next_node_pos += it->curr_node->prev;
  it->element = (void *)&it->curr_node[ALIST_ELEMENT];

  it->index -= 1;
  // Returns non-null if not done
  return it->element;
}

//////////////////////////////
// Basic Operations
//////////////////////////////

void* alist_copy_element(alist_t *list, int index) {
  void* elem = alist_get(list, index);

  // Copy the element into new memory
  void* elem_copy = safe_malloc(list->element_size);
  assert(memcpy(elem_copy, elem, list->element_size));

  return elem_copy;
}

int alist_relative_distance(int source, int dest) {
  return dest - source;
}

void alist_join_nodes(alist_t* list, int left_node, int right_node) {
  int distance = alist_relative_distance(left_node, right_node);
  alist_get_node(list, left_node)->next = distance;
  alist_get_node(list, right_node)->prev = -distance;
}

void *alist_get(alist_t *list, int index) {
  index = index_convert_negative_safe(list->size, index);

  // Create the iterator
  alist_iterator_t it;
  if (index <= list->size / 2) {
    it = new_alist_iterator(list, true);
  } else {
    it = new_alist_iterator(list, false);
  }

  // return a pointer to the element at the given index
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    if (it.index == index) {
      return it.element;
    }
  }

  exit_error("Invalid list structure detected", "std/c/list.c", "alist_get");
  return NULL; // Will not run, exit_error calls exit(EXIT_FAILURE)
}

void alist_append(alist_t *list, void *data) {
  if (list->capacity <= list->size) {
    alist_extend(list);
  }

  alist_node_t *node = alist_get_node(list, list->size);
  if (list->size != 0) {
    alist_join_nodes(list, list->last, list->size);
  } else {
    node->prev = ALIST_NULL;
    list->first = 0;
  }

  list->last = list->size;
  node->next = ALIST_NULL;

  // Copy the data in and Ensure the copy succeeds
  assert(memcpy(&node[ALIST_ELEMENT], data, list->element_size));

  list->size += 1;
}

void alist_append_multi(alist_t *list, void *data, ...) {
  va_list args;
  va_start(args, data);

  void *element = data;

  while (element) {
    alist_append(list, element);
    element = va_arg(args, void *);
  }

  va_end(args);
}

void *alist_pop(alist_t *list, int index) {
  index = index_convert_negative_safe(list->size, index);

  alist_node_t* node = alist_get_node(list, index);

  // Copy the data to new memory
  void* data = safe_malloc(list->element_size);
  assert(memcpy(data, &node[ALIST_ELEMENT], list->element_size));

  // Remove the node from the array
  alist_remove_node(list, node, index);

  return data;
}

void alist_remove_at(alist_t *list, int index) {
  index = index_convert_negative_safe(list->size, index);

  bool from_start = index_closer_to_start(list->size, index);
  alist_iterator_t it = new_alist_iterator(list, from_start);

  for (it.first(&it); !it.done(&it); it.next(&it)) {
    if (it.index == index) {
      alist_remove_node(list, it.curr_node, it.curr_node_pos);
      return;
    }
  }

  exit_error("Invalid alist structure detected", "std/c/alist.c",
             "alist_remove_at");
}

bool alist_remove(alist_t *list, void *element) {
  // Find the first occurance of the given element in the list and remove it
  alist_iterator_t it = new_alist_iterator(list, true);
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    if (list->compare) {
      if (list->compare(element, it.element) == 0) {
        alist_remove_node(list, it.curr_node, it.curr_node_pos);
        return true;
      }
    }
    else {
      if (memcmp(element, it.element, list->element_size) == 0) {
        alist_remove_node(list, it.curr_node, it.curr_node_pos);
        return true;
      }
    }
  }
  
  // Could not find the element in the alist
  return false;
}

void alist_clear(alist_t *list) {
  if (list->destroy && list->destroy_on_remove) {
    alist_iterator_t it = new_alist_iterator(list, true);
    for (it.first(&it); !it.done(&it); it.next(&it)) {
      list->destroy(it.element);
    }
  }

  list->size = 0;
  list->first = ALIST_NULL;
  list->last = ALIST_NULL;
}

//////////////////////////////
// High-level functions
//////////////////////////////

alist_t *alist_combine(alist_t *list1, alist_t *list2) {
  // Dealing with if either of the lists are empty
  if (!list1 || list1->size == 0) {
    if (list2 && list2->size != 0) {
      alist_t combined = alist_copy(list2);
      *list1 = combined;
    }
    return list1;
  }
  else if (!list2 || list2->size == 0) {
    return list1;
  }

  // Ensure lists can theoretically be combined
  unsigned int combined_size = list1->size + list2->size;
  if (combined_size >= INT32_MAX) {
    exit_error("Combined alist size exceeds max alist size", "std/c/alist.c",
               "alist_combine");
  }
  int size = list1->size + list2->size;

  // Allocate enough space to store the combined lists
  if (size > list1->capacity) {
    alist_set_length(list1, size);
  }

  ///////////////////////////////////////////////////////////////////////
  // Code below should only run if both lists have at least one element
  ///////////////////////////////////////////////////////////////////////
  
  // Copy data from list 2 to the end of list 1
  alist_node_t *list1_end = alist_get_node(list1, list1->size);
  assert(memcpy(list1_end, list2->list_start, list2->block_size * list2->size));

  // Patch references between list 1 and the copied data from list 2
  alist_join_nodes(list1, list1->last, list1->size + list2->first);

  // Update overall list properties
  list1->last = list1->size + list2->last;
  list1->size = size;

  return list1;
}

alist_t alist_copy(alist_t *list) {
  // The list being copied is empty, so return an empty list of the same
  // element size
  if (list->size == 0) {
    alist_t empty_list = new_alist(list->element_size);
    return empty_list;
  }

  // Copy the data from list into new_list, except capacity which will be the
  // size of list
  alist_t new_list = *list;
  new_list.capacity = list->size;
  new_list.list_start = safe_malloc(list->block_size * list->size);
  assert(memcpy(new_list.list_start, list->list_start,
                list->block_size * list->size));

  return new_list;
}

array_t alist_to_array(alist_t *list) {
  array_t array = new_array(list->size, list->element_size);

  // Populate the array
  alist_iterator_t it = new_alist_iterator(list, true);
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    array_set_index(&array, it.index, it.element);
  }

  return array;
}

//////////////////////////////
// Utility Functions
//////////////////////////////

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

void alist_remove_node(alist_t *list, alist_node_t *node, int curr) {
  // Delete the data from the node being removed
  alist_node_t *moved_node = alist_get_node(list, curr);
  if (list->destroy_on_remove && list->destroy) {
    list->destroy(&moved_node[ALIST_ELEMENT]);
  }
  
  // Calculate the positions of the surrounding nodes
  int list_final = list->size - 1;
  int next = curr + node->next;
  int prev = curr + node->prev;

  if (next <= -list->size) {
    next = ALIST_NULL; 
  }
  if (prev <= -list->size) {
    prev = ALIST_NULL;
  }

  // Join the nodes surrounding the removed node together
  if (prev != ALIST_NULL && next != ALIST_NULL) {
    alist_join_nodes(list, prev, next);
  }
  else if (prev != ALIST_NULL) {
    alist_get_node(list, prev)->next = ALIST_NULL;
  }
  else if (next != ALIST_NULL) {
    alist_get_node(list, next)->prev = ALIST_NULL;
  }
  else {
    list->first = ALIST_NULL;
    list->last = ALIST_NULL;
    list->size = 0;
    return;
  }

  // Fix overall list references
  if (list->first == curr) {
    list->first = next;
  }
  if (list->last == curr) {
    list->last = prev;
  }

  // If the current node wasn't the in the last filled position in the array,
  // move the last node in the array to fill its place
  if (curr != list_final) {

    // Get the last (array[-1]) element in the array
    alist_node_t *base_node = alist_get_node(list, list_final);

    int base_next_exact = list_final + base_node->next;
    int base_prev_exact = list_final + base_node->prev;
    if (base_next_exact <= -list->size) {
      base_next_exact = ALIST_NULL;
    }
    if (base_prev_exact <= -list->size) {
      base_prev_exact = ALIST_NULL;
    }

    // Fill new empty spot in the array with the node "base_node"
    array_set_element(list->list_start, base_node, curr, list->block_size);

    // Update the pointers of the moved node to match its new position
    if (base_next_exact != ALIST_NULL) {
      moved_node->next = alist_relative_distance(curr, base_next_exact);
    }
    if (base_prev_exact != ALIST_NULL) {
      moved_node->prev = alist_relative_distance(curr, base_prev_exact);
    }

    // Fix the surrounding node pointers
    if (base_prev_exact != ALIST_NULL) {
      alist_join_nodes(list, base_prev_exact, curr);
    }
    if (base_next_exact != ALIST_NULL) {
      alist_join_nodes(list, curr, base_next_exact);
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

alist_node_t *alist_get_node(alist_t *list, int element) {
  return (alist_node_t *)array_get_element(list->list_start, element,
                                           list->block_size);
}

//////////////////////////////
// Cleanup
//////////////////////////////

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
