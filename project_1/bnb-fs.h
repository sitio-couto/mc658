#ifndef BNBFS
#define BNBFS 1

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

typedef struct{
  int id;   // Val that represents the order in which the task was read from file
  int dm1;  // Duration of taskID in machine 1
  int dm2;  // Duration of taskID in mcahine 2
} task;

task* add_task(int id, int dm1, int dm2) {
  task *node = malloc(sizeof(task));
  node->id = id;
  node->dm1 = dm1;
  node->dm2 = dm2;
  return node;
}

int cmp_dm1(const void *a, const void *b) {
  return ( (*(task**)a)->dm1 -(*(task**)b)->dm1 );
}

int cmp_dm2(const void *a, const void *b) {
   return ( (*(task**)a)->dm2 - (*(task**)b)->dm2 );
}

typedef struct{
  int dual_bound;   // Lower bound calculated with the classifying functions
  int primal_bound; // Upper bound given by the sum of finishing times do far
  float t_dual;
  float t_primal;
  int* result;      // Indicates the resolved permutation of tasks (pos=task.id | val=first-second...)
} node;

// Reads the input files into arrays (instance) and variables (parameters), and returns the amount of tasks.
void read_input(char *args[]);

// Branch-and-bound algorithm implementation.
node* bnb(int *n_nodes);

#endif
