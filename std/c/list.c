#include "../list.h"

// Returns a pointer to a new list Node with the relevant data attached
static list_node_t *list_new_node(void *data, unsigned int dataSize);

array_t list_to_array(list_t list)
{
    array_t converted = new_array(list.size, list.elementSize);

    unsigned int element = 0;
    list_node_t *node = list.first_node;
    while (node)
    {
        array_set_element(&converted, node->data, element++, list.elementSize);
        node = node->next;
    }

    return converted;
}

list_t new_list(unsigned int elementSize)
{
    list_t new_list = {0};

    new_list.elementSize = elementSize;

    return new_list;
}

static list_node_t *list_new_node(void *data, unsigned int dataSize)
{
    list_node_t *newNode = (list_node_t *)safe_calloc(sizeof(list_node_t));

    // Create space for the data
    newNode->data = safe_malloc(dataSize);

    // The recieved pointer is not-null
    // TODO im not sure how to handle null data just yet
    // for these functions, may have to re-work this later
    // FUTURE RILEY - get on it
    if (data)
    {
        // Copy the data into the generated space
        newNode->data = memcpy(newNode->data, data, dataSize);
    }

    // Ensure the copy succeded
    assert(newNode->data);

    return newNode;
}

list_t *list_append_multi_n(list_t *list, void* toAppend, ...)
{
    va_list args;
    va_start(args, toAppend);

    void* element = toAppend;
    
    while(element)
    {
        list_append(list, element);
        element = va_arg(args, void*);
    }

    va_end(args);

    return list;
}

list_t *list_append(list_t *list, void *data)
{
    if (!list)
    {
        return list;
    }

    list_node_t *new_node = list_new_node(data, list->elementSize);

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

    ++(list->size);

    return list;
}

void *list_remove_first(list_t* list)
{
    if (list->size == 0)
    {
        return NULL;
    }
    return list_remove_at(list, 0);
}

void *list_remove_last(list_t* list)
{
    if (list->size == 0)
    {
        return NULL;
    }
    return list_remove_at(list, list->size-1);
}

// TODO
// should this also chcek if toRemove is non-null?
void *list_remove_element(list_t *list, void *toRemove)
{
    if (!list || list->size == 0)
    {
        return NULL;
    }

    list_node_t *current_node = list->first_node;

    while (current_node)
    {
        if (current_node->data == toRemove)
        {
            return list_remove_node(list, current_node);
        }

        current_node = current_node->next;
    }
    return NULL;
}

/*
 * Removes a given node from the list it is contained within and 
 * patches surrounding references. If a delete_data function is 
 * provided, the node's associated data is deleted
*/
void *list_remove_node(list_t *list, list_node_t *toRemove)
{
    if (list == NULL || toRemove == NULL)
    {
        return NULL;
    }

    list_node_t *prev = toRemove->prev;
    list_node_t *next = toRemove->next;

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

    void *data = toRemove->data;

    free(toRemove);

    return data;
}

void *list_remove_at(list_t *list, unsigned int index)
{
    if (list->size == 0 || index >= list->size)
    {
        return NULL;
    }

    /*
     * Finds and deletes the node at the given index. If the 
     * index is above the mid-point of the list the traversal 
     * to the node begins from the end. otherwise the traversal 
     * begins from the start
    */

    // if above the list midpoint
    if (index > list->size / 2)
    {
        int pos = list->size - 1;
        list_node_t *currentNode = list->last_node;
        while (pos >= index)
        {
            if (pos == index)
            {
                return list_remove_node(list, currentNode);
            }
            currentNode = currentNode->prev;
        }
    }
    // if below or at the list midpoint
    else
    {
        int pos = 0;
        list_node_t *currentNode = list->first_node;
        while (pos <= index)
        {
            if (pos == index)
            {
                return list_remove_node(list, currentNode);
            }
            currentNode = currentNode->next;
        }
    }

    // Through the use of dark magic, the node at the given index could not be found
    return NULL;
}

/*
 * If you have dynamically allocated data, you must send
 * a 'delete_data' function which will be responsible
 * for destroying your dynamically allocated data 
*/
void list_destroy(list_t *list, void (*delete_data)(void *data))
{
    if (list->size <= 0)
    {
        return;
    }

    list_node_t *current_node = list->first_node;
    list_node_t *next_node = NULL;

    while (current_node != NULL)
    {
        next_node = current_node->next;

        // Freeing the dynamically allocated node structure
        if (delete_data)
        {
            delete_data(current_node->data);
            free(current_node->data);
        }
        else
        {
            free(current_node->data);
        }

        free(current_node);

        current_node = next_node;
    }

    list->size = 0;
}

list_t* list_combine(list_t *base, list_t *extension)
{
    if (!base || !extension)
    {
        return base;
    }
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

    // TODO future riley should we even include this code?
    // Encouraging the person using this function to not use the 'extension' list again
    extension->elementSize = 0;
    extension->first_node = NULL;
    extension->last_node = NULL;
    extension->size = 0;

    return base;
}
