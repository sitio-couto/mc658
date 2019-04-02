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
  task *new_task = malloc(sizeof(task));
  new_task->id = id;
  new_task->dm1 = dm1;
  new_task->dm2 = dm2;
  return new_task;
}

int cmp_dm1(const void *a, const void *b) {
  return ( (*(task**)a)->dm1 -(*(task**)b)->dm1 );
}

int cmp_dm2(const void *a, const void *b) {
   return ( (*(task**)a)->dm2 - (*(task**)b)->dm2 );
}

typedef struct{
  int dual_bound;   // Lower bound calculated with the classifying functions
  int primal_bound; // Upper bound given by the sum of finishing times so far
  float t_dual;
  float t_primal;
  int* result;      // Indicates the resolved permutation of tasks (pos=task.id | val=first-second...)
} node;

node* add_node(int db, int pb, float td, float tp, int r[]){
  node *new_node = malloc(sizeof(node));
  new_node->dual_bound = db;
  new_node->primal_bound = pb;
  new_node->t_dual = td;
  new_node->t_primal = tp;
  new_node->result = r;
  return new_node;
}

// Reads the input files into arrays (instance) and variables (parameters), and returns the amount of tasks.
void read_input(char *args[]);

// Branch-and-bound algorithm implementation.
node* bnb(int *n_nodes);

#endif
