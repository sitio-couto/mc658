#ifndef BNBFS
#define BNBFS 1

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct{
  int f1tr;       // End time of last task alocated in machine 1
  int f2tr;       // End time of last task alocated in machine 2
  int sumf2;      // Sum of end  times in M2 so far
  int dual;
  int primal;
  char result[64]; // Indicates the resolved permutation of tasks (pos=task.id | val=first-second...)
} node;

typedef struct{
  int id;   // Val that represents the order in which the task was read from file
  int dm1;  // Duration of taskID in machine 1
  int dm2;  // Duration of taskID in mcahine 2
} task;

// GLOBALS
// Set and mantained until end of execution
extern int n_tasks;                                  // Amount of tasks read
extern int max_nodes, max_time;                      // maximum amount of nodes and execution time
extern task **sorted_id, **sorted_dm1, **sorted_dm2; // tasks sorted by attributes id, dm1 and dm2
extern float start_time, end_time;

// Will change during execution
extern int best_dual, best_primal;         // best bounds found so far
extern float t_best_dual, t_best_primal;   // time taken for each best bound

// Definitions for heap handling
extern node **min_heap;           // Pointer to smallest element of the heap
extern int size_used, heap_size;  // Alocated and used heap positions

// Reads the input files into arrays (instance) and variables (parameters), and returns the amount of tasks.
void read_input(char *args[]);

// Branch-and-bound algorithm implementation.
void bnb(int *n_nodes);

int primal_bound(char result[], int f1tr, int f2tr, int sumf2);

int dual_bound(char result[], int f1tr, int f2tr, int sumf2);

task* add_task(int, int, int);

node* add_node(node *parent, int id);

node* make_root(void);

void print_results(char*, int, int, int);

void increase_heap(int n_tasks);

void insert_heap(node *new_node, int n_tasks);

node* remove_min(void);

int heap_check(void);

float curr_time(void);

int check_dominance(node*);

void remove_heap(int);

int cmp_dm1(const void*, const void*);

int cmp_dm2(const void*, const void*);

#endif
