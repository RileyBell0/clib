#include "../nlist.h"

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

nlist new_nlist(uint32_t chunkLen, uint32_t elementSize)
{
    nlist newList = safe_calloc(SIZEOF_INT32*NLIST_NUM_UINT32 + NLIST_NUM_POINTER * sizeof(void*));
    
    *nlist_get_chunklen(newList) = chunkLen;
    *nlist_get_elementSize(newList) = elementSize;    

    return newList;
}

nlist_node new_nlist_node(nlist list)
{
    return (nlist_node)safe_calloc(SIZEOF_INT32 + sizeof(void*) * NLIST_NODE_NUM_POINTER + (*nlist_get_chunklen(list) * *nlist_get_elementSize(list)));
}

void nlist_append(nlist list, void* data)
{
    if (!list || !data)
    {
        return;
    }

    uint32_t elementSize = *nlist_get_elementSize(list);
    nlist_node node = *nlist_get_last(list);
    
    if (!node || *nlist_get_chunklen(list) == *nlist_node_get_len(node))
    {
        nlist_node newNode = new_nlist_node(list);

        // Updating node references
        *nlist_node_get_prev(newNode) = node;

        // If there is a previous node
        if (node)
        {
            // Set the next value
            *nlist_node_get_next(node) = newNode;
        }

        // If this is the first node in the list
        if (!node)
        {
            *nlist_get_first(list) = newNode;
        }

        // Updating the last node in the list
        *nlist_get_last(list) = newNode;

        
        node = newNode;
    }

    void* arr = nlist_node_get_array(node);
    
    array_set_element(arr, data, (*nlist_node_get_len(node)), elementSize);

    *nlist_node_get_len(node) += 1;
    *nlist_get_size(list) += 1;
}

void nlist_prepend(nlist list, void* data)
{
    if (!list || !data)
    {
        return;
    }
    
    uint32_t elementSize = *nlist_get_elementSize(list);
    nlist_node node = *nlist_get_first(list);

    if (!node)
    {
        nlist_node newNode = new_nlist_node(list);
        *nlist_get_first(list) = newNode;
        *nlist_get_last(list) = newNode;
        node = newNode;
    }
    else if (*nlist_node_get_len(node) == *nlist_get_chunklen(list))
    {
        nlist_node newNode = new_nlist_node(list);
        *nlist_get_first(list) = newNode;
        *nlist_node_get_next(newNode) = node;
        *nlist_node_get_prev(node) = newNode;
        node = newNode;
    }
    
    void* array = nlist_node_get_array(node);

    // We're now at a point where the node pointed to by 'node' contains an empty space
    for (uint32_t i  = *nlist_node_get_len(node); i >= 0; i--)
    {
        // Shift the current element up one space
        array_set_element(array, array_get_element(array, i, elementSize), i, elementSize);
    }
    
    // Putting the data in the first space
    array_set_element(array, data, 0, elementSize);

    // Updating the length of the node
    *nlist_node_get_len(node) += 1;
    *nlist_get_size(list) += 1;
}

void nlist_node_remove_element_at(nlist list, nlist_node node, uint32_t element)
{
    uint32_t length = *nlist_node_get_len(node);
    uint32_t elementSize = *nlist_get_elementSize(list);
    void* array = nlist_node_get_array(node);

    // Drag everything back in the list one position to fill the empty gap
    for (int i = element; i < length - 1; i++)
    {
        array_set_element(array, array_get_element(array, i + 1, elementSize), i, elementSize);
    }

    *nlist_node_get_len(node) -= 1;
    *nlist_get_size(list) -= 1;
}

int nlist_node_remove_element(nlist list, nlist_node node, void* data)
{
    uint32_t length = *nlist_node_get_len(node);
    uint32_t elementSize = *nlist_get_elementSize(list);
    void* array = nlist_node_get_array(node);

    for (uint32_t i = 0; i < length; i++)
    {
        // if we've found the element
        if (memcmp(array_get_element(array, i, elementSize), data, elementSize) == 0)
        {
            nlist_node_remove_element_at(list, node, i);
            return TRUE;
        }
    }

    return FALSE;
}

int nlist_remove_element(nlist list, nlist_node node, void* data)
{
    nlist_node node;

    // For all nodes in the list
    while (node)
    {
        // If we've found a matching node
        if (nlist_node_remove_element(list,node,data))
        {
            return TRUE;
        }

        // Go to the next one
        node = *nlist_node_get_next(node);
    }

    // Should not be able to reach here
    return FALSE;
}

int nlist_remove_element_at(nlist list, nlist_node node, uint32_t element)
{
    uint32_t size = *nlist_get_size(list);
    if (element > size)
    {
        return FALSE;
    }

    nlist_node node;
    nlist_node* (*iterator)(nlist_node list);

    if (element > size/2)
    {
        iterator = nlist_node_get_prev;
        node = *nlist_get_last(list);
    }
    else
    {
        iterator = nlist_node_get_next;
        node = *nlist_get_first(list);
    }

    uint32_t total = 0;
    while (node)
    {
        uint32_t len = *nlist_node_get_len(node);

        if (total + len <= element)
        {
            total += len;
            nlist_node_remove_element_at(list, node, total - element);
            return TRUE;
        }

        total += len;

        node = *iterator(node);
    }

    // Should not be able to reach here
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

// Getters -- NODE
uint32_t* nlist_node_get_len(nlist_node node)
{
    return (uint32_t*)node;
}
nlist_node* nlist_node_get_next(nlist_node node)
{
    return (nlist_node*)((char*)node + SIZEOF_INT32);
}
nlist_node* nlist_node_get_prev(nlist_node node)
{
    return (nlist_node*)((char*)node + SIZEOF_INT32 + sizeof(nlist_node*));
}
void* nlist_node_get_array(nlist_node node)
{
    return (void*)((char*)node + SIZEOF_INT32 + sizeof(nlist_node*) * 2);
}

// Getters -- LIST
uint32_t* nlist_get_size(nlist list)
{
    return (uint32_t*)(list);
}
uint32_t* nlist_get_chunklen(nlist list)
{
    return (uint32_t*)((char*)list + OFFSET_CHUNKLEN);
}
uint32_t* nlist_get_elementSize(nlist list)
{
    return (uint32_t*)((char*)list + OFFSET_ELEMENT_SIZE);
}
nlist_node* nlist_get_last(nlist list)
{
    return (nlist_node*)((char*)list + OFFSET_LAST);
}
nlist_node* nlist_get_first(nlist list)
{
    return (nlist_node*)((char*)list + OFFSET_LAST + sizeof(nlist_node));
}

// void* get_element(void* arr, int elementSize, int element)
// {
//     (char*)arr + elementSize * element;
// }