/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

/* TODO
 * FUTURE RILEY - fix this
 * 
 * Undefined behaviour when
 * NULL values are recieved for any field
 * that is not a list_t* field
 * 
 * This functionality and checking
 * is yet to be implemented. not sure
 * whether to copy in a null value when
 * null is recieved or just to do nothing
 * and throw an error
*/

#ifndef CLIB_STD_LIST_H
#define CLIB_STD_LIST_H

#include <stdarg.h>
#include "array.h"

#define ERROR -1
#define ALIST_REALLOC_MULTIPLIER 1.5
#define ALIST_NULL UINT32_MAX

/*
 * All of these lists use UINT32_MAX as a NULL value
 * but we might consider changing this later
*/

/*
 * Struct for storing a single noede
 */
typedef struct list_node_t
{
    void *data;
    struct list_node_t *next;
    struct list_node_t *prev;
} list_node_t;

/*
 * Struct for storing a single list
 */
typedef struct list_t
{
    list_node_t *first_node;
    list_node_t *last_node;
    uint32_t size;
    size_t elementSize;
    int (*compare)(const void *first, const void *second);
} list_t;

/*
 * Good for building a sorted list
 * or for making, well anything sorted
 * 
 * has fast insert (log(n)) and allows fast lookup (log(n))
 * and range searching (log(n) gets you to the start of the range search)
*/
typedef struct tlist_node_t
{
    void* data;
    struct tlist_node_t* left;
    struct tlist_node_t* right;
    struct tlist_node_t* prev;
    struct tlist_node_t* next;
} tlist_node_t;

typedef struct tlist_t
{
    tlist_node_t *root;
    tlist_node_t *first;
    tlist_node_t *last;
    uint32_t size;
    size_t elementSize;
    int (*compare)(const void *first, const void *second);
} tlist_t;

/*
 * Null value is INT_MAX
*/
typedef struct alist_node_t
{
    uint32_t next;
    uint32_t prev;
} alist_node_t;

typedef struct alist_t
{
    void* list_start;
    uint32_t first;
    uint32_t last;
    uint32_t size;
    uint32_t capacity;
    size_t element_size;
    int (*compare)(const void *first, const void *second);
} alist_t;

alist_t new_alist(size_t element_size);
void alist_append(alist_t *list, void* data);
void* alist_get_element(alist_t* list, uint32_t element);
size_t alist_node_size(alist_t* list);
void alist_remove_node(alist_t *list, alist_node_t* node, uint32_t curr);
int alist_remove(alist_t* list, void* element);

// Returns a pointer to a new list Node with the relevant data attached
list_node_t *list_new_node(void *data, size_t dataSize);

/*
 * list vs alist
 * list insertion: O(1)
 * alist insertion: O(n)
*/


/*
 * Converts the given list into an array
*/
array_t list_to_array(list_t list);

/*
 * Appends all given items to the list
 * NULL must be last argument
*/
list_t *list_append_multi_n(list_t *list, void* toAppend, ...);

/*
 * Returns a new list where the stored elements will be of size (elementSize)
 */
list_t new_list(size_t elementSize);

/*
 * Adds the data to the end of the list
 * Requires a pointer to the element that is going to be added (element data gets copied in)
 */
list_t *list_append(list_t *list, void *data);

/*
 * Finds and returns the node at the given index
 * If the index is invalid, exits with code (-1)
 */
void *list_remove_at(list_t *list, unsigned int index);

void *list_remove_first(list_t* list);
void *list_remove_last(list_t* list);

/*
 * USAGE:
 *  Requires
 *      - List in which the item is contained
 *      - A pointer to the item
 * removes the element at the given position in the list
 * if it can be found
*/
void *list_remove_element(list_t *list, void *toRemove);

/*
 * Removes the given node from the list, updates list size and pointers
 * accordingly
*/
void *list_remove_node(list_t *list, list_node_t *toRemove);

/*
 * Combines the two given lists, appending the list 'extension'
 * to the end of the list 'base'
*/
list_t* list_combine(list_t *base, list_t *extension);

/*
 * frees all dynamically allocated data in a list.
 * IMPORTANT: Send a 'delete_data' function IF your data cannot be freed just by calling free() on it's pointer
 */
void list_destroy(list_t *list, void (*delete_data)(void *data));

#endif

// TODO
// should this also chcek if toRemove is non-null, or well, the data being recieved is not-null