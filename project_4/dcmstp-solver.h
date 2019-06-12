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

// Output (primal, lagrangian dual, tree)
struct out{
    double primal;
    double dual;
    // Add tree edges here
};

// Adjacency Matrix
typedef struct{
    int n,m;
    int *deg;
    int **mat;
}mat_graph;

/* IO FUNCTIONS */
// List
graph *read_input_list(char *filename);
void print_graph_list(graph *g);
void free_graph_list(graph *g);

// Matrix
mat_graph *read_input_matrix(char *filename);
void free_graph_matrix(mat_graph *g);
void print_graph_matrix(mat_graph *g);

// Out
void generate_out_file(char *filename, struct out *ans);


/* LAGRANGIAN HEURISTIC */
struct out *lagrangian_heuristic(mat_graph *g, int max_time);
void mst_prim(int **g);

/* METAHEURISTIC */
struct out *metaheuristic(mat_graph *g, int max_time);


/* MISC */
double curr_time(time_t start_time);
int min(int a, int b);
int max(int a, int b);

#endif
