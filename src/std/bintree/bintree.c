#include "bintree.h"

static tree_node *tree_new_node(void *data);
static void tree_insert_to_node(tree_node *node, void *data, int (*compare)(void *first, void *second));
static void tree_destroy_all_nodes(tree_node *treeNode, void (*delete_data)(void *data));

tree new_tree()
{
    tree newTree = {0};

    return newTree;
}

// returns a new tree_node struct with its first element in its new list set to 'data'
static tree_node *tree_new_node(void *data)
{
    tree_node *node = (tree_node *)safe_calloc(sizeof(tree_node));

    list_append(&node->list, data);

    return node;
}


void tree_insert(tree tree, void *data, int (*compare)(void *first, void *second))
{
    if (tree.root == NULL)
    {
        tree.root = tree_new_node(data);
    }
    else
    {
        tree_insert_to_node(tree.root, data, compare);
    }
}

static void tree_insert_to_node(tree_node *node, void *data, int (*compare)(void *first, void *second))
{
    /* 
     * As a node cannot be created without calling tree_new_node, its list cannot be empty,
     * hence this is entirely safe. Similarly, a node cannot be entered unless it is not null
    */
    int direction = compare(data, node->list.first_node);
    tree_node **toInsert = NULL;

    if (direction == 0)
    {
        // Insert at the current node
        list_append(&node->list, data);
        return;
    }
    else if (direction > 0)
    {
        toInsert = &node->right;
    }
    else
    {
        // Data's value is less than the value at the current node
        toInsert = &node->left;
    }

    if (*toInsert == NULL)
    {
        // The node in the given direction is empty
        *toInsert = tree_new_node(data);
    }
    else
    {
        // The node in the given direction exists, Moving to next node
        tree_insert_to_node(*toInsert, data, compare);
    }
}

void tree_destroy(tree tree, void (*delete_data)(void *data))
{
    tree_destroy_all_nodes(tree.root, delete_data);
    tree.root = NULL;
}

// destroys a tree node, calling the delete_data function on all of its stored data
static void tree_destroy_all_nodes(tree_node *treeNode, void (*delete_data)(void *data))
{
    if (treeNode != NULL)
    {
        list_destroy(&treeNode->list, delete_data);
        tree_destroy_all_nodes(treeNode->left, delete_data);
        tree_destroy_all_nodes(treeNode->right, delete_data);
        destroy(treeNode);
    }
}