#include "../list.h"

// Returns a pointer to a new list Node with the relevant data attached
static list_node *list_new_node(void *data, unsigned int dataSize);

list new_list(unsigned int elementSize)
{
    list new_list = {0};

    new_list.elementSize = elementSize;

    return new_list;
}

static list_node *list_new_node(void *data, unsigned int dataSize)
{
    list_node *newNode = (list_node *)safe_calloc(sizeof(list_node));

    // Create space for the data
    newNode->data = safe_malloc(dataSize);

    // Copy the data into the generated space
    newNode->data = memcpy(newNode->data, data, dataSize);

    // Ensure the copy succeded
    assert(newNode->data);

    return newNode;
}

void list_append(list *list, void *data)
{
    list_node *new_node = list_new_node(data, list->elementSize);

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
}

void *list_remove_element(list *list, void *toRemove)
{
    list_node *current_node = list->first_node;

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
void *list_remove_node(list *list, list_node *toRemove)
{
    if (toRemove == NULL)
    {
        return NULL;
    }

    list_node *prev = toRemove->prev;
    list_node *next = toRemove->next;

    // list gymnastics
    list->size -= 1;

    if (prev != NULL)
    {
        list->first_node = prev;
        prev->next = next;
    }
    else
    {
        list->first_node = next;
    }
    
    if (next != NULL)
    {
        list->last_node = next;
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

void *list_remove_at(list *list, int index)
{
    if (index >= list->size)
    {
        exit(ERROR);
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
        list_node *currentNode = list->last_node;
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
        list_node *currentNode = list->first_node;
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
void list_destroy(list *list, void (*delete_data)(void *data))
{
    if (list->size <= 0)
    {
        return;
    }

    list_node *current_node = list->first_node;
    list_node *next_node = NULL;

    // TODO
    // Method can be shortened for readability by
    // putting the if statement around the line
    //  'delete_data(current_node->data)'
    // but this would make it slightly, every so slightly slower
    if (delete_data)
    {
        // Freeing the Dynamically Allocated Data in the node
        delete_data(current_node->data);
        while (current_node != NULL)
        {
            next_node = current_node->next;

            // Destroying the Data
            delete_data(current_node->data);
            free(current_node->data);
            
            // Freeing the dynamically allocated node structure
            free(current_node);
            
            current_node = next_node;
        }
    }
    else
    {
        // Freeing the Dynamically Allocated Data in the node
        delete_data(current_node->data);
        while (current_node != NULL)
        {
            next_node = current_node->next;

            // Destroying the Data
            free(current_node->data);

            // Freeing the dynamically allocated node structure
            free(current_node);
            
            current_node = next_node;
        }
    }

    list->size = 0;
}