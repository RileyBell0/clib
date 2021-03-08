#include "../alist.h"


alist_t new_alist(size_t element_size)
{
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

void alist_extend(alist_t *list)
{
    unsigned int new_len = list->capacity + list->capacity / 2;
    if (new_len <= 1)
    {
        ++new_len;
    }
    list->list_start = safe_realloc(list->list_start, (sizeof(alist_node_t)+list->element_size)*new_len);
    list->capacity = new_len;
}

// TODO replace all instances of this with straight up maths because we dont need a function call for the list.element_size + sizeof(alist_node_t)
size_t alist_node_size(alist_t* list)
{
    return list->element_size + sizeof(alist_node_t);
}

void alist_append(alist_t *list, void* data)
{
    if (list->capacity <= list->size)
    {
        alist_extend(list);
    }

    alist_node_t* newNode = (alist_node_t*)array_get_element(list->list_start, list->size, list->block_size);

    // Patching reference so the new node is appended
    if (list->size) // there is an element in the list which needs to point at this new node
    {
        // Setting the node after the last node to the new node being placed into the list
        ((alist_node_t*)array_get_element(list->list_start, list->last, list->block_size))->next = list->size;
    }
    else
    {
        list->first = 0;
    }

    newNode->prev = list->last;
    list->last = list->size;
    newNode->next = ALIST_NULL;
    
    // Copy the data in and Ensure the copy succeeds
    // Treating each element asthough its only an alist_node_t struct means that going to the 'second' element actually takes us to the 
    // memory directly after the end of the alist_node_t which is the start of our actual element in the list. this refers to &newNode[1]
    assert(memcpy(&newNode[1], data, list->element_size));

    list->size+=1;
}

/*
 * TODO
 * if removing a node so that the list becomes empty, need to make sure that the last and first pointers in the
 * main list struct are set to ALIST_NULL otherwise the appending method will break, as it assumes these values are ALIST_NULL
 * when the list is empty
*/
//  Patch references around the node to be removed, making it disappear from the list
//  
//  if there was a node before the removed node we need to make it point to the node after
//  the removed node. You dont need to do any null checking because if there is no node after
//  then it will be equal to ALIST_NULL already  
// Only once we have fixed the references around the removed node can we move a node from the end
// of the array to fill its spot.  

// Get references to the surrounding nodes for the current node (the node being removed)
// and the references to the surrounding nodes for the last node in the array (the node that
// will replace the current node's position)

void alist_remove_node(alist_t *list, alist_node_t* node, uint32_t curr)
{
    uint32_t next = node->next;
    uint32_t prev = node->prev;
    // Patching references around the removed node      
    if (prev != ALIST_NULL)
    {
        // Get the previous node's address
        ((alist_node_t*)array_get_element(list->list_start, prev, list->block_size))->next = next;
    }

    if (next != ALIST_NULL)
    {
        ((alist_node_t*)array_get_element(list->list_start, next, list->block_size))->prev = prev;
        // Get the next node's address
        // point the node after the removed node at the node before the removed node
    }

    // fix overall list references
    if (list->first == curr)
    {
        // If the first node was removed
        list->first = next;
    }
    if (list->last == curr)
    {
        // If the last node was removed
        list->last = prev;
    }
    
    // If the removed node is not the last node in the list we need to move a node to fill its empty spot
    if (curr != list->size - 1)
    {
        // Get the last element in the array
        alist_node_t *moved_node =  array_get_element(list->list_start, list->size - 1, list->block_size);

        // Moves the last element in the array into the removed element's position
        array_set_element(list->list_start, moved_node, curr, list->block_size);

        // If there was a node before the moved node
        if (moved_node->prev != ALIST_NULL)
        {
            // Get a reference to the node before the node we're moving
            ((alist_node_t*)array_get_element(list->list_start, moved_node->prev, list->block_size))->next = curr;
        }
        if (moved_node->next != ALIST_NULL)
        {
            // Get a reference to the node after the node we're moving
            ((alist_node_t*)array_get_element(list->list_start, moved_node->next, list->block_size))->prev = curr;
        }
    }

    if (list->size - 1 == list->last)
    {
        list->last = curr;
    }
    if (list->size - 1 == list->first)
    {
        list->first = curr;
    }
}

int alist_remove(alist_t* list, void* element)
{
    void* start = list->list_start;

    int (*compare)(const void* elem1, const void* elem2) = list->compare;
    uint32_t elemsize = alist_node_size(list);
    uint32_t curr = list->first;

    while (curr != ALIST_NULL)
    {
        // Get the address of the current list element
        alist_node_t* node = array_get_element(start, curr, elemsize);

        // Have we found the matching element?
        if (compare(element, &node[1]) == 0)
        {
            alist_remove_node(list, node, curr);

            list->size -= 1;

            return TRUE;
        }

        curr = node->next;
    }

    return FALSE;
    
}

alist_node_t* alist_next_node(alist_t* list, alist_node_t* node)
{
    return array_get_element(list->list_start, node->next, alist_node_size(list));
}

void* alist_get_element(alist_t* list, uint32_t element)
{
    alist_node_t *node = (alist_node_t*)array_get_element(list->list_start, element, list->element_size + sizeof(alist_node_t));
    return (void*)&node[1];
}

void alist_destroy(alist_t* list)
{
    if (list->capacity == 0)
    {
        return;
    }

    if (list->destroy)
    {
        for (uint32_t i = 0; i < list->size; i++)
        {
            alist_node_t* node = array_get_element(list->list_start, i, list->block_size);
            
            // Attempt to destroy the data in the node 
            list->destroy(&node[1]);
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
