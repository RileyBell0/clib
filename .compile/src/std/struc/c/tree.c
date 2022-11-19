#include "../tree.h"

static tree_node_t *tree_node_new(size_t elem_size, void *data);
static void *tree_node_data(tree_node_t *node);
static tree_node_t *tree_node_max(tree_node_t *start);
static tree_node_t *tree_node_min(tree_node_t *start);
static tree_node_t *tree_node_pop_max(tree_node_t **link_to_start,
                                      tree_node_t *start);
static tree_node_t *tree_node_pop_min(tree_node_t **link_to_start,
                                      tree_node_t *start);
static void tree_destroy_node(tree_t *tree, tree_node_t *node);

tree_t tree_new(size_t elem_size,
                int (*compare)(const void *first, const void *second),
                void (*destroy)(void *data))
{
  tree_t tree;

  tree.elem_size = elem_size;
  tree.destroy_on_remove = true;
  tree.allow_collisions = true;
  tree.size = 0;
  tree.head = NULL;
  tree.compare = compare;
  tree.destroy = destroy;

  return tree;
}

// Append an item to a tree
void tree_insert(tree_t *tree, void *data)
{
  tree->size += 1;
  tree_node_t **current_node = &tree->head;

  // Find the leaf to append the data to
  while (true)
  {
    // Have we found a leaf?
    if (*current_node == NULL)
    {
      break;
    }

    // Go left or right?
    if (tree->compare(data, tree_node_data(*current_node)) <= 0)
    {
      current_node = &(*current_node)->left;
    }
    else
    {
      current_node = &(*current_node)->right;
    }
  }

  // Append the data to this node
  *current_node = tree_node_new(tree->elem_size, data);
}

bool tree_remove(tree_t *tree, void *data)
{
  tree_node_t *current_node = tree->head;
  tree_node_t **link_to_current = &tree->head;

  // Find the leaf to append the data to
  while (current_node != NULL)
  {
    int compare_val = tree->compare(data, tree_node_data(current_node));

    if (compare_val == 0)
    {
      // Matching data is in the current node.
      break;
    }
    else if (compare_val < 0)
    {
      // Go Left
      link_to_current = &current_node->left;
      current_node = current_node->left;
    }
    else
    {
      // Go Right
      link_to_current = &current_node->right;
      current_node = current_node->right;
    }
  }

  if (current_node == NULL)
  {
    return false;
  }

  tree_node_t *left = current_node->left;
  tree_node_t *right = current_node->right;
  tree_node_t *replacement = NULL;

  if (left && right)
  {
    // replacement = tree_node_pop_min(link_to_current, current_node->right); // TODO
    replacement = tree_node_pop_min(&current_node->right, current_node->right);
    replacement->left = current_node->left;
    replacement->right = current_node->right;
  }
  else if (left)
  {
    replacement = left;
  }
  else if (right)
  {
    replacement = right;
  }

  *link_to_current = replacement;

  tree_destroy_node(tree, current_node);

  tree->size -= 1;

  return true;
}

void *tree_get(tree_t *tree, void *key)
{
  tree_node_t *current_node = tree->head;

  // Find the leaf to append the data to
  while (current_node != NULL)
  {
    int compare_val = tree->compare(key, tree_node_data(current_node));

    if (compare_val == 0)
    {
      // Matching data is in the current node.
      return tree_node_data(current_node);
    }
    else if (compare_val < 0)
    {
      // Go Left
      current_node = current_node->left;
    }
    else
    {
      // Go Right
      current_node = current_node->right;
    }
  }

  return NULL;
}

bool tree_contains(tree_t *tree, void *key)
{
  tree_node_t *current_node = tree->head;

  // Find the leaf to append the data to
  while (current_node != NULL)
  {
    int compare_val = tree->compare(key, tree_node_data(current_node));

    if (compare_val == 0)
    {
      // Matching data is in the current node.
      return true;
    }
    else if (compare_val < 0)
    {
      // Go Left
      current_node = current_node->left;
    }
    else
    {
      // Go Right
      current_node = current_node->right;
    }
  }

  return false;
}

void *tree_get_min(tree_t *tree)
{
  return tree_node_data(tree_node_min(tree->head));
}

void *tree_get_max(tree_t *tree)
{
  return tree_node_data(tree_node_max(tree->head));
}

// Does not call the tree_t's destroy function
void *tree_pop_min(tree_t *tree)
{
  tree_node_t *min = tree_node_pop_min(&tree->head, tree->head);
  void *data = NULL;

  if (min)
  {
    data = tree_node_data(min);
    free(min);
    tree->size -= 1;
  }

  return data;
}

// Does not call the tree_t's destroy function
void *tree_pop_max(tree_t *tree)
{
  tree_node_t *max = tree_node_pop_max(&tree->head, tree->head);
  void *data = NULL;

  if (max)
  {
    data = tree_node_data(max);
    free(max);
    tree->size -= 1;
  }

  return data;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

tree_node_t *tree_node_new(size_t elem_size, void *data)
{
  tree_node_t *node =
      (tree_node_t *)safe_malloc(elem_size + sizeof(tree_node_t));

  node->left = NULL;
  node->right = NULL;

  // Copy the data across
  memcpy(&node[TREE_NODE_ELEMENT], data, elem_size);

  return node;
}

// Returns a pointer to the start of the data attached to the tree node
void *tree_node_data(tree_node_t *node)
{
  return (void *)&node[TREE_NODE_ELEMENT];
}

tree_node_t *tree_node_max(tree_node_t *start)
{
  if (!start)
  {
    return NULL;
  }

  while (true)
  {
    if (start->right == NULL)
    {
      return start;
    }
    start = start->right;
  }

  return start;
}

tree_node_t *tree_node_min(tree_node_t *start)
{
  if (!start)
  {
    return NULL;
  }

  while (true)
  {
    if (start->left == NULL)
    {
      return start;
    }
    start = start->left;
  }

  return start;
}

tree_node_t *tree_node_pop_max(tree_node_t **link_to_start,
                               tree_node_t *start)
{
  if (!start)
  {
    return NULL;
  }

  while (true)
  {
    if (start->right == NULL)
    {
      // Unlink this node from the tree and return
      *link_to_start = start->left;
      return start;
    }
    link_to_start = &start->right;
    start = start->right;
  }

  return start;
}

tree_node_t *tree_node_pop_min(tree_node_t **link_to_start,
                               tree_node_t *start)
{
  if (!start)
  {
    return NULL;
  }

  while (true)
  {
    if (start->left == NULL)
    {
      // Unlink this node from the tree and return
      *link_to_start = start->right;
      return start;
    }
    link_to_start = &start->left;
    start = start->left;
  }

  return start;
}

void tree_destroy_node(tree_t *tree, tree_node_t *node)
{
  // Complex custom data-type destruction method
  if (tree->destroy_on_remove)
  {
    tree->destroy((void *)&node[TREE_NODE_ELEMENT]);
  }

  // Free the node's memory
  free(node);
}