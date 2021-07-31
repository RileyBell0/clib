#include "../list.h"

// TODO as of 15/07/2021, need tyo look over this entire file to make sure
// nothing is broken here

//////////////////////////////
// Function Declarations
//////////////////////////////

void *list_node_get_data(void *node);

/*
 * Returns a pointer to a new list Node with the relevant data attached
 */
list_node_t *list_new_node(void *element, size_t element_size);

/*
 * Removes the given node from the list, updates list size and pointers
 * accordingly
 */
void list_remove_node(list_t *list, list_node_t *node, void* dest);

/*
 * List iterator functions
 */
/*
 * Move the iterator to the next position in the list based on the direction
 * defined by it.from_start
 *
 * Returns a pointer to the element
 */
void *list_iterator_next(list_iterator_t *it);

/*
 * Move the iterator to the start position in the list based on the direction
 * defined by it.from_start
 *
 * Returns a pointer to the first element
 */
void *list_iterator_first(list_iterator_t *it);

/*
 * If there is a node after this current one bvased on the list direction,
 * returns true, false otherwise
 */
bool list_iterator_done(list_iterator_t *it);

//////////////////////////////
// Initialisation
//////////////////////////////

list_t list_new(size_t element_size) {

  list_t list_new = {0};

  list_new.element_size = element_size;

  return list_new;
}

list_node_t *list_new_node(void *element, size_t element_size) {
  // Ensure the element is non-null
  if (!element) {
    exit_error("Null value recieved creating list node", "std/c/list.c",
               "list_new_node");
  }

  // Allocate memory for the node
  list_node_t *node = safe_calloc(sizeof(list_node_t) + element_size);
  node->next = NULL;
  node->prev = NULL;

  // Copy the data into the generated space
  assert(memcpy(&node[LIST_ELEMENT], element, element_size));

  return node;
}

list_iterator_t list_new_iterator(list_t *list, bool from_start) {
  list_iterator_t it;

  it.list = list;
  it.from_start = from_start;
  it.index = NO_INDEX;
  it.curr_node = NULL;
  it.element = NULL;

  if (from_start) {
    it.next_node = it.list->first_node;
  } else {
    it.next_node = it.list->last_node;
  }

  it.first = list_iterator_first;
  it.next = list_iterator_next;
  it.done = list_iterator_done;

  return it;
}

void *list_iterator_first(list_iterator_t *it) {
  if (it->from_start) {
    it->curr_node = it->list->first_node;
    it->index = 0;
    it->next_node = it->curr_node->next;
  } else {
    it->curr_node = it->list->last_node;
    it->index = it->list->size - 1;
    it->next_node = it->curr_node->prev;
  }
  it->element = &it->curr_node[LIST_ELEMENT];

  return it->element;
}

void *list_iterator_next(list_iterator_t *it) {
  it->curr_node = it->next_node;

  // Update index and next node based on direction
  if (it->from_start) {
    it->next_node = it->curr_node->next;
    it->index += 1;
  } else {
    it->next_node = it->curr_node->prev;
    it->index -= 1;
  }

  it->element = &it->curr_node[LIST_ELEMENT];

  return it->element;
}

bool list_iterator_done(list_iterator_t *it) {
  if (it->next_node == NULL) {
    return true;
  }
  return false;
}

//////////////////////////////
// Basic Operations
//////////////////////////////

void *list_get(list_t *list, int index) {
  // Convert negative index into a positive index
  index = index_convert_negative_safe(list->size, index);

  // Determine which end of the list is closer to the index
  bool from_start = index_closer_to_start(list->size, index);

  // Return the element at the given index within the list
  list_iterator_t it = list_new_iterator(list, from_start);
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    if (it.index == index) {
      return it.element;
    }
  }

  exit_error("List structure invalid - requested index not present",
             "std/c/list.c", "list_get");
  return NULL;
}

list_t *list_append(list_t *list, void *data) {
  list_node_t *new_node = list_new_node(data, list->element_size);

  // patching references to add to end of list
  if (list->size == 0) {
    list->last_node = new_node;
    list->first_node = new_node;
  } else {
    list->last_node->next = new_node;
    new_node->prev = list->last_node;
    list->last_node = new_node;
  }

  list->size += 1;

  return list;
}

list_t *list_append_multi(list_t *list, void *toAppend, ...) {
  va_list args;
  va_start(args, toAppend);

  void *element = toAppend;

  while (element) {
    list_append(list, element);
    element = va_arg(args, void *);
  }

  va_end(args);

  return list;
}

void list_pop(list_t *list, void* dest, int index) {
  index = index_convert_negative_safe(list->size, index);

  // Determine which end of the list is closer to the index
  bool from_start = index_closer_to_start(list->size, index);

  // Find the node at the index requested and remove it
  list_iterator_t it = list_new_iterator(list, from_start);
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    if (index == it.index) {
      list_remove_node(list, it.curr_node, dest);
      return;
    }
  }

  exit_error("List structure invalid - requested index not present",
             "std/c/list.c", "list_pop");
}

void list_remove_at(list_t *list, int index) {
  index = index_convert_negative_safe(list->size, index);

  // Determine which end of the list is closer to the index
  bool from_start = index_closer_to_start(list->size, index);

  // Find the node at the index requested and remove it
  list_iterator_t it = list_new_iterator(list, from_start);
  for (it.first(&it); !it.done(&it); it.next(&it)) {
    if (index == it.index) {
      list_remove_node(list, it.curr_node, NULL);
      return;
    }
  }

  exit_error("List structure invalid - requested index not present",
             "std/c/list.c", "list_remove_at");
}

bool list_remove(list_t *list, void *elem) {
  if (!list || !elem || list->size == 0) {
    return false;
  }

  list_node_t *current_node = list->first_node;

  while (current_node) {
    if (memcmp(&current_node[LIST_ELEMENT], elem, list->element_size) == 0) {
      list_remove_node(list, current_node, NULL);
      return true;
    }

    current_node = current_node->next;
  }

  return false;
}

//////////////////////////////
// Utility
//////////////////////////////

void *list_node_get_data(void *node) {
  return (void *)&((list_node_t *)node)[LIST_ELEMENT];
}

/*
 * Removes a given node from the list it is contained within and
 * patches surrounding references. If a delete_data function is
 * provided, the node's associated data is deleted
 */
void list_remove_node(list_t *list, list_node_t *node, void* dest) {
  list_node_t *prev = node->prev;
  list_node_t *next = node->next;

  // list gymnastics
  list->size -= 1;

  if (prev) {
    prev->next = next;
  } else {
    list->first_node = next;
  }

  if (next) {
    next->prev = prev;
  } else {
    list->last_node = prev;
  }

  // Copy the data out of the node so it can be returned
  if (dest) {
    assert(memcpy(dest, &node[LIST_ELEMENT], list->element_size));
  }

  free(node);
}

//////////////////////////////
// High-level functions
//////////////////////////////

list_t *list_combine(list_t *base, list_t *extension) {
  // TODO fix this fucntion for new data structure in list_node_t
  if (extension->size == 0) {
    return base;
  }

  if (base->size == 0) {
    base->first_node = extension->first_node;
    base->last_node = extension->last_node;
  } else {
    // Patch endings
    base->last_node->next = extension->first_node;
    extension->first_node->prev = base->last_node->next;
  }

  // Update the last element of the list
  base->last_node = extension->last_node;

  // Update the new list size
  base->size = base->size + extension->size;

  // Empty extension list
  list_clear(extension);

  return base;
}

array_t list_to_array(list_t *list) {
  array_t converted = array_new(list->size, list->element_size);

  list_iterator_t it = list_new_iterator(list, true);
  for (void *elem = it.first(&it); !it.done(&it); elem = it.next(&it)) {
    void *data = list_node_get_data(elem);
    array_generic_set(&converted, data, it.index, list->element_size);
  }

  return converted;
}

void list_clear(list_t *list) { list_destroy(list); }

//////////////////////////////
// Cleanup
//////////////////////////////

/*
 * If you have dynamically allocated data, you must send
 * a 'delete_data' function which will be responsible
 * for destroying your dynamically allocated data
 */
void list_destroy(list_t *list) {
  if (list->size == 0) {
    return;
  }

  list_iterator_t it = list_new_iterator(list, true);
  if (list->delete_data) {
    for (void *node = it.first(&it); !it.done(&it); node = it.next(&it)) {
      void *data = list_node_get_data(node);
      list->delete_data(data);
    }
  }

  // Free the entire node
  for (void *node = it.first(&it); !it.done(&it); node = it.next(&it)) {
    free(node);
  }

  list->size = 0;
  list->first_node = NULL;
  list->last_node = NULL;
}
