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
void* alist_copy_elem_from_node(alist_t* list, alist_node_t* node);

alist_node_t* alist_get_node_at_pos(alist_t* list, int pos);

/*
 * Iterator Functions
 */
bool alist_iterator_done(alist_iterator_t *iterator);
void *alist_iterator_first_node(alist_iterator_t *iterator);
void *alist_iterator_next_node(alist_iterator_t *iterator);

int alist_relative_distance(int source, int dest);
void alist_join_nodes(alist_t* list, int left_node, int right_node);
/*
 * Extends the alist by the extension factor to make space for mode data
 */
void alist_extend(alist_t *list);

/*
 * Given a node in an alist, removes it
 */
void alist_remove_node(alist_t *list, alist_node_t *node, int pos);

/*
 * Returns a pointer to the node at the given index in the alist
 */
alist_node_t *alist_get_node(alist_t *list, int index);

int alist_next_ext_len(alist_t* list);
void alist_set_len(alist_t* list, int len);
void alist_make_space(alist_t* list, int req_nodes);

//////////////////////////////
// Initialisation
//////////////////////////////

alist_t alist_new(size_t element_size) {
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

alist_iterator_t alist_iterator_new(alist_t *list, bool from_start) {
  alist_iterator_t it;

  it.list = list;
  it.from_start = from_start;
  it.index = ALIST_NULL;
  it.curr_node_pos = ALIST_NULL;

  // Set direction dependent information
  it.next = alist_iterator_next_node;
  it.done = alist_iterator_done;
  it.first = alist_iterator_first_node;

  return it;
}

//////////////////////////////
// Iterator
//////////////////////////////

bool alist_iterator_done(alist_iterator_t *it) {
  return it->curr_node == NULL;
}

void *alist_iterator_first_node(alist_iterator_t *it) {
  // Get the index of the starting node
  int start_node;
  if (it->from_start) {
    it->index = 0;
    start_node = it->list->first;
  } else {
    it->index = it->list->size - 1;
    start_node = it->list->last;
  }
  it->curr_node_pos = start_node;

  // Attach the starting node to the iterator
  if (start_node == ALIST_NULL) {
    it->curr_node = NULL;
    it->element = NULL;
    it->index = ALIST_NULL;
  }
  else {
    it->curr_node = alist_get_node_at_pos(it->list, start_node);
    it->element = (void *)&it->curr_node[ALIST_ELEMENT];
  }

  return it->element;
}

void *alist_iterator_next_node(alist_iterator_t *it) {
  // Update the current node pos
  if (it->from_start) {
    it->curr_node_pos = it->curr_node_pos + it->curr_node->next;
    it->curr_node = alist_get_next_node(it->list, it->curr_node);
    it->index += 1;
  }
  else {
    it->curr_node_pos = it->curr_node_pos + it->curr_node->prev;
    it->curr_node = alist_get_prev_node(it->list, it->curr_node);
    it->index -= 1;
  }

  if (it->curr_node == NULL) {
    return NULL;
  }

  it->element = (void *)&it->curr_node[ALIST_ELEMENT];

  // Returns non-null if not done
  return it->element;
}

//////////////////////////////
// Basic Operations
//////////////////////////////

void* alist_copy_elem_from_node(alist_t* list, alist_node_t* node) {
  // Copy the element into new memory
  void* elem_copy = safe_malloc(list->element_size);
  assert(memcpy(elem_copy, (void *)&node[ALIST_ELEMENT], list->element_size));

  return elem_copy;
}

void* alist_copy_element(alist_t *list, int index) {
  void* elem = alist_get(list, index);

  // Copy the element into new memory
  void* elem_copy = safe_malloc(list->element_size);
  assert(memcpy(elem_copy, elem, list->element_size));

  return elem_copy;
}

void *alist_get(alist_t *list, int index) {
  index = index_convert_negative_safe(list->size, index);

  // Create the iterator
  alist_iterator_t it;
  if (index <= list->size / 2) {
    it = alist_iterator_new(list, true);
  } else {
    it = alist_iterator_new(list, false);
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

void alist_insert_at(alist_t* list, void* data, int index) {
  index = index_convert_negative_safe(list->size + 1, index);
  alist_make_space(list, list->size + 1);
  bool inserted = false;
  
  // Copy the data into a new node
  alist_node_t *node = alist_get_node_at_pos(list, list->size);
  assert(memcpy(&node[ALIST_ELEMENT], data, list->element_size));

  if (index == list->size) {
    // Appending
    node->next = ALIST_NULL;

    if (list->size) {
      alist_join_nodes(list, list->last, list->size);
    }
    else {
      node->prev = ALIST_NULL;
      list->first = list->size;
    }
    list->last = list->size;
    
    inserted = true;
  }
  else if (index == 0) {
    // Prepending
    node->prev = ALIST_NULL;
    
    if (list->size) {
      alist_join_nodes(list, list->size, list->first);
    }
    else {
      node->next = ALIST_NULL;
      list->last = list->size;
    }
    list->first = list->size;

    inserted = true;
  }
  else {
    // Within a list
    bool from_start = index_closer_to_start(list->size, index);
    alist_iterator_t it = alist_iterator_new(list, from_start);
    for (it.first(&it); !it.done(&it); it.next(&it)) {
      if (it.index == index) {
        // There will be a node before this and after this
        alist_node_t* right_node = it.curr_node;
        
        int right_node_pos = it.curr_node_pos;
        int left_node_pos = right_node_pos + right_node->prev;

        alist_join_nodes(list, left_node_pos, list->size);
        alist_join_nodes(list, list->size, right_node_pos);

        inserted = true;
      }
    }
  }

  if (!inserted) {
    exit_error("Index not within alist", "std/c/alist.c", "alist_insert_at");
  }

  list->size += 1;
}

void alist_append(alist_t *list, void *data) {
  alist_insert_at(list, data, list->size);
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

  // Get the node and its pos
  bool from_start = index_closer_to_start(list->size, index);
  alist_iterator_t it = alist_iterator_new(list, from_start);
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    if (it.index == index) {
      break;
    }
  }

  // Copy the data from the node out
  void* elem = alist_copy_elem_from_node(list, it.curr_node);

  // Remove the node from the array
  alist_remove_node(list, it.curr_node, it.curr_node_pos);

  return elem;
}

void alist_remove_at(alist_t *list, int index) {
  index = index_convert_negative_safe(list->size, index);

  // Get the node and its pos
  bool from_start = index_closer_to_start(list->size, index);
  alist_iterator_t it = alist_iterator_new(list, from_start);
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    if (it.index == index) {
      break;
    }
  }

  alist_remove_node(list, it.curr_node, it.curr_node_pos);
}

bool alist_remove(alist_t *list, void *element) {
  // Find the first occurance of the given element in the list and remove it
  alist_iterator_t it = alist_iterator_new(list, true);
  if (list->compare) {
    // if a compare method is provided, use to compare elements
    for (it.first(&it); !it.done(&it); it.next(&it)) {
      if (list->compare(element, it.element) == 0) {
        alist_remove_node(list, it.curr_node, it.curr_node_pos);
        return true;
      }
    }
  }
  else {
    // If no compare method, compare the memory of each element
    for (it.first(&it); !it.done(&it); it.next(&it)) {
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
    alist_iterator_t it = alist_iterator_new(list, true);
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
    alist_make_space(list1, size);
  }

  ///////////////////////////////////////////////////////////////////////
  // Code below should only run if both lists have at least one element
  ///////////////////////////////////////////////////////////////////////
  
  // Copy data from list 2 to the end of list 1
  alist_node_t *list1_end = alist_get_node_at_pos(list1, list1->size);
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
    alist_t empty_list = alist_new(list->element_size);
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
  array_t array = array_new(list->size, list->element_size);

  // Populate the array
  alist_iterator_t it = alist_iterator_new(list, true);
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    array_set(&array, it.index, it.element);
  }

  return array;
}

//////////////////////////////
// Utility Functions
//////////////////////////////

alist_node_t *alist_get_next_node(alist_t *list, alist_node_t *node) {
  if (node->next != ALIST_NULL) {
    return pointer_offset(node, node->next, list->block_size);
  }
  else {
    return NULL;
  }
}

alist_node_t *alist_get_prev_node(alist_t *list, alist_node_t *node) {
  if (node->prev != ALIST_NULL) {
    return pointer_offset(node, node->prev, list->block_size);
  }
  else {
    return NULL;
  }
}

void alist_join_nodes(alist_t* list, int left_node, int right_node) {
  int distance = alist_relative_distance(left_node, right_node);
  alist_get_node_at_pos(list, left_node)->next = distance;
  alist_get_node_at_pos(list, right_node)->prev = -distance;
}

int alist_relative_distance(int source, int dest) {
  return dest - source;
}

// TODO move to relevant space
int alist_next_ext_len(alist_t* list) {
  // Calculate the new length, realloc factor set to be 1.5x
  int new_len = list->capacity + (list->capacity / 2);

  // Increment to avoid new_len getting stuck at 0 or 1
  if (new_len <= 1) {
    ++new_len;
  }

  return new_len;
}

// TODO move to relevanat space
void alist_set_len(alist_t* list, int len) {
  size_t req_size = len * list->block_size;

  // Allocate new space and update the capacity
  list->list_start = safe_realloc(list->list_start, req_size);
  list->capacity = len;
}

// TODO move to relevant space
void alist_make_space(alist_t* list, int req_nodes) {
  if (list->capacity - list->size < req_nodes) {
    int new_len = alist_next_ext_len(list);

    if (new_len - list->capacity < req_nodes) {
      // Extend the tree to fit specifically the new nodes
      alist_set_len(list, list->size + req_nodes);
    }
    else {
      // Extend the tree by the extension factor in atree_next_ext_len
      alist_set_len(list, new_len);
    }
  }
}

alist_node_t* alist_get_node_at_pos(alist_t* list, int pos) {
  return (alist_node_t *)array_generic_get(list->list_start, pos,
                                           list->block_size);
}

void alist_remove_node(alist_t *list, alist_node_t *node, int pos) {
  void* elem_start = (void*)&node[ALIST_ELEMENT];

  bool node_before = false;
  bool node_after = false;

  // Delete the data from the node being removed
  if (list->destroy_on_remove && list->destroy) {
    list->destroy(elem_start);
  }
  
  // Determine whether there are nodes before and/or after the removed node
  if (node->next != ALIST_NULL) {
    node_after = true;
  }
  if (node->prev != ALIST_NULL) {
    node_before = true;
  }

  // Remove the node from the array
  if (node_before && node_after) {
    alist_join_nodes(list, pos + node->prev, pos + node->next);
  }
  else if (node_before) {
    alist_get_prev_node(list, node)->next = ALIST_NULL;
    list->last = pos + node->prev;
  }
  else if (node_after) {
    alist_get_next_node(list, node)->prev = ALIST_NULL;
    list->first = pos + node->next;
  }
  else {
    // This was the last node in the list and it has been removeds
    list->first = ALIST_NULL;
    list->last = ALIST_NULL;
  }

  // If the removed node was not the last one in the array
  int last_node_pos = list->size - 1;
  if (pos != last_node_pos) {
    // Move the last node in the array to fill the spot of the removed node
    int prev_node_pos = ALIST_NULL;
    int next_node_pos = ALIST_NULL;
    alist_node_t* last_node = alist_get_node_at_pos(list, last_node_pos);

    // Determine whether nodes are before and/or after this node
    if (last_node->prev == ALIST_NULL) {
      node_before = false;
    }
    else {
      node_before = true;
      prev_node_pos = last_node_pos + last_node->prev;
    }
    if (last_node->next == ALIST_NULL) {
      node_after = false;
    }
    else {
      node_after = true;
      next_node_pos = last_node_pos + last_node->next;
    }

    // Move the node and update pointers to its new location
    assert(memcpy(elem_start, (void *)&last_node[ALIST_ELEMENT],
                  list->element_size));
    if (node_before && node_after) {
      alist_join_nodes(list, prev_node_pos, pos);
      alist_join_nodes(list, pos, next_node_pos);
    }
    else if (node_before){
      alist_join_nodes(list, prev_node_pos, pos);
      node->next = ALIST_NULL;
      list->last = pos;
    }
    else if (node_after) {
      alist_join_nodes(list, pos, next_node_pos);
      node->prev = ALIST_NULL;
      list->first = pos;
    }
    else {
      list->first = pos;
      list->last = pos;
      node->next = ALIST_NULL;
      node->prev = ALIST_NULL;
    }
  }

  list->size -= 1;
}

alist_node_t *alist_get_node(alist_t *list, int index) {
  index = index_convert_negative_safe(list->size, index);

  bool from_start = index_closer_to_start(list->size, index);
  alist_iterator_t it = alist_iterator_new(list, from_start);
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    if (it.index == index) {
      return (alist_node_t*)it.curr_node;
    }
  }

  exit_error("Invalid alist structure - index not in alist", "std/c/alist.c",
             "alist_get_node");
  return NULL;
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
          array_generic_get(list->list_start, i, list->block_size);
      list->destroy(&node[ALIST_ELEMENT]);
    }
  }

  destroy(list->list_start);
  list->capacity = 0;
  list->size = 0;
  list->first = ALIST_NULL;
  list->last = ALIST_NULL;
  list->list_start = NULL;
}
