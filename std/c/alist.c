#include "../alist.h"

// alist_relative_ptr = int32_t dest - int32_t src

alist_t new_alist(size_t element_size) {
  alist_t list;

  list.list_start = NULL;
  list.ledger = NULL;
  list.fast_index = TRUE;
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

void alist_extend(alist_t *list) {
  // Calculate the new length, realloc factor set to be 1.5x
  int32_t new_len = list->capacity + list->capacity / 2;

  // Increment to avoid new_len getting stuck at 0 or 1
  if (new_len <= 1) {
    ++new_len;
  }

  if (list->fast_index){
    // Increase the ledger size to match
    safe_realloc(list->ledger, new_len * sizeof(int32_t));
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


#ifdef CLIB_STD_ALIST_COMPLEMENT
void alist_fix_ledger(alist_t *list, int32_t removed_node) {
  int32_t orig = list->ledger_complement[removed_node];

  // TODO should only be +1 if the removal has already occured
  list->ledger_complement[removed_node] = list->ledger_complement[list->size + 1];

  for (int32_t i = 0; i < list->size; i++)
  {
    if (list->ledger_complement[i] > orig)
    {
      list->ledger_complement[i] -= 1;
    }
  }

  // Create the ledger from the complementary list
  for (int32_t i = 0; i < list->size; i++)
  {
    list->ledger[list->ledger_complement[i]] = i;
  }
}
#else
void alist_fix_ledger(alist_t *list, int32_t removed_node) {
  uint32_t elemsize = alist_node_size(list);
  int32_t curr = list->first;
  int32_t curr_node = 0;

  while (curr >= 0) {
    alist_node_t *node = array_get_element(list->list_start, curr, elemsize);
    list->ledger[curr_node] = curr;
    curr += node->next;
    ++curr_node;
  }
}
#endif

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

  if (list->fast_index) {
    // Record the location of the new node in the ledger
    ((int32_t*)list->ledger)[list->size] = list->size;
  }

  // Add new entry to the ledger for the new node if fast indexing enabled
  if (list->fast_index) {
#ifdef CLIB_STD_ALIST_COMPLEMENT
    list->ledger_complement[list->size] = list->size;
#endif
    list->ledger[list->size] = list->size;
  }

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

  if (list->fast_index) {
    /*
     * Fix the ledger information
    */


  }
}

int alist_remove(alist_t *list, void *element) {
  uint32_t elemsize = alist_node_size(list);
  int32_t curr = list->first;

  // Find the first occurance of the given element in the list and remove it
  while (curr >= 0) {

    // Get the address of the current list element
    alist_node_t *node = array_get_element(list->list_start, curr, elemsize);

    // Check if it matches the given element, if so, remove it
    if (list->compare(element, &node[ALIST_ELEMENT]) == 0) {
      alist_remove_node(list, node, curr);
      
      // Fix the ledger
      if (list->fast_index) {
        alist_fix_ledger(list, curr);
      }

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
