#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include "bnb-fs.h"
#include "heap.h"

void main(void) {
  int i, j;
  node *n;
  srand(time(NULL));

  for (i = 0; i < 100000; ++i){
    n = calloc(1, sizeof(node));
    n->dual = rand();
    if (n->dual%10 < 6) {
      insert_heap(n, 40);
    } else if (size_used) {
      if(!heap_check()) printf("FUCKED\n");
      remove_min();
      // printf("(%i)%i<=",i, remove_min()->dual);
    }
  }

  // for (i = 40; i >=0; --i){
  //   printf("%i<=", remove_min()->dual);
  // }
}
