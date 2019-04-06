#ifndef BNBFS
#define BNBFS 1

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

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
  int f1tr;       // End time of last task alocated in machine 1
  int f2tr;       // End time of last task alocated in machine 2
  int sumf2;      // Sum of end  times in M2 so far
  int result[64]; // Indicates the resolved permutation of tasks (pos=task.id | val=first-second...)
} node;

// Reads the input files into arrays (instance) and variables (parameters), and returns the amount of tasks.
void read_input(char *args[]);

// Branch-and-bound algorithm implementation.
void bnb(int *n_nodes);

int primal_bound(int result[], int f1tr, int f2tr, int sumf2);

int dual_bound(int result[], int f1tr, int f2tr, int sumf2);

node* add_node(node *parent, int id);

float curr_time(void);

#endif
