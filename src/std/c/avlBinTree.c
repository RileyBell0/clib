#include "../avlBinTree.h"

static avl_bin_tree_node_t *avl_bin_tree_new_node(avl_bin_tree_t *tree, void *data);
static avl_bin_tree_node_t *avl_bin_tree_insert_node(avl_bin_tree_t *tree, avl_bin_tree_node_t *node, void *data);
static void avl_bin_tree_destroy_all_nodes(avl_bin_tree_node_t *treeNode, void (*delete_data)(void *data));
static avl_bin_tree_node_t *avl_rotate_left(avl_bin_tree_node_t *base);
static avl_bin_tree_node_t *avl_rotate_right(avl_bin_tree_node_t *base);

avl_bin_tree_t new_avl_bin_tree(unsigned int elementSize, int (*compareFunc)(const void *first, const void *second))
{
    avl_bin_tree_t newTree = {0};

    newTree.elementSize = elementSize;

    // Compare function is required
    assert(compareFunc);
    newTree.compare = compareFunc;

    return newTree;
}

// A utility function to get the height of the tree 
int avl_bin_tree_node_height(struct avl_bin_tree_node_t *N) 
{ 
    if (N == NULL) 
        return 0; 
    return N->height; 
} 

// Get Balance factor of node N 
int getBalance(struct avl_bin_tree_node_t *N) 
{ 
    if (N == NULL) 
        return 0; 
    return avl_bin_tree_node_height(N->left) - avl_bin_tree_node_height(N->right); 
} 

// returns a new avl_bin_tree_node_t struct with its first element in its new list set to 'data'
static avl_bin_tree_node_t *avl_bin_tree_new_node(avl_bin_tree_t *tree, void *data)
{
    avl_bin_tree_node_t *node = (avl_bin_tree_node_t *)safe_calloc(sizeof(avl_bin_tree_node_t));

    // Make space for the new element
    node->data = safe_malloc(tree->elementSize);

    memcpy(node->data, data, tree->elementSize);

    node->height = 1;

    return node;
}

void avl_bin_tree_insert(avl_bin_tree_t *tree, void *data)
{
    tree->root = avl_bin_tree_insert_node(tree, tree->root, data);
    
    tree->size += 1;
}

avl_bin_tree_node_t *avl_bin_tree_insert_node(avl_bin_tree_t *tree, avl_bin_tree_node_t *node, void *data)
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL) 
        return(avl_bin_tree_new_node(tree, data)); 
    
    int direction = tree->compare(data, node->data);

    if (direction < 0) 
        node->left  = avl_bin_tree_insert_node(tree, node->left, data); 
    else if (direction > 0) 
        node->right  = avl_bin_tree_insert_node(tree, node->right, data); 
    else // Equal keys are not allowed in BST 
        return node; 
  
    /* 2. Update height of this ancestor node */
    node->height = 1 + max_int(avl_bin_tree_node_height(node->left), 
                           avl_bin_tree_node_height(node->right)); 
  
    /* 3. Get the balance factor of this ancestor 
          node to check whether this node became 
          unbalanced */
    int balance = getBalance(node); 
  
    // If this node becomes unbalanced, then 
    // there are 4 cases 
  
    // Left Left Case 
    if (balance > 1 && tree->compare(data, node->left->data) < 0) 
        return avl_rotate_right(node); 
  
    // Right Right Case 
    if (balance < -1 && tree->compare(data, node->right->data) > 0) 
        return avl_rotate_left(node); 
  
    // Left Right Case 
    if (balance > 1 && tree->compare(data, node->left->data) > 0) 
    { 
        node->left =  avl_rotate_left(node->left); 
        return avl_rotate_right(node); 
    } 
  
    // Right Left Case 
    if (balance < -1 && tree->compare(data, node->right->data) < 0) 
    { 
        node->right = avl_rotate_right(node->right); 
        return avl_rotate_left(node); 
    } 
  
    /* return the (unchanged) node pointer */
    return node; 
}

static avl_bin_tree_node_t *avl_rotate_left(avl_bin_tree_node_t *x)
{
    /*
     * Patching references for rotation
    */
    struct avl_bin_tree_node_t *y = x->right; 
    struct avl_bin_tree_node_t *T2 = y->left; 
  
    // Perform rotation 
    y->left = x; 
    x->right = T2; 
  
    //  Update heights 
    x->height = max_int(avl_bin_tree_node_height(x->left), avl_bin_tree_node_height(x->right))+1; 
    y->height = max_int(avl_bin_tree_node_height(y->left), avl_bin_tree_node_height(y->right))+1; 
  
    // Return new root 
    return y; 
}

static avl_bin_tree_node_t *avl_rotate_right(avl_bin_tree_node_t *y)
{
    
    /*
     * Patching references for rotation
    */
    struct avl_bin_tree_node_t *x = y->left; 
    struct avl_bin_tree_node_t *T2 = x->right; 
  
    // Perform rotation 
    x->right = y; 
    y->left = T2; 
  
    // Update heights 
    y->height = max_int(avl_bin_tree_node_height(y->left), avl_bin_tree_node_height(y->right))+1; 
    x->height = max_int(avl_bin_tree_node_height(x->left), avl_bin_tree_node_height(x->right))+1; 
  
    // Return new root 
    return x; 
}

void avl_bin_tree_destroy(avl_bin_tree_t tree, void (*delete_data)(void *data))
{
    avl_bin_tree_destroy_all_nodes(tree.root, delete_data);
}

// destroys a tree node, calling the delete_data function on all of its stored data
static void avl_bin_tree_destroy_all_nodes(avl_bin_tree_node_t *treeNode, void (*delete_data)(void *data))
{
    if (treeNode != NULL)
    {
        if (delete_data)
        {
            delete_data(treeNode->data);
        }
        destroy(treeNode->data);
        avl_bin_tree_destroy_all_nodes(treeNode->left, delete_data);
        avl_bin_tree_destroy_all_nodes(treeNode->right, delete_data);
        destroy(treeNode);
    }
}

void avl_bin_tree_fill_arr(array_t *arr, avl_bin_tree_node_t *node, int *pos, unsigned int elementSize)
{
    if (node == NULL)
        return;

    /* first recur on left child */
    avl_bin_tree_fill_arr(arr, node->left, pos, elementSize);

    // Add the element in the node to the array
    array_set_element(arr->data, node->data, *pos, elementSize);
    *pos += 1;

    /* then recur on right child */
    avl_bin_tree_fill_arr(arr, node->right, pos, elementSize);
}

array_t avl_bin_tree_to_array(avl_bin_tree_t tree)
{
    array_t final = new_array(tree.size, tree.elementSize);
    int pos = 0;

    avl_bin_tree_fill_arr(&final, tree.root, &pos, tree.elementSize);

    return final;
}
