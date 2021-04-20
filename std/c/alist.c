#include "../alist.h"

// alist_relative_ptr = int32_t dest - int32_t src

alist_t new_alist(size_t element_size) {
  alist_t list;
  list.capacity = 0;
  list.first = ALIST_NULL;
  list.last = ALIST_NULL;
  list.size = 0;
  list.element_size = element_size;
  list.block_size = element_size + sizeof(alist_node_t);
  list.list_start = NULL;
  list.destroy = NULL;
  return list;
}

void alist_extend(alist_t *list) {
  // Calculate the new length, realloc factor set to be 1.5x
  int32_t new_len = list->capacity + list->capacity / 2;

  // Increment to avoid new_len getting stuck at 0 or 1
  if (new_len <= 1) {
    ++new_len;
  }

  // Convert new_len from num elements into num bytes
  new_len = (sizeof(alist_node_t) + list->element_size) * new_len;

  // Extend the list's memory and update its capacity
  list->list_start = safe_realloc(list->list_start, new_len);
  list->capacity = new_len;
}

// TODO replace all instances of this with straight up maths because we dont
// need a function call for the list.element_size + sizeof(alist_node_t)
size_t alist_node_size(alist_t *list) {
  return list->element_size + sizeof(alist_node_t);
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
 * TODO
 * if removing a node so that the list becomes empty, need to make sure that the
 * last and first pointers in the main list struct are set to ALIST_NULL
 * otherwise the appending method will break, as it assumes these values are
 * ALIST_NULL when the list is empty
 */
//  Patch references around the node to be removed, making it disappear from the
//  list
//
//  if there was a node before the removed node we need to make it point to the
//  node after the removed node. You dont need to do any null checking because
//  if there is no node after then it will be equal to ALIST_NULL already
// Only once we have fixed the references around the removed node can we move a
// node from the end of the array to fill its spot.

// Get references to the surrounding nodes for the current node (the node being
// removed) and the references to the surrounding nodes for the last node in the
// array (the node that will replace the current node's position)

void alist_remove_node(alist_t *list, alist_node_t *node, uint32_t curr) {
  // Calculate the positions of the surrounding nodes
  uint32_t next = curr + node->next;
  uint32_t prev = curr + node->prev;

  // Patching references around the removed node
  if (prev >= 0) {
    ((alist_node_t *)array_get_element(list->list_start, prev,
                                       list->block_size))
        ->next = next;
  }
  if (next >= 0) {
    ((alist_node_t *)array_get_element(list->list_start, next,
                                       list->block_size))
        ->prev = prev;
  }

  // Fix the list's first and last refernces
  if (list->first == curr) {
    list->first = next;
  }
  if (list->last == curr) {
    list->last = prev;
  }

  // Fill the empty spot in the array (if one exists)
  if (curr != list->size - 1) {

    // Get the last (array[-1]) element in the array
    alist_node_t *moved_node =
        array_get_element(list->list_start, list->size - 1, list->block_size);

    // Fill new empty spot in the array with the current final element
    array_set_element(list->list_start, moved_node, curr, list->block_size);

    // Fix the surrounding node pointers
    if (moved_node->prev != ALIST_NULL) {
      ((alist_node_t *)array_get_element(list->list_start, moved_node->prev,
                                         list->block_size))
          ->next = curr;
    }
    if (moved_node->next != ALIST_NULL) {
      ((alist_node_t *)array_get_element(list->list_start, moved_node->next,
                                         list->block_size))
          ->prev = curr;
    }
  }

  // Patch the overall list first/last references to match the new list info
  if (list->size - 1 == list->last) {
    list->last = curr;
  }
  if (list->size - 1 == list->first) {
    list->first = curr;
  }
}

int alist_remove(alist_t *list, void *element) {
  uint32_t elemsize = alist_node_size(list);
  uint32_t curr = list->first;

  // Find the first occurance of the given element in the list and remove it
  while (curr >= 0) {

    // Get the address of the current list element
    alist_node_t *node = array_get_element(list->list_start, curr, elemsize);

    // Check if it matches the given element, if so, remove it
    if (list->compare(element, &node[ALIST_ELEMENT]) == 0) {
      alist_remove_node(list, node, curr);
      list->size -= 1;

      return TRUE;
    }

    curr += node->next;
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
