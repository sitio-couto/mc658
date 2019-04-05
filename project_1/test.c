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
    new_node = add_node(0,0,i);
    printf("hs->%i|su->%i|nn->%i\n", heap_size, size_used, new_node->sumf2);
    insert_heap(new_node, 1);
  }

  for (j = 0; j < 10; ++j) {
    new_node = remove_min();
    printf("%i ", new_node->sumf2);
    for (i = 0; i < 10; ++i) printf("%i->%i|", i, new_node->result[i]);
  }
  printf("\n");
}
