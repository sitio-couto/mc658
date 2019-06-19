#ifndef DCM
#define DCM 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// Graph edge with vertex index
typedef struct edge2vert{
    int a, b;
    int cost;
} edge2vert;

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
	int *mst;
};

// Heuristic adjacency matrix tree
typedef struct {
    int primal;
    int n;
    int *deg;
    int **mst;
} heu_graph;

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
void print_mst(int *mst, int size, int **g);

/* LAGRANGIAN HEURISTIC */
struct out *lagrangian_heuristic(mat_graph *g, int max_time);
int* mst_prim(double **g, int size);
int min_value(double *values, char *mst_flag, int size);
double mult_deg(double *mult, int *deg, int size);
double subgradient(int v, int deg, int size, int *mst);
int check_viability(int size, int *r_deg, int *mst);

/* HEURISTIC */
heu_graph* first_primal(mat_graph *g);

/* METAHEURISTIC */
struct out *metaheuristic(mat_graph *g, int max_time);


/* MISC */
double curr_time(time_t start_time);
int min(int a, int b);
double max(double a, double b);
double mst_value(int *mst, int size, double **g);
int compare(const void * a, const void * b);
void test_mst(int **mx, int deg[], int n);
void dfs(int **mx, int *flag, int deg[], int n, int v, int p);


#endif
