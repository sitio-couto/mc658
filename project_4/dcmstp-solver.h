#ifndef DCM
#define DCM 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <float.h>
#include <string.h>

// Graph edge with vertex index
typedef struct edge_list{
    int a, b;
    int cost;
} edge_list;

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
    int n, m;
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
struct out *lagrangian_heuristic(mat_graph *g, int max_time, time_t start_time);
int* mst_prim(double **g, int size);
int min_value(double *values, char *mst_flag, int size);
double mult_deg(double *mult, int *deg, int size);
double subgradient(int v, int deg, int size, int *mst);
int check_viability(int size, int *r_deg, int *mst);
int update_multipliers_and_check(mat_graph *g, double *mult, int *mst, double *subgrad, struct out *ans, int viable, double pi);

/* HEURISTIC / METAHEURISTIC */
heu_graph* first_primal(mat_graph *g);
void tag_component (int **mx, int n, int v, int *comp, int tag);
void tag_component_dfs (int **mx, int n, int *visited, int v, int *comp, int tag);
void heuristic(heu_graph *r, edge_list *e);
int* get_comp_gap (int n, int comp[], int deg[], int n_comp);
void update_comp_gap (int n, int comp[], int deg[], int n_curr, int *curr);
void insert_edge(heu_graph *r, int vacant[], int comp[], edge_list e);
void remove_edge(heu_graph *r, int vacant[], int comp[], edge_list e, int k);

/* METAHEURISTIC */
struct out *metaheuristic(mat_graph *g, int max_time, time_t start_time);

/* MISC */
double curr_time(time_t start_time);
int min(int a, int b);
double max(double a, double b);
double mst_value(int *mst, int size, double **g);
int compare(const void * a, const void * b);
int contains(edge_list *arr[], int len, edge_list e);
int* to_array (int **mx, int n, int *arr);
void to_array_dfs (int **mx, int n, int *visited, int v, int *arr);
struct out* out_alloc(int primal, int dual, int n);
void heu_graph_free (heu_graph *hg);
edge_list* edge_list_alloc (int **mx, int n, int m);
int is_disjoint(int comp[], int n);

/* TEST */
void test_mst(int **mx, int deg[], int n, int comp[]);
void dfs_test(int **mx, int *flag, int deg[], int n, int v, int p);
int is_new_result (int *(***list), int *qnt, heu_graph *r);
int is_equal (int *old[], int *new[], int n);
int*** append_result (int ***list, int qnt, int **new, int n);
void free_results (int ***list, int qnt, int n);

#endif
