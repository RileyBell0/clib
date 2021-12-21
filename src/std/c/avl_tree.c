// #include "../avl_tree.h"

// // TODO write comments describing what each function does for each function
// // declaration below
// static int avl_tree_insert_node(avl_tree_t *tree, int node_pos, void *data);
// static int avl_tree_left_rotate(avl_tree_t *tree, int node_pos);
// static int avl_tree_right_rotate(avl_tree_t *tree, int node_pos);
// static void *avl_tree_node_data(avl_tree_node_t *node);
// static avl_tree_node_t *avl_tree_get_node(avl_tree_t *tree, int node);
// static avl_tree_node_t *avl_tree_node_left(avl_tree_t *tree,
//                                            avl_tree_node_t *node);
// static avl_tree_node_t *avl_tree_node_right(avl_tree_t *tree,
//                                             avl_tree_node_t *node);
// static int avl_tree_write_new_node(avl_tree_t *tree, void *data, int parent);
// static int avl_tree_balance(avl_tree_t *tree, int node_pos);
// static int avl_tree_relative_distance(int source, int dest);
// static void avl_tree_extend(avl_tree_t *tree);
// static int avl_tree_height(avl_tree_t *tree, int node_pos);

// //////////////////////////////
// // Initialisation
// //////////////////////////////

// avl_tree_t avl_tree_new(size_t elem_size,
//                         int (*compare)(const void *elem1, const void *elem2)) {
//   avl_tree_t tree;

//   tree.tree_start = NULL;
//   tree.root = AVL_TREE_NULL;
//   tree.size = 0;
//   tree.capacity = 0;
//   tree.elem_size = elem_size;
//   tree.block_size = elem_size + sizeof(avl_tree_node_t);
//   tree.destroy_on_remove = true;
//   tree.destroy = NULL;
//   tree.compare = compare;

//   return tree;
// }

// //////////////////////////////
// // Basic Operations
// //////////////////////////////

// void avl_tree_insert(avl_tree_t *tree, void *data) {
//   avl_tree_extend(tree);

//   // Case of insert at root
//   if (tree->root == AVL_TREE_NULL) {
//     return avl_tree_write_new_node(tree, data, AVL_TREE_NULL);
//   }

//   tree->root = avl_tree_insert_node(tree, tree->root, data);
//   tree->size += 1;
// }

// // TODO verify the rotations are associated correctly
// // I kinda fumbled them together until it stopped giving me bus errors
// static int avl_tree_insert_node(avl_tree_t *tree, int node_pos, void *data) {
//   avl_tree_node_t *node = avl_tree_get_node(tree, node_pos);
//   void *node_data = avl_tree_node_data(node);

//   // Insert data into tree
//   if (tree->compare(data, node_data) <= 0) {
//     // Insert to the left
//     if (node->left == AVL_TREE_NULL) {
//       node->left = avl_tree_write_new_node(tree, data, node_pos);
//     }
//     else {
//       node->left = avl_tree_insert_node(tree, node->left, data);
//     }
//   } else {
//     // Insert to the right
//     if (node->right == AVL_TREE_NULL) {
//       node->right = avl_tree_write_new_node(tree, data, node_pos);
//     }
//     else {
//       node->right = avl_tree_insert_node(tree, node->right, data);
//     }
//   }

//   // Update height and balance
//   node->height = 1 + max_int(avl_tree_height(tree, node->left),
//                              avl_tree_height(tree, node->right));
                             
//   int balance = avl_tree_balance(tree, node_pos);

//   void *node_left = avl_tree_node_data(avl_tree_node_left(tree, node));
//   void *node_right = avl_tree_node_data(avl_tree_node_right(tree, node));

//   // Left Left
//   if (balance > 1 && tree->compare(node_left, node_data) <= 0) {
//     return avl_tree_right_rotate(tree, node_pos);
//   }

//   // Right Right
//   if (balance < -1 && tree->compare(node_right, node_data) > 0) {
//     return avl_tree_left_rotate(tree, node_pos);
//   }

//   // Left Right
//   if (balance > 1 && tree->compare(node_left, node_data) > 0) {
//     node->left = avl_tree_left_rotate(tree, node->left);
//     return avl_tree_right_rotate(tree, node_pos);
//   }

//   // Right Left
//   if (balance < -1 && tree->compare(node_right, node_data) <= 0) {
//     node->right = avl_tree_right_rotate(tree, node->right);
//     return avl_tree_left_rotate(tree, node_pos);
//   }

//   // return the position of the current node (unchanged)
//   return node_pos;
// }

// //////////////////////////////
// // Utility
// //////////////////////////////

// static int avl_tree_left_rotate(avl_tree_t *tree, int x_pos) {
//   avl_tree_node_t *x = avl_tree_get_node(tree, x_pos);
//   avl_tree_node_t *y = avl_tree_get_node(tree, x->right);
//   avl_tree_node_t *t = avl_tree_get_node(tree, y->left);

//   int return_node = x->right;

//   // Perform rotation
//   y->parent = x->parent;
//   x->parent = x->right;
//   t->parent = x_pos;
//   x->right = y->left;
//   y->left = x_pos;

//   // Update heights
//   x->height = 1 + max_int(avl_tree_height(tree, x->left),
//                           avl_tree_height(tree, x->right));
//   y->height = 1 + max_int(avl_tree_height(tree, y->left),
//                           avl_tree_height(tree, y->right));
//   // Return new root
//   return return_node;
// }

// static int avl_tree_right_rotate(avl_tree_t *tree, int y_pos) {
//   avl_tree_node_t *y = avl_tree_get_node(tree, y_pos);
//   avl_tree_node_t *x = avl_tree_get_node(tree, y->left);
//   avl_tree_node_t *t = avl_tree_get_node(tree, x->right);

//   int return_node = y->left;

//   // Perform Rotation
//   x->parent = y->parent;
//   y->parent = y->left;
//   t->parent = y_pos;
//   y->left = x->right;
//   x->right = y_pos;

//   y->height = 1 + max_int(avl_tree_height(tree, y->left),
//                           avl_tree_height(tree, y->right));
//   x->height = 1 + max_int(avl_tree_height(tree, x->left),
//                           avl_tree_height(tree, x->right));

//   // Return new root
//   return return_node;
// }

// static void *avl_tree_node_data(avl_tree_node_t *node) {
//   return &node[AVL_TREE_ELEM];
// }

// static avl_tree_node_t *avl_tree_get_node(avl_tree_t *tree, int node) {
//   return array_generic_get(tree->tree_start, node, tree->block_size);
// }

// static avl_tree_node_t *avl_tree_node_left(avl_tree_t *tree,
//                                            avl_tree_node_t *node) {
//   if (node->left == AVL_TREE_NULL) {
//     return NULL;
//   }
//   return avl_tree_get_node(tree, node->left);
// }

// static avl_tree_node_t *avl_tree_node_right(avl_tree_t *tree,
//                                             avl_tree_node_t *node) {
//   if (node->right == AVL_TREE_NULL) {
//     return NULL;
//   }
//   return avl_tree_get_node(tree, node->right);
// }

// static avl_tree_node_t *avl_tree_node_parent(avl_tree_t* tree,
//                                              avl_tree_node_t* node) {
//   if (node->parent == AVL_TREE_NULL) {
//     return NULL;
//   }
//   return avl_tree_get_node(tree, node->parent);
// }

// static int avl_tree_balance(avl_tree_t *tree, int node_pos) {
//   if (node_pos == AVL_TREE_NULL) {
//     return 0;
//   }

//   avl_tree_node_t *node = avl_tree_get_node(tree, node_pos);

//   return avl_tree_height(tree, node->left) - avl_tree_height(tree, node->right);
// }

// static int avl_tree_write_new_node(avl_tree_t *tree, void *data, int parent) {
//   avl_tree_node_t *node = avl_tree_get_node(tree, tree->size);

//   assert(memcpy(&node[AVL_TREE_ELEM], data, tree->elem_size));
//   node->left = AVL_TREE_NULL;
//   node->right = AVL_TREE_NULL;
//   node->height = 1;
//   node->parent = parent;

//   return tree->size;
// }

// static void avl_tree_extend(avl_tree_t *tree) {
//   if (tree->capacity > tree->size) {
//     return;
//   }

//   // Increase capacity by 1.5x
//   tree->capacity = tree->capacity + (tree->capacity / 2);
//   if (tree->capacity <= 1) {
//     tree->capacity += 1;
//   }

//   // Reallocate / allocate space for more nodes
//   if (tree->tree_start) {
//     tree->tree_start =
//         safe_realloc(tree->tree_start, tree->block_size * tree->capacity);
//   } else {
//     tree->tree_start = safe_malloc(tree->capacity * tree->block_size);
//   }
// }

// static int avl_tree_height(avl_tree_t *tree, int node_pos) {
//   if (node_pos == AVL_TREE_NULL) {
//     return 0;
//   }

//   avl_tree_node_t *node = avl_tree_get_node(tree, node_pos);

//   return node->height;
// }

// // TODO translate into my tree format
// int avl_tree_delete_node(avl_tree_t* tree, int node_pos, void* key) {
//   // Find Node
//   avl_tree_node_t* parent = NULL;
//   avl_tree_node_t *curr = avl_tree_get_node(tree, node_pos);
//   while (true) {
//     int compare_val = tree->compare(&curr[AVL_TREE_ELEM], key);
//     if (compare_val == 0) {
//       // Found node
//     }
//     else if (compare_val > 0) {
//       // TODO if something is breaking in deletion, check its going the correct way
//       // Go right
//       parent = curr;
//       curr = avl_tree_node_right(tree, curr);
//     }
//     else {
//       // TODO if something is breaking in deletion, check its going the correct way
//       // Go Left
//       parent = curr;
//       curr = avl_tree_node_left(tree, curr);
//     }
    
//     if (curr == NULL) {
//       // Requested item not in tree
//       break;
//     }
//   }
  
//   // STEP 1: PERFORM STANDARD BST DELETE

//   if (node_pos == AVL_TREE_NULL)
//     return node_pos;

//   // If the key to be deleted is smaller than the
//   // root's key, then it lies in left subtree
//   avl_tree_node_t *node = avl_tree_get_node(tree, node_pos);
//   void* node_key = avl_tree_node_data(node);
//   int compare_result = tree->compare(key, node_key);
//   if (compare_result < 0) {
//     node->left = avl_tree_delete_node(tree, node->left, key);
//   }

//   // If the key to be deleted is greater than the
//   // root's key, then it lies in right subtree
//   else if (compare_result > 0)
//     node->right = avl_tree_delete_node(tree, node->right, key);

//   // if key is same as root's key, then This is
//   // the node to be deleted
//   else {
//     // node with only one child or no child
//     if ((node->left == AVL_TREE_NULL) || (node->right == AVL_TREE_NULL)) {
//       int temp;
//       if (node->left != AVL_TREE_NULL) {
//         temp = node->left;
//       }
//       else {
//         temp = node->right;
//       }

//       // No child case
//       if (temp == AVL_TREE_NULL) {
//         // TODO What is this doing?
//         // So we've found the node and it has no children
//         // what this is doing is saying "set temp to your parent
//         // then set your parent to NULL, 
//         // The current node is to be removed, and there are no children, taht means we just need to remove the node
//         // and we have nothing to deal with.
//         // so when removing the node what we're saying is that we're gonna save
//         // the poisition of our current node 'temp'
//         /*
//          * Heres the issue, i think i may need parent pointers
//          cause if im moving the node that's last in the array to fill the empty
//          spot then i need to access its parent
//         */
//         temp = node_pos;
//         node_pos = AVL_TREE_NULL;
//       } else {
//         *root = *temp; // Copy the contents of
//       }          // One child case
//                        // the non-empty child
//       free(temp);
//     } else {
//       // node with two children: Get the inorder
//       // successor (smallest in the right subtree)
//       struct Node *temp = minValueNode(root->right);

//       // Copy the inorder successor's data to this node
//       root->key = temp->key;

//       // Delete the inorder successor
//       root->right = deleteNode(root->right, temp->key);
//     }
//   }

//   // If the tree had only one node then return
//   if (root == NULL)
//     return root;

//   // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
//   root->height = 1 + max(height(root->left), height(root->right));

//   // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
//   // check whether this node became unbalanced)
//   int balance = getBalance(root);

//   // If this node becomes unbalanced, then there are 4 cases

//   // Left Left Case
//   if (balance > 1 && getBalance(root->left) >= 0)
//     return rightRotate(root);

//   // Left Right Case
//   if (balance > 1 && getBalance(root->left) < 0) {
//     root->left = leftRotate(root->left);
//     return rightRotate(root);
//   }

//   // Right Right Case
//   if (balance < -1 && getBalance(root->right) <= 0)
//     return leftRotate(root);

//   // Right Left Case
//   if (balance < -1 && getBalance(root->right) > 0) {
//     root->right = rightRotate(root->right);
//     return leftRotate(root);
//   }

//   return root;
// }

// // TODO add the pre-order code from
// // https://www.geeksforgeeks.org/avl-tree-set-1-insertion/ See if anything is
// // being inserted Make sure the root is being updated right Find a way to write
// // the tree to the screen Add removal
// //