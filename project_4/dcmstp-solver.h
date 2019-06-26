#ifndef DCM
#define DCM 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <math.h>

// Adjacency Matrix
typedef struct{
    int n,m;
    int *deg;
    int **mat;
}mat_graph;

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

// Graph edge with vertex index
typedef struct edge_list{
    int a, b;
    int cost;
} edge_list;

/* IO FUNCTIONS */
mat_graph *read_input(char *filename);
void free_graph(mat_graph *g);
void print_graph(mat_graph *g);
void generate_out_file(char *filename, struct out *ans, int size);
void print_mst(int *mst, int size, int **g);

/* LAGRANGIAN HEURISTIC */
struct out *lagrangian_heuristic(mat_graph *g, int max_time, time_t start_time);
int* mst_prim(double **g, int size);
int min_value(double *values, char *mst_flag, int size);
double mult_deg(double *mult, int *deg, int size);
double subgradient(int v, int deg, int size, int *mst);
int check_viability(int size, int *r_deg, int *mst);
void update_primal(struct out *ans, mat_graph *g, int *mst, int viable, time_t start_time, int max_time);
int *viabilize_mst(int *mst, mat_graph *g, time_t start_time, int max_time);
int change_edge(mat_graph *g, int parent, int *deg_mst, int *dcmst, int curr_pos);
int *splice_row(int v, int parent, int *deg, mat_graph *g);
int check_cycle_connection(int *tree, int size);
int update_multipliers_and_check(mat_graph *g, double *mult, int *mst, double *subgrad, struct out *ans, int viable, double pi);

/* HEURISTIC / METAHEURISTIC */
heu_graph* first_primal(mat_graph *g);
void tag_component (int **mx, int n, int v, int *comp, int tag);
void tag_component_dfs (int **mx, int n, int *visited, int v, int *comp, int tag);
void heuristic(heu_graph *r, edge_list *e, char** tabus, char** timer);
int* get_comp_gap (int n, int comp[], int deg[], int n_comp);
void update_comp_gap (int n, int comp[], int deg[], int n_curr, int *curr);
void insert_edge(heu_graph *r, int vacant[], int comp[], edge_list e);
void remove_edge(heu_graph *r, int vacant[], int comp[], edge_list e, int k);
int viable_insertion(heu_graph* r, int* vacant, int* comp, char** tabus, edge_list e, int count);

/* METAHEURISTIC */
struct out *metaheuristic(mat_graph *g, int max_time, time_t start_time);
void add_tabu (char **tabus, char **timer, edge_list e);
int is_tabu (char **tabus, edge_list e);
void update_tabus (char** tabus, char** timer, int n);
int tabu_time(edge_list e);

/* MISC */
double curr_time(time_t start_time);
int min(int a, int b);
int min_array_idx(int *arr, int n);
double max(double a, double b);
double mst_value(int *mst, int size, double **g);
int mst_value_int(int *mst, int size, int **g);
int compare(const void * a, const void * b);
int contains(edge_list *arr[], int len, edge_list e);
int is_connected(int **g, int *visited, int size);
void connected_dfs(int **g, int *visited, int n, int v);
int* to_array (int **mx, int n, int *arr);
void to_array_dfs (int **mx, int n, int *visited, int v, int *arr);
struct out* out_alloc(int primal, int dual, int n);
void heu_graph_free (heu_graph *hg);
edge_list* edge_list_alloc (int **mx, int n, int m);
int is_disjoint(int comp[], int n);
void alloc_tabus (char*** tabus, char*** timer, int n);
void tabus_free (char*** tabus, char*** timer, int n);
heu_graph* heu_graph_alloc (mat_graph *g);


/* TEST */
void test_mst(int **mx, int deg[], int n, int comp[]);
void dfs_test(int **mx, int *flag, int deg[], int n, int v, int p);
int is_new_result (int *(***list), int *qnt, heu_graph *r);
int is_equal (int *old[], int *new[], int n);
int*** append_result (int ***list, int qnt, int **new, int n);
void free_results (int ***list, int qnt, int n);
void print_report(int, int, int, int*, int, int, int, int);

#endif
