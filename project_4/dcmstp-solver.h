#ifndef DCM
#define DCM 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Graph edge
typedef struct edge{
    int idx;
    int cost;
    struct edge *next;
} edge;

// Graph vertex
typedef struct node{
    int deg;
    struct edge *edges;
} node;

// Graph
typedef struct{
    int n,m;
    node *list;
} graph;

// Lagrangian Output
struct out{
    double primal;
    double dual;
    // Add tree edges here
};

// IO
graph *read_input(char *filename);
void generate_out_file(char *filename, struct out *ans);
void print_graph(graph *g);
void free_graph(graph *g);

// Lagrangian Heuristic
struct out *lagrangian_heuristic(graph *g, int max_time);

// Metaheuristic
struct out *metaheuristic(graph *g, int max_time);

// Misc
double curr_time(time_t start_time);

#endif
