#include "../std/struc/alist.h"
#include "../std/struc/vector.h"
#include <stdio.h>

#define DIV_FAC 1000000
#define BYTE_TO_GIG 1073741824f

int main(int argc, char **argv) {
  vector_t vec = vector_new(sizeof(int));
  for (int i = 0; i < INT_MAX; i++) {
    vector_append(&vec, &i);
    if (i % DIV_FAC == 0 && i != 0) {
      printf("%d %d %f\n", i / DIV_FAC, *(int *)vector_get(&vec, 5), ((float)vec.capacity / 1073741824f));
    }
  }

  //// Testing for alist capacity - it works for millions, not sure past about
  /// 2gb tho, havent tried it to its limit / TODO - check its capacity past
  /// that
  // alist_t list_test = alist_new(sizeof(int));
  // for (int i = 0; i < INT_MAX; i++) {
  //     alist_append(&list_test, &i);
  //     if (i % 1000000 == 0 && i != 0) {
  //         printf("%d %d\n", i, *(int*)alist_get(&list_test, 5));
  //     }
  // }
}