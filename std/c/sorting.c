#include "../sorting.h"

  
array_t tree_sort(array_t data, unsigned int elementSize, int (*compareFunc)(const void *first, const void *second))
{
    avl_bin_tree_t tree = new_avl_bin_tree(elementSize, compareFunc);
    
    for (int i = 0; i < data.len; i++)
    {
        // Add everything to the tree
        avl_bin_tree_insert(&tree, array_get_element(data.dat, i, elementSize));
    }
    
    array_t sorted = avl_bin_tree_to_array(tree);

    avl_bin_tree_destroy(tree, NULL);

    return sorted;
}