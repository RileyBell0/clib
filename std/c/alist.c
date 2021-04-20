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


#ifdef CLIB_STD_ALIST_COMPLEMENTARY
int32_t* alist_generate_complementary_ledger(alist_t* list)
{
  if (list->capacity == 0)
  {
    return NULL;
  }

  int32_t* ledger = safe_malloc(sizeof(int32_t) * list->capacity);

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
#endif

int32_t* alist_generate_ledger(alist_t* list)
{
  if (list->capacity == 0)
  {
    return NULL;
  }

  int32_t* ledger = safe_malloc(sizeof(int32_t) * list->capacity);

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

alist_t *alist_combine(alist_t *list1, alist_t *list2) {
  // Cannot combine two lists of different types
  // Note that this does not check types, only the size of types
  if (list1->element_size != list2->element_size) {
    return NULL;
  }
  if (list2->size == 0) {
    return list1;
  }

  // Allocate enough space to store the combined lists
  if (list1->size + list2->size > list1->capacity) {
    alist_set_length(list1, list1->size + list2->size);
  }

  alist_node_t *list1_end =
      array_get_element(list1->list_start, list1->size, list1->block_size);
  alist_node_t *list2_first = array_get_element(
      list1->list_start, list1->size + list2->first, list1->block_size);
  alist_node_t *list1_last =
      array_get_element(list1->list_start, list1->last, list1->block_size);

  // Copy list2's data across and ensure the copy succeeds
  assert(memcpy(list1_end, list2->list_start, list2->block_size * list2->size));

  // Patch the references between the first element in list2 and the
  // last element in list 1
  list2_first->prev = list1->last - list1->size - list2->first;
  list1_last->next = list1->size + list2->first - list1->last;

  // New last element
  list1->last = list1->size + list2->last;

  // Update the ledgers
  if (list1->fast_index) {
    // Get the ledger information from the second list
    if (list2->fast_index) {
      assert(
          memcpy(list1->ledger, list2->ledger, list2->size * sizeof(int32_t)));

#ifdef CLIB_STD_ALIST_COMPLEMENT
      assert(memcpy(list1->ledger_complement, list2->ledger_complement,
                    list2->size * sizeof(int32_t)));
#endif
    } else {
      // No ledger information existed, generate it
      int32_t* ledger_2 = alist_generate_ledger(list2);
      assert(
          memcpy(list1->ledger, ledger_2, list2->size * sizeof(int32_t)));
      free(ledger_2);

#ifdef CLIB_STD_ALIST_COMPLEMENT
      int32_t* ledger_complementary = alist_generate_complementary_ledger(list2);
      assert(memcpy(list1->ledger_complement, ledger_complementary,
                    list2->size * sizeof(int32_t)));
      free(ledger_complementary);
#endif

    }

      // Update the new ledger information to make it consistent with the
      // new combined list
      for (int32_t i = list1->size; i < list1->size + list2->size; i++) {
        list1->ledger[i] += list1->size;

#ifdef CLIB_STD_ALIST_COMPLEMENT
        list1->ledger_complement[i] += list1->size;
#endif
      }

  }

  // New list size
  list1->size = list1->size + list2->size;

  return list1;
}

void alist_extend(alist_t *list) {
  // Calculate the new length, realloc factor set to be 1.5x
  int32_t new_len = list->capacity + list->capacity / 2;

  // Increment to avoid new_len getting stuck at 0 or 1
  if (new_len <= 1) {
    ++new_len;
  }

  if (list->fast_index) {
    // Increase the ledger size to match
    safe_realloc(list->ledger, new_len * sizeof(int32_t));
#ifdef CLIB_STD_ALIST_COMPLEMENT
    safe_realloc(list->ledger_complement, new_len * sizeof(int32_t));
#endif
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

  if (list->fast_index) {
    // Increase the ledger size to match
    safe_realloc(list->ledger, new_len * sizeof(int32_t));
#ifdef CLIB_STD_ALIST_COMPLEMENT
    safe_realloc(list->ledger_complement, new_len * sizeof(int32_t));
#endif
  }

  // Convert new_len from num elements into num bytes
  size_t new_capacity = (sizeof(alist_node_t) + list->element_size) * new_len;

  // Extend the list's memory and update its capacity
  list->list_start = safe_realloc(list->list_start, new_capacity);
  list->capacity = new_len;
}

#ifdef CLIB_STD_ALIST_COMPLEMENT
void alist_fix_ledger(alist_t *list, int32_t removed_node) {
  int32_t orig = list->ledger_complement[removed_node];

  // TODO should only be +1 if the removal has already occured
  list->ledger_complement[removed_node] =
      list->ledger_complement[list->size + 1];

  for (int32_t i = 0; i < list->size; i++) {
    if (list->ledger_complement[i] > orig) {
      list->ledger_complement[i] -= 1;
    }
  }

  // Create the ledger from the complementary list
  for (int32_t i = 0; i < list->size; i++) {
    list->ledger[list->ledger_complement[i]] = i;
  }
}
#else
void alist_fix_ledger(alist_t *list, int32_t removed_node) {
  size_t elemsize = list->block_size;
  int32_t curr = list->first;

  if (curr != ALIST_NULL) {
    int32_t curr_node = 0;
    while (curr >= 0) {
      alist_node_t *node = array_get_element(list->list_start, curr, elemsize);
      list->ledger[curr_node] = curr;
      curr += node->next;
      ++curr_node;
    }
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
    ((int32_t *)list->ledger)[list->size] = list->size;
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
  if (list->size == 0) {
    return FALSE;
  }

  int32_t curr = list->first;

  // Find the first occurance of the given element in the list and remove it
  while (curr >= 0) {

    // Get the address of the current list element
    alist_node_t *node = array_get_element(list->list_start, curr, list->block_size);

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