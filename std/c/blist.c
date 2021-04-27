#include "../blist.h"

// blist_relative_ptr = int32_t dest - int32_t src

blist_t new_blist(size_t element_size) {
  blist_t list;

  list.list_start = NULL;
  list.ledger = NULL;
  list.ledger_complement = NULL;
  list.destroy_on_remove = TRUE;
  list.fast_index = TRUE;
  list.first = BLIST_NULL;
  list.last = BLIST_NULL;
  list.size = 0;
  list.capacity = 0;
  list.element_size = element_size;
  list.block_size = element_size + sizeof(blist_node_t);
  list.compare = NULL;
  list.destroy = NULL;

  return list;
}

void *blist_index(blist_t *list, int32_t index) {
  if (index < 0 || index >= list->size) {
    return NULL;
  }

  return &((blist_node_t*)array_get_element(list->list_start, list->ledger[index],
                           list->block_size))[BLIST_ELEMENT];
}

int32_t *blist_generate_complementary_ledger(blist_t *list) {
  if (list->capacity == 0) {
    return NULL;
  }

  int32_t *ledger = safe_malloc(sizeof(int32_t) * list->capacity);

  int32_t curr = list->first;
  if (curr != BLIST_NULL) {
    int32_t curr_node = 0;
    while (curr >= 0) {
      blist_node_t *node =
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

int32_t *blist_generate_ledger(blist_t *list) {
  if (list->capacity == 0) {
    return NULL;
  }

  int32_t *ledger = safe_malloc(sizeof(int32_t) * list->capacity);

  int32_t curr = list->first;
  if (curr != BLIST_NULL) {
    int32_t curr_node = 0;
    while (curr >= 0) {
      blist_node_t *node =
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
blist_t *blist_combine(blist_t *list1, blist_t *list2) {
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
    blist_set_length(list1, combined_size);
  }

  blist_node_t *list1_end =
      array_get_element(list1->list_start, list1->size, list1->block_size);
  blist_node_t *list2_first = array_get_element(
      list1->list_start, list1->size - 1 + list2->first, list1->block_size);
  blist_node_t *list1_last =
      array_get_element(list1->list_start, list1->last, list1->block_size);

  // Copy list2's data across and ensure the copy succeeds
  assert(memcpy(list1_end, list2->list_start, list2->block_size * list2->size));

  // Patch the references between the first element in list2 and the
  // last element in list 1
  list2_first->prev = list1->last - list1->size - 1 + list2->first;
  list1_last->next = list1->size - 1 + list2->first - list1->last;

  // New last element
  list1->last = list1->size - 1 + list2->last;

  // Update the ledgers
  if (list1->fast_index) {
    // Get the ledger information from the second list
    if (list2->fast_index) {
      assert(
          memcpy(&list1->ledger[list1->size], list2->ledger, list2->size * sizeof(int32_t)));

      assert(memcpy(&list1->ledger_complement[list1->size], list2->ledger_complement,
                    list2->size * sizeof(int32_t)));
    } else {
      // No ledger information existed, generate it
      int32_t *ledger_2 = blist_generate_ledger(list2);
      assert(memcpy(&list1->ledger[list1->size], ledger_2, list2->size * sizeof(int32_t)));
      free(ledger_2);

      int32_t *ledger_complementary =
          blist_generate_complementary_ledger(list2);
      assert(memcpy(&list1->ledger_complement[list1->size], ledger_complementary,
                    list2->size * sizeof(int32_t)));
      free(ledger_complementary);
    }

    // Update the new ledger information to make it consistent with the
    // new combined list
    for (int32_t i = list1->size; i < list1->size + list2->size; i++) {
      list1->ledger[i] += list1->size;

      list1->ledger_complement[i] += list1->size;
    }
  }

  // New list size
  list1->size = list1->size + list2->size;

  return list1;
}

void blist_extend(blist_t *list) {
  // Calculate the new length, realloc factor set to be 1.5x
  int32_t new_len = list->capacity + list->capacity / 2;

  // Increment to avoid new_len getting stuck at 0 or 1
  if (new_len <= 1) {
    ++new_len;
  }

  if (list->fast_index) {
    // Increase the ledger size to match
    list->ledger = safe_realloc(list->ledger, new_len * sizeof(int32_t));
    list->ledger_complement =
        safe_realloc(list->ledger_complement, new_len * sizeof(int32_t));
  }

  // Convert new_len from num elements into num bytes
  size_t new_capacity = (sizeof(blist_node_t) + list->element_size) * new_len;

  // Extend the list's memory and update its capacity
  list->list_start = safe_realloc(list->list_start, new_capacity);
  list->capacity = new_len;
}

void blist_set_length(blist_t *list, int32_t new_len) {
  // Can only increase the list's length
  if (new_len <= list->capacity) {
    return;
  }

  if (list->fast_index) {
    // Increase the ledger size to match
    list->ledger = safe_realloc(list->ledger, new_len * sizeof(int32_t));
    list->ledger_complement =
        safe_realloc(list->ledger_complement, new_len * sizeof(int32_t));
  }

  // Convert new_len from num elements into num bytes
  size_t new_capacity = list->block_size * new_len;

  // Extend the list's memory and update its capacity
  list->list_start = safe_realloc(list->list_start, new_capacity);
  list->capacity = new_len;
}

void blist_fix_ledger(blist_t *list, int32_t removed_node) {
  int32_t orig = list->ledger_complement[removed_node];

  // TODO remember should only be +1 if the removal has already occured
  // this works fine unless you moved where this is called from
  list->ledger_complement[removed_node] = list->ledger_complement[list->size];

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

void blist_append(blist_t *list, void *data) {
  // Make space for the new element
  if (list->capacity <= list->size) {
    blist_extend(list);
  }

  blist_node_t *newNode = (blist_node_t *)array_get_element(
      list->list_start, list->size, list->block_size);

  // Patching reference so the new node is appended
  if (list->size) {
    /*
     * Point the last element in the list to the new appended element
     */
    blist_node_t *last = (blist_node_t *)array_get_element(
        list->list_start, list->last, list->block_size);

    last->next = list->size - list->last;
    newNode->prev = list->last - list->size;
  } else {
    newNode->prev = BLIST_NULL;
    list->first = 0;
  }

  list->last = list->size;
  newNode->next = BLIST_NULL;

  // Copy the data in and Ensure the copy succeeds
  assert(memcpy(&newNode[BLIST_ELEMENT], data, list->element_size));

  if (list->fast_index) {
    // Record the location of the new node in the ledger
    list->ledger[list->size] = list->size;
  }

  // Add new entry to the ledger for the new node if fast indexing enabled
  if (list->fast_index) {
    list->ledger_complement[list->size] = list->size;
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

void blist_remove_node(blist_t *list, blist_node_t *node, uint32_t curr) {
  // Calculate the positions of the surrounding nodes
  int32_t next = curr + node->next; // blist min
  int32_t prev = curr + node->prev; // 1
  int32_t list_final = list->size - 1; // 2

  // Make the nodes around the removed node point at eachother
  if (prev >= 0) {
    blist_node_t *prevnode = ((blist_node_t *)array_get_element(
        list->list_start, prev, list->block_size));
    if (next >= 0) {
      prevnode->next = next - prev;
    } else {
      prevnode->next = BLIST_NULL;
    }

    // Fix overall list references
    if (list->last == curr) {
      list->last = prev;
    }
    else {
      list->last = BLIST_NULL;
    }
  }
  if (next >= 0) {
    blist_node_t *nextnode = ((blist_node_t *)array_get_element(
        list->list_start, next, list->block_size));
    if (prev >= 0) {
      nextnode->prev = prev - next;
    } else {
      nextnode->prev = BLIST_NULL;
    }

    // Fix overall list references
    if (list->first == curr) {
      list->first = next;
    } else {
      list->first = BLIST_NULL;
    }
  }

  blist_node_t *moved_node =
      array_get_element(list->list_start, curr, list->block_size);

  // Delete the data from the node if requested
  if (list->destroy_on_remove && list->destroy) {
    list->destroy(&moved_node[BLIST_ELEMENT]);
  }

  // Fill the empty spot in the array (if one exists)
  if (curr != list_final) {

    // Get the last (array[-1]) element in the array
    int32_t node_orig_pos = list_final;
    blist_node_t *base_node =
        array_get_element(list->list_start, node_orig_pos, list->block_size);

    int32_t base_next = base_node->next;
    int32_t base_prev = base_node->prev;

    // Fill new empty spot in the array with the current final element
    array_set_element(list->list_start, base_node, curr, list->block_size);

    // Update the pointers of the moved node to match its new position
    if (base_next != BLIST_NULL) {
      moved_node->next = list_final + base_next - curr;
    }
    if (base_prev != BLIST_NULL) {

      moved_node->prev = list_final + base_prev - curr;
    }

    // Fix the surrounding node pointers
    if (moved_node->prev != BLIST_NULL) {
      int32_t prev_node_pos = node_orig_pos + base_prev;
      ((blist_node_t *)array_get_element(list->list_start, prev_node_pos,
                                         list->block_size))
          ->next = curr - prev_node_pos;
    }
    if (moved_node->next != BLIST_NULL) {
      int32_t next_node_pos = node_orig_pos + base_next;
      ((blist_node_t *)array_get_element(list->list_start, next_node_pos,
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

  if (list->fast_index) {
    /*
     * Fix the ledger information
     */
    blist_fix_ledger(list, curr);
  }
}

int blist_remove(blist_t *list, void *element) {
  if (list->size == 0) {
    return FALSE;
  }

  int32_t curr = list->first;

  // Find the first occurance of the given element in the list and remove it
  while (curr >= 0) {

    // Get the address of the current list element
    blist_node_t *node =
        array_get_element(list->list_start, curr, list->block_size);

    // Check if it matches the given element, if so, remove it
    if (list->compare(element, &node[BLIST_ELEMENT]) == 0) {
      
      blist_remove_node(list, node, curr);

      return TRUE;
    }

    curr += node->next;
  }

  return FALSE;
}

void *blist_get_element(blist_t *list, uint32_t element) {
  blist_node_t *node = (blist_node_t *)array_get_element(
      list->list_start, element, list->element_size + sizeof(blist_node_t));
  return (void *)&node[BLIST_ELEMENT];
}

void blist_destroy(blist_t *list) {
  // Check if any memory has been allocated for the list
  if (list->capacity == 0) {
    return;
  }

  // Check if there is a method for destroying the elements in the list
  if (list->destroy) {

    // Go through the list and destroy each element
    for (uint32_t i = 0; i < list->size; i++) {

      // Get a reference to and destroy the current element in the list
      blist_node_t *node =
          array_get_element(list->list_start, i, list->block_size);
      list->destroy(&node[BLIST_ELEMENT]);
    }
  }

#ifdef BLIST_SAFE_DESTROY
  // Reset the list, meaning if someone tries to use it again
  // it doesnt break anything
  list->capacity = 0;
  list->first = BLIST_NULL;
  list->last = BLIST_NULL;
#endif

  // Destroy ledger information if it exists
  destroy(list->ledger);
  destroy(list->ledger_complement);

  // Free the memory occupied by the list itself
  free(list->list_start);
  list->list_start = NULL;
}
