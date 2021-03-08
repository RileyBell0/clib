#include "../list.h"

// TODO
// array_set_element should return a pointer to the start of the modified memory, the start of the 'set' element
/*
 * The alist is a good all-rounder list
 * 
 * Insertion: Really fast (except when the list needs to be realloc'd somehwere)
 *      -> O(1)
 *         good average time complexity O(1), but significantly the space where the new
 *         element is going to be inserted is already allocated alot of the time
 * Deletion: Really fast
 *      -> O(1) 
 *         there is no need to free the memory due to the structure the data being in an array
 *         there is generally (especially in smaller lists) huge advantages in memory locality
 *         due to all elements being in similar places in memory due to being stored in the same chunk
 *         however deleting an element has an added cost in comparison to a linked-list
 *         as the deleted element needs to have the last element in the array copied into its
 *         place and the referenes for that element fixed aswell to point at its new position
 *         
 *         so deleition is still fast, and benefits from memory locality but it has more to do in a deletion
 * Search: Quite fast
 *      -> This is where the memory locality helps out, all list elements are stored together so we get the added
 *         boost of hvaing less cache misses because especially in smaller lists, the entire list will be able
 *         to be stored in the cache, or at least theres a high likely hood that at least some of the next elements
 *         will be in the cache
 *          
 *         i dont actually know how cache works
 * 
 * Drawbacks:
 *      ->  No element's position in memory is guaranteed using this data structure
 *          the list is faster with smaller element sizes due to stuff being frequently
 *          moved around within the list
 * 
 *          so no element's position in memory is guaranteed, and its position relative to the
 *          list's origin in memory is also not guaranteed.
 *          
 *          it's faster at inserting stuff than a list, assuming faster at searching stuff than a list
 *          and when an atree_t structure exists it'll be faster for doing anything sorted-related than using
 *          a list or a tree
 * 
 * from testing, insertion is magnitudes of times faster than in a linked list when all optimisations are turned
 * on in the compiler. but worth noting theres a larger range of performance than when comparing to an array
*/

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

list_t new_list(size_t elementSize)
{
    list_t new_list = {0};

    new_list.elementSize = elementSize;

    return new_list;
}

list_node_t *list_new_node(void *data, size_t dataSize)
{
    list_node_t *newNode = safe_calloc(sizeof(list_node_t));

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
    if (list->size == 0)
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
