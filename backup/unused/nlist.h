#ifndef CLIB_NLIST_H
#define CLIB_NLIST_H

//TODO
/*
 * Add removal methods for when the list is sorted
 * add insertion methods for when the list is sorted
 * deal with edge cases or cases where we need to merge
 * two nodes 
*/

#include "general.h"
#include "array.h"
#include <stdio.h>
#include <stdint.h>

#define NLIST_NUM_UINT32 3
#define NLIST_NUM_POINTER 2
#define SIZEOF_INT32 4
#define OFFSET_CHUNKLEN 4
#define OFFSET_ELEMENT_SIZE 8
#define OFFSET_LAST 12

#define NLIST_NODE_NUM_POINTER 2
#define NLIST_NODE_NUM_UINT 1

/* 
 * int32 len
 * void* next
 * void* prev
 * array
*/
typedef void* nlist_node;

/*
 * int32 size
 * int32 chunklen
 * int32 elementsize
 * void* last
 * void* first
*/
typedef void* nlist;

nlist new_nlist(uint32_t chunkLen, uint32_t elementSize);

nlist_node new_nlist_node(nlist list);

void nlist_append(nlist list, void* data);

void nlist_insert(nlist list, void* data);

// Getters for list
uint32_t* nlist_get_size(nlist list);

uint32_t* nlist_get_chunklen(nlist list);

uint32_t* nlist_get_elementSize(nlist list);

nlist_node* nlist_get_last(nlist list);

nlist_node* nlist_get_first(nlist list);

// Getters for node
uint32_t* nlist_node_get_len(nlist_node node);

nlist_node* nlist_node_get_next(nlist_node node);

nlist_node* nlist_node_get_prev(nlist_node node);

void* nlist_node_get_array(nlist_node node);

#endif