#ifndef BNBFS
#define BNBFS 1

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

typedef struct{
    int dual_bound;
    int primal_bound;
    float t_dual;
    float t_primal;
    int task;                    // TODO: precisa ser a sequencia de jobs: vetor?
              } node;

// Reads the input files into arrays (instance) and variables (parameters), and returns the amount of tasks.
int read_input(char *args[], int **start_t, int **end_t, int *max_node, int *max_time);

// Branch-and-bound algorithm implementation.
node* bnb(int max_node, int max_time, int *n_nodes);

#endif
