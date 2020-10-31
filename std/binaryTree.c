#include "binaryTree.h"

static binary_tree_node_t *binary_tree_new_node(binary_tree_t *tree, void *data);
static void binary_tree_insert_to_node(binary_tree_t *tree, binary_tree_node_t *node, void *data);
static void binary_tree_destroy_all_nodes(binary_tree_node_t *treeNode, void (*delete_data)(void *data));

binary_tree_t new_binary_tree(unsigned int elementSize, int (*compareFunc)(void *first, void *second))
{
    binary_tree_t newTree = {0};

    newTree.elementSize = elementSize;

    // Compare function is required
    assert(compareFunc);
    newTree.compare = compareFunc;

    return newTree;
}

// returns a new binary_tree_node_t struct with its first element in its new list set to 'data'
static binary_tree_node_t *binary_tree_new_node(binary_tree_t *tree, void *data)
{
    binary_tree_node_t *node = (binary_tree_node_t *)safe_calloc(sizeof(binary_tree_node_t));

    node->list = new_list(tree->elementSize);

    list_append(&node->list, data);

    return node;
}

void binary_tree_insert(binary_tree_t *tree, void *data)
{
    if (tree == NULL || tree->root == NULL)
    {
        tree->root = binary_tree_new_node(tree, data);
    }
    else
    {
        binary_tree_insert_to_node(tree, tree->root, data);
    }

    tree->size += 1;
}

static void binary_tree_insert_to_node(binary_tree_t *tree, binary_tree_node_t *node, void *data)
{
    /* 
     * As a node cannot be created without calling tree_new_node, its list cannot be empty,
     * hence this is entirely safe. Similarly, a node cannot be entered unless it is not null
    */
    int direction = tree->compare(data, node->list.first_node);
    binary_tree_node_t **toInsert = NULL;

    if (direction == 0)
    {
        // Insert at the current node
        list_append(&node->list, data);
        return;
    }
    else if (direction > 0)
    {
        // Try inserting to the right
        toInsert = &node->right;
    }
    else
    {
        // Try inserting to the left
        toInsert = &node->left;
    }

    if (*toInsert == NULL)
    {
        // The node in the given direction is empty
        *toInsert = binary_tree_new_node(tree, data);
    }
    else
    {
        // The node in the given direction exists, Moving to next node
        binary_tree_insert_to_node(tree, *toInsert, data);
    }
}

void binary_tree_destroy(binary_tree_t tree, void (*delete_data)(void *data))
{
    binary_tree_destroy_all_nodes(tree.root, delete_data);
}

// destroys a tree node, calling the delete_data function on all of its stored data
static void binary_tree_destroy_all_nodes(binary_tree_node_t *treeNode, void (*delete_data)(void *data))
{
    if (treeNode != NULL)
    {
        list_destroy(&treeNode->list, delete_data);
        binary_tree_destroy_all_nodes(treeNode->left, delete_data);
        binary_tree_destroy_all_nodes(treeNode->right, delete_data);
        destroy(treeNode);
    }
}