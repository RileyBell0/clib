#include "../avl_tree.h"

// TODO write comments describing what each function does for each function
// declaration below
avl_tree_node_t avl_tree_node_new();
static int avl_tree_insert_node(avl_tree_t *tree, int node_pos, void *data);
static int avl_tree_left_rotate(avl_tree_t *tree, int node_pos);
static int avl_tree_right_rotate(avl_tree_t *tree, int node_pos);
static void *avl_tree_node_data(avl_tree_node_t *node);
static avl_tree_node_t *avl_tree_get_node(avl_tree_t *tree, int node);
static avl_tree_node_t *avl_tree_node_left(avl_tree_t *tree,
                                           avl_tree_node_t *node);
static avl_tree_node_t *avl_tree_node_right(avl_tree_t *tree,
                                            avl_tree_node_t *node);
static int avl_tree_balance(avl_tree_t *tree, int node_pos);
static void avl_tree_write_new_node(avl_tree_t *tree, void *data);
static int avl_tree_relative_distance(int source, int dest);
static void avl_tree_extend(avl_tree_t *tree);
static int avl_tree_height(avl_tree_t *tree, int node_pos);

//////////////////////////////
// Initialisation
//////////////////////////////

avl_tree_t avl_tree_new(size_t elem_size,
                        int (*compare)(const void *elem1, const void *elem2)) {
  avl_tree_t tree;

  tree.tree_start = NULL;
  tree.root = AVL_TREE_NULL;
  tree.size = 0;
  tree.capacity = 0;
  tree.elem_size = elem_size;
  tree.block_size = elem_size + sizeof(avl_tree_node_t);
  tree.destroy_on_remove = true;
  tree.destroy = NULL;
  tree.compare = compare;

  return tree;
}

avl_tree_node_t avl_tree_node_new() {
  // TODO is this even used anywhere?
  avl_tree_node_t node;

  node.left = AVL_TREE_NULL;
  node.right = AVL_TREE_NULL;
  node.height = 1;

  return node;
}

//////////////////////////////
// Basic Operations
//////////////////////////////

void avl_tree_insert(avl_tree_t *tree, void *data) {
  avl_tree_extend(tree);
  tree->root = avl_tree_insert_node(tree, tree->root, data);
  tree->size += 1;
}

static int avl_tree_insert_node(avl_tree_t *tree, int node_pos, void *data) {
  avl_tree_node_t *node = avl_tree_get_node(tree, node_pos);
  int compare_val = tree->compare(avl_tree_node_data(node), data);
  if (compare_val <= 0) {
    if (node->left == AVL_TREE_NULL) {
      avl_tree_write_new_node(tree, data);
      node->left = avl_tree_relative_distance(node_pos, tree->size);
    } else {
      int sub_node_pos =
          avl_tree_insert_node(tree, node_pos + node->left, data);
      node->left = avl_tree_relative_distance(node_pos, sub_node_pos);
    }
  } else {
    if (node->right == AVL_TREE_NULL) {
      avl_tree_write_new_node(tree, data);
      node->right = avl_tree_relative_distance(node_pos, tree->size);
    } else {
      int sub_node_pos =
          avl_tree_insert_node(tree, node_pos + node->right, data);
      node->right = avl_tree_relative_distance(node_pos, sub_node_pos);
    }
  }

  int left_node = AVL_TREE_NULL;
  int right_node = AVL_TREE_NULL;
  void *node_left_data = NULL;
  void *node_right_data = NULL;
  if (node->left != AVL_TREE_NULL) {
    left_node = node_pos + node->left;
    node_left_data = avl_tree_node_data(avl_tree_get_node(tree, left_node));
  }
  if (node->right != AVL_TREE_NULL) {
    right_node = node_pos + node->right;
    node_right_data = avl_tree_node_data(avl_tree_get_node(tree, right_node));
  }

  node->height = 1 + max_int(avl_tree_height(tree, left_node),
                             avl_tree_height(tree, right_node));

  int balance = avl_tree_balance(tree, node_pos);

  // Left Left
  if (balance > 1) {
    int compare = tree->compare(avl_tree_node_data(node), node_left_data);
    if (compare <= 0) {
      return avl_tree_right_rotate(tree, node_pos);
    }
  }

  // Right Right
  if (balance < -1) {
    int compare = tree->compare(avl_tree_node_data(node), node_right_data);
    if (compare > 0) {
      return avl_tree_left_rotate(tree, node_pos);
    }
  }

  // Left Right
  if (balance > 1) {
    int compare = tree->compare(avl_tree_node_data(node), node_left_data);
    if (compare > 0) {
      node->left = avl_tree_relative_distance(
          node_pos, avl_tree_left_rotate(tree, left_node));
      return avl_tree_right_rotate(tree, node_pos);
    }
  }

  // Right Left
  if (balance < -1) {
    int compare = tree->compare(avl_tree_node_data(node), node_right_data);
    if (compare > 0) {
      node->right = avl_tree_relative_distance(
          node_pos, avl_tree_right_rotate(tree, right_node));
      return avl_tree_left_rotate(tree, node_pos);
    }
  }

  // return the position of the current node (unchanged)
  return node_pos;
}

//////////////////////////////
// Utility
//////////////////////////////

static int avl_tree_left_rotate(avl_tree_t *tree, int node_pos) {
  avl_tree_node_t *node = avl_tree_get_node(tree, node_pos);
  avl_tree_node_t *node_left = avl_tree_node_left(tree, node);
  int node_left_pos = node_pos + node->left;
  int node_left_right_pos = node_left_pos + node_left->right;

  // Perform rotation
  node_left->right = avl_tree_relative_distance(node_left_pos, node_pos);
  node->left = avl_tree_relative_distance(node_pos, node_left_right_pos);

  int right_node_pos = AVL_TREE_NULL;
  if (node->right != AVL_TREE_NULL) {
    right_node_pos = node_pos + node->right;
  }

  int node_left_left_pos = AVL_TREE_NULL;
  if (node_left->left != AVL_TREE_NULL) {
    node_left_left_pos = node_left_pos + node_left->left;
  }

  // Update heights
  node->height = 1 + max_int(avl_tree_height(tree, node_left_right_pos),
                             avl_tree_height(tree, right_node_pos));
  node_left->height = 1 + max_int(avl_tree_height(tree, node_left_left_pos),
                                  avl_tree_height(tree, node_pos));

  // Return new root
  return node_left_pos;
}

static int avl_tree_right_rotate(avl_tree_t *tree, int node_pos) {
  avl_tree_node_t *node = avl_tree_get_node(tree, node_pos);
  avl_tree_node_t *node_right = avl_tree_node_right(tree, node);
  int node_right_pos = node_pos + node->right;
  int node_right_left_pos = node_right_pos + node_right->left;

  // Perform rotation
  node_right->left = avl_tree_relative_distance(node_right_pos, node_pos);
  node->right = avl_tree_relative_distance(node_pos, node_right_left_pos);

  int left_node = AVL_TREE_NULL;
  if (node->left != AVL_TREE_NULL) {
    left_node = node_pos + node->left;
  }

  int node_right_right_pos = AVL_TREE_NULL;
  if (node_right->right != AVL_TREE_NULL) {
    node_right_right_pos = node_right_pos + node_right->right;
  }

  // Update heights
  node->height = 1 + max_int(avl_tree_height(tree, left_node),
                             avl_tree_height(tree, node_right_left_pos));
  node_right->height =
      1 + max_int(avl_tree_height(tree, node_pos),
                  avl_tree_height(tree, node_right_right_pos));

  // Return new root
  return node_right_pos;
}

static void *avl_tree_node_data(avl_tree_node_t *node) {
  return &node[AVL_TREE_ELEM];
}

static avl_tree_node_t *avl_tree_get_node(avl_tree_t *tree, int node) {
  return array_generic_get(tree->tree_start, node, tree->block_size);
}

static avl_tree_node_t *avl_tree_node_left(avl_tree_t *tree,
                                           avl_tree_node_t *node) {
  if (node->left == AVL_TREE_NULL) {
    return NULL;
  }

  return offset(node, tree->block_size * node->left);
}

static avl_tree_node_t *avl_tree_node_right(avl_tree_t *tree,
                                            avl_tree_node_t *node) {
  if (node->right == AVL_TREE_NULL) {
    return NULL;
  }

  return offset(node, tree->block_size * node->right);
}

static int avl_tree_balance(avl_tree_t *tree, int node_pos) {
  if (node_pos == AVL_TREE_NULL) {
    return 0;
  }

  avl_tree_node_t *node =
      array_generic_get(tree->tree_start, node_pos, tree->block_size);
  int left_node = AVL_TREE_NULL;
  int right_node = AVL_TREE_NULL;
  if (node->left != AVL_TREE_NULL) {
    left_node = node_pos + node->left;
  }
  if (node->right != AVL_TREE_NULL) {
    right_node = node_pos + node->right;
  }

  return avl_tree_height(tree, left_node) - avl_tree_height(tree, right_node);
}

static void avl_tree_write_new_node(avl_tree_t *tree, void *data) {
  avl_tree_node_t *node_area = (avl_tree_node_t *)array_generic_get(
      tree->tree_start, tree->size, tree->block_size);

  node_area->left = AVL_TREE_NULL;
  node_area->right = AVL_TREE_NULL;

  assert(memcpy(&node_area[AVL_TREE_ELEM], data, tree->elem_size));
}

static int avl_tree_relative_distance(int source, int dest) {
  return dest - source;
}

static void avl_tree_extend(avl_tree_t *tree) {
  if (tree->capacity > tree->size) {
    return;
  }

  // Increase capacity by 1.5x
  tree->capacity = tree->capacity + (tree->capacity / 2);
  if (tree->capacity <= 1) {
    tree->capacity += 1;
  }

  // Reallocate / allocate space for more nodes
  if (tree->tree_start) {
    tree->tree_start =
        safe_realloc(tree->tree_start, tree->block_size * tree->capacity);
  } else {
    tree->tree_start = safe_malloc(tree->capacity * tree->block_size);
  }
}

static int avl_tree_height(avl_tree_t *tree, int node_pos) {
  if (node_pos == AVL_TREE_NULL) {
    return 0;
  }

  avl_tree_node_t *node = (avl_tree_node_t *)array_generic_get(
      tree->tree_start, node_pos, tree->block_size);
  return node->height;
}

// TODO add the pre-order code from https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
// See if anything is being inserted
// Make sure the root is being updated right
// Find a way to write the tree to the screen
// Add removal
// 