#include "bnb-fs.h"

node **min_heap;
int size_used = 0,heap_size = 0;

void increase_heap(int n_tasks){
  //node **aux;
  //int i;

  if(heap_size == 0) {
    heap_size = 2*n_tasks;
    min_heap = malloc(sizeof(node*)*heap_size);
  }else{
    heap_size *= 2;
    min_heap = realloc(min_heap, sizeof(node*)*heap_size);
    if (!min_heap) {
      printf("\nREALLOC FAILLED, EXITING...\n");
      exit(0);
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
    f = min_heap[father]->dual;
    c = min_heap[child]->dual;
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
  min_heap[0] = min_heap[size_used];
  min_heap[size_used] = NULL;

	while(left < size_used) {
    f = min_heap[father]->dual;
    l = min_heap[left]->dual;
    if (right < size_used) r = min_heap[right]->dual;
    else r = INT_MAX;

    if (f > l && (f <= r || r >= l)) {
      aux = min_heap[left];
      min_heap[left] = min_heap[father];
      min_heap[father] = aux;
      father = left;
    } else if (f > r && (f <= l || l >= r)) {
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

void remove_heap(int father){
	int left = 2*father+1, right = 2*father+2;
  node *aux;
  int f, l, r;

  if (size_used == 0) return; // checks if theres at least one to remove

  // Replace min node with node from botttom
  --size_used;
  free(min_heap[father]);
  min_heap[father] = min_heap[size_used];
  min_heap[size_used] = NULL;

	while(left < size_used) {
    f = min_heap[father]->dual;
    l = min_heap[left]->dual;
    if (right < size_used) r = min_heap[right]->dual;
    else r = INT_MAX;

    if (f > l && (f <= r || r >= l)) {
      aux = min_heap[left];
      min_heap[left] = min_heap[father];
      min_heap[father] = aux;
      father = left;
    } else if (f > r && (f <= l || l >= r)) {
      aux = min_heap[right];
      min_heap[right] = min_heap[father];
      min_heap[father] = aux;
      father = right;
    } else break;

    left = 2*father+1;
    right = 2*father+2;
  }

	return;
}

int heap_check(void){
  int i;

  for (i = 1; i < size_used; ++i) {
    if (min_heap[(i-1)/2]->dual > min_heap[i]->dual){
      printf("%i <= %i is false\n", min_heap[(i-1)/2]->dual, min_heap[i]->dual);
      return 0;
    }
  }

  return 1;
}
