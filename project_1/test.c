#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include "bnb-fs.h"
#include "heap.h"

void main(void) {
  int i, j;
  node *new_node;

  for (i = 9; i >= 0; --i) {
    new_node = add_node(0,i,0,0,NULL);
    printf("hs->%i|su->%i|nn->%i\n", heap_size, size_used, new_node->primal_bound);
    insert_heap(new_node, 1);
  }

  for (j = 0; j < 10; ++j) {
    new_node = remove_min();
    printf("%i ", new_node->primal_bound);
  }
  printf("\n");
}
