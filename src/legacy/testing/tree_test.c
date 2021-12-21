#include "../std/avl_tree.h"
#include "../std/fileIO.h"
#include "../std/queue.h"
#include "../std/stack.h"
#include "../std/string.h"
void tree_print_layers(avl_tree_t *tree);
void explore_tree(avl_tree_t* tree);

int main(int argc, char **argv) {
  avl_tree_t avl = avl_tree_new(sizeof(string_t), string_void_compare);
  avl_tree_t *tree = &avl;

  string_t gerald = string_new(DEFAULT_BUFFER_LEN);
  string_t *buffer = &gerald;

  string_t exit_str = string_make("exit");
  printf("Enter a thing: ");
  while (fileio_next_line(stdin, buffer)) {
    string_t copy = string_copy(buffer);

    if (!cstring_equals(cstr(buffer), "trav")) {
      avl_tree_insert(tree, &copy);
    }
    for (int i = 0; i < tree->size; i++) {
      avl_tree_node_t* stuff = array_generic_get(tree->tree_start, i, tree->block_size);
      string_t* elem = (string_t*)&stuff[1];
      printf("(%d)%s   ", i, cstr(elem));
    }
    printf("\n");

    if (string_compare(&exit_str, buffer) == 0) {
      break;
    }
    else if (cstring_equals(cstr(buffer), "trav")) {
      explore_tree(tree);
    }

    printf("Size: %d\nCapacity: %d\nRoot: %d\n", tree->size, tree->capacity,
           tree->root);
  }
}

int ipow(int base, int exp) {
  int result = 1;
  for (;;) {
    if (exp & 1)
      result *= base;
    exp >>= 1;
    if (!exp)
      break;
    base *= base;
  }

  return result;
}


void explore_tree(avl_tree_t* tree) {
  string_t buf = string_new(DEFAULT_BUFFER_LEN);

  stackk_t stack = stack_new(sizeof(int));

  string_t exit_string = string_make("exit");
  string_t *buffer = &buf;
  int curr_node = tree->root;
  int level = 0;
  while (true) {
    avl_tree_node_t *node = (avl_tree_node_t*)array_generic_get(tree->tree_start, curr_node, tree->block_size);
    printf("\n\n\n\nCURRENT NODE: %d, \"%s\"\n", curr_node, cstr(&node[1]));
    printf("Level: %d\n", level);
    printf("||%d||    ||%d||\n", node->left, node->right);

    printf("Enter \"L\" or \"R\" or \"B\" or \"exit\": ");
    fileio_next_line(stdin, buffer);
    if (string_compare(buffer, &exit_string) == 0) {
      break;
    }
    char* str = cstr(buffer);
    if (cstring_equals(str, "l")) {
      printf("Going Left...\n");
      stack_push(&stack, &curr_node);
      curr_node = node->left;
      level += 1;
    } else if (cstring_equals(str, "r")) {
      printf("Going Right...\n");
      stack_push(&stack, &curr_node);
      curr_node = node->right;
      level += 1;
    } else if (cstring_equals(str, "b")) {
      stack_pop(&stack, &curr_node);
      printf("Going Back...\n");
      level -= 1;
    }

  }
}