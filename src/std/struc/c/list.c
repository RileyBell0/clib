#include "../list.h"
void *list_node_get_data(list_node_t *node);
/*
 * Returns a pointer to a new list Node with the relevant data attached
 */
list_node_t *list_new_node(void *element, size_t element_size);
/*
 * Removes the given node from the list, updates list size and pointers
 * accordingly
 */
void list_remove_node(list_t *list, list_node_t *node, void *dest);
bool list_iterator_done(list_iterator_t *it);
void *list_iterator_next(list_iterator_t *it);

list_iterator_t new_list_iterator(list_t list, bool from_start)
{
  list_iterator_t it;
  it.next = list_iterator_next;
  it.done = list_iterator_done;
  it.from_start = from_start;
  it.first = true;
  if (from_start)
  {
    it.node = list.first_node;
    it.index = 0;
  }
  else
  {
    it.node = list.last_node;
    it.index = list.size - 1;
  }
  if (list.size == 0)
  {
    it.node = NULL;
  }
  return it;
}

/*
 * returns true if the list has no more elements
 */
bool list_iterator_done(list_iterator_t *it)
{
  if (it->node == NULL)
  {
    return true;
  }
  else if (it->first)
  {
    return false;
  }

  // If the next node is empty, the list is done
  if (it->from_start)
  {
    return it->node->next == NULL;
  }
  else
  {
    return it->node->prev == NULL;
  }
}

/*
 * Move to (and return) the next elem in the list
 */
void *list_iterator_next(list_iterator_t *it)
{
  it->first = false;
  // Move in the direction of the elem
  if (it->node != NULL)
  {
    if (it->from_start)
    {
      it->node = it->node->next;
      it->index += 1;
    }
    else
    {
      it->node = it->node->prev;
      it->index -= 1;
    }
  }

  return list_node_get_data(it->node);
}

list_t list_new(size_t element_size)
{
  list_t list = {0};

  list.element_size = element_size;

  return list;
}

list_node_t *list_new_node(void *element, size_t element_size)
{
  // Ensure the element is non-null
  if (!element)
  {
    exit_error("Null value recieved creating list node", "std/c/list.c",
               "list_new_node");
  }

  // Allocate memory for the node
  list_node_t *node = safe_calloc(sizeof(list_node_t) + element_size);
  node->next = NULL;
  node->prev = NULL;

  // Copy the data into the generated space
  assert(memcpy(&node[1], element, element_size));

  return node;
}

void *list_get(list_t *list, int index)
{
  // Convert negative index into a positive index
  index = index_convert_negative_safe(list->size, index);

  // Determine which end of the list is closer to the index
  bool from_start = index_closer_to_start(list->size, index);
  list_iterator_t it = new_list_iterator(*list, from_start);
  for (void *elem = it.node; !it.done(&it); elem = it.next(&it))
  {
    // When we get to the required element, return it
    if (it.index == index)
    {
      return elem;
    }
  }

  exit_error("List structure invalid - requested index not present",
             "std/c/list.c", "list_get");
  return NULL;
}

list_t *list_append(list_t *list, void *data)
{
  list_node_t *new_node = list_new_node(data, list->element_size);

  // patching references to add to end of list
  if (list->size == 0)
  {
    list->last_node = new_node;
    list->first_node = new_node;
  }
  else
  {
    list->last_node->next = new_node;
    new_node->prev = list->last_node;
    list->last_node = new_node;
  }

  list->size += 1;

  return list;
}

void list_pop(list_t *list, void *dest, int index)
{
  // Determine which end of the list is closer to the index
  bool from_start = index_closer_to_start(list->size, index);

  // Find the node at the index requested and remove it
  list_iterator_t it = new_list_iterator(*list, from_start);
  for (; !it.done(&it); it.next(&it))
  {
    if (index == it.index)
    {
      // Remove the elem, and save it to "dest"
      return list_remove_node(list, it.node, dest);
    }
  }

  exit_error("List structure invalid - requested index not present",
             "std/c/list.c", "list_pop");
}

void list_remove_at(list_t *list, int index)
{
  // Determine which end of the list is closer to the index
  bool from_start = index_closer_to_start(list->size, index);

  if (index >= list->size)
  {
    exit_error("List Index out of range", "std/c/list.c", "list_remove_at");
  }

  // Find the node at the index requested and remove it
  list_iterator_t it = new_list_iterator(*list, from_start);
  for (; !it.done(&it); it.next(&it))
  {
    if (index == it.index)
    {
      // Remove the elem
      return list_remove_node(list, it.node, NULL);
    }
  }

  exit_error("List structure invalid - requested index not present",
             "std/c/list.c", "list_remove_at");
}

bool list_remove(list_t *list, void *elem)
{
  if (!list || !elem || list->size == 0)
  {
    return false;
  }

  list_node_t *current_node = list->first_node;

  while (current_node)
  {
    if (memcmp(&current_node[1], elem, list->element_size) == 0)
    {
      list_remove_node(list, current_node, NULL);
      return true;
    }

    current_node = current_node->next;
  }

  return false;
}

void *list_node_get_data(list_node_t *node)
{
  if (node == NULL)
  {
    return NULL;
  }
  else
  {
    return (void *)&(node[1]);
  }
}

/*
 * Removes a given node from the list it is contained within and
 * patches surrounding references. If a delete_data function is
 * provided, the node's associated data is deleted
 */
void list_remove_node(list_t *list, list_node_t *node, void *dest)
{
  list_node_t *prev = node->prev;
  list_node_t *next = node->next;

  // list gymnastics
  list->size -= 1;

  if (prev)
  {
    prev->next = next;
  }
  else
  {
    list->first_node = next;
  }

  if (next)
  {
    next->prev = prev;
  }
  else
  {
    list->last_node = prev;
  }

  // Copy the data out of the node so it can be returned
  if (dest)
  {
    assert(memcpy(dest, &node[1], list->element_size));
  }

  free(node);
}

//////////////////////////////
// High-level functions
//////////////////////////////

list_t *list_combine(list_t *base, list_t *extension)
{
  // TODO fix this fucntion for new data structure in list_node_t
  if (extension->size == 0)
  {
    return base;
  }

  if (base->size == 0)
  {
    base->first_node = extension->first_node;
    base->last_node = extension->last_node;
  }
  else
  {
    // Patch endings
    base->last_node->next = extension->first_node;
    extension->first_node->prev = base->last_node->next;
  }

  // Update the last element of the list
  base->last_node = extension->last_node;

  // Update the new list size
  base->size = base->size + extension->size;

  // Clear the extension list
  extension->first_node = NULL;
  extension->last_node = NULL;
  extension->size = 0;

  return base;
}

array_t list_to_array(list_t *list)
{
  array_t converted = array_new(list->size, list->element_size);

  // Copy all elements across
  list_iterator_t it = new_list_iterator(*list, true);
  for (void *elem = it.node; !it.done(&it); elem = it.next(&it))
  {
    array_set(&converted, it.index, elem);
  }

  return converted;
}

//////////////////////////////
// Cleanup
//////////////////////////////

/*
 * If you have dynamically allocated data, you must send
 * a 'delete_data' function which will be responsible
 * for destroying your dynamically allocated data
 */
void list_destroy(list_t *list, void (*delete_data)(void *data))
{
  if (list->size == 0)
  {
    return;
  }

  list_iterator_t it = new_list_iterator(*list, true);
  if (delete_data)
  {
    for (; !it.done(&it); it.next(&it))
    {
      void *data = list_node_get_data(it.node);

      delete_data(data);
    }
  }

  // Free the entire node
  for (; !it.done(&it); it.next(&it))
  {
    free(it.node);
  }

  list->size = 0;
  list->first_node = NULL;
  list->last_node = NULL;
}
