#ifndef _HEAP
#define _HEAP

#include <stdio.h>
#include <stdlib.h>

node **min_heap;
int size_used = 0,heap_size = 0;

void increase_heap(int n_tasks){
  node **aux;
  int i;

  if(heap_size == 0) {
    heap_size = 2*n_tasks;
    min_heap = malloc(sizeof(node*)*heap_size);
  }else{
    heap_size *= heap_size;
    if ((aux = malloc(heap_size*sizeof(node*))) == NULL) {
      printf("FAILED TO RELOCATE MIN HEAP. EXITING...\n");
      exit(1);
    } else {
      for (i = 0; i < heap_size; ++i) aux[i] = min_heap[i];
      min_heap = aux;
    }
  }
  return;
}

void insert_heap(node *new_node, int n_tasks){
  node *aux;
  int father, child, f, c;

  if(++size_used >= heap_size) increase_heap(n_tasks);
  child = size_used-1;
  min_heap[child] = new_node;

	while(child != 0){      // Havent reached the root
		father = (child-1)/2; //get father pos
    f = min_heap[father]->sumf2;
    c = min_heap[child]->sumf2;
    if(c < f) {
      aux = min_heap[father];
      min_heap[father] = min_heap[child];
      min_heap[child] = aux;
      child = father;
    } else break;
  }

	return;
}

node* remove_min(void){
	int father = 0, left = 1, right = 2;
  node *aux, *min_node = min_heap[father];
  int f, l, r;

  if (size_used == 0) return NULL; // checks if theres at least one to remove

  // Replace min node with node from botttom
  --size_used;
  min_heap[father] = min_heap[size_used];
  min_heap[size_used] = NULL;

	while(left < size_used) {
    f = min_heap[father]->sumf2;
    l = min_heap[left]->sumf2;
    if (right < size_used) r = min_heap[right]->sumf2;
    else r = INT_MAX;

    if (f > l && (f < r || (f > r && r > l))) {
      aux = min_heap[left];
      min_heap[left] = min_heap[father];
      min_heap[father] = aux;
      father = left;
    } else if (f > r && (f < l || (f > l && l > r))) {
      aux = min_heap[right];
      min_heap[right] = min_heap[father];
      min_heap[father] = aux;
      father = right;
    } else break;

    left = 2*father+1;
    right = 2*father+2;
  }

	return min_node;
}

#endif
