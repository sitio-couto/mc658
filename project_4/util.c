#include "dcmstp-solver.h"

/**
 * Calculates elapsed time in seconds.
 * @param start_time. Clock value from start of execution.
 */
double curr_time(time_t start_time){
    return (clock() - start_time)/((double)CLOCKS_PER_SEC);
}

/**
 * Returns minimum value between a and b.
 */
int min(int a, int b){
    return ((a < b) ? a:b);
}

int min_array_idx(int *arr, int n){
    int i, minn = INT_MAX, idx=0;
    for(i=0; i<n; i++){
        if (arr[i] < minn){
            minn = arr[i];
            idx = i;
        }
    }
    return idx;
}

/**
 * Returns maximum value between a and b.
 */
double max(double a, double b){
    return ((a >= b) ? a:b);
}

/**
 * Calculates value of MST solution.
 * Returns the sum of all edge costs.
 */
double mst_value(int *mst, int size, double **g){
    int i;
    double sum=0;
    
    for(i=0; i<size; i++)
        if (mst[i] >= 0)
            sum+= g[i][mst[i]];
    
    return sum;
}

/**
 * Calculates value of MST solution.
 * Returns the sum of all edge costs.
 */
int mst_value_int(int *mst, int size, int **g){
    int i;
    int sum=0;
    
    for(i=0; i<size; i++)
        if (mst[i] >= 0)
            sum+= g[i][mst[i]];
    
    return sum;
}

/**
 * Compares values returning their difference.
 * For sorting edges lists with qsort.
 */
int compare(const void * a, const void * b) { 
    return (((edge_list*)a)->cost - ((edge_list*)b)->cost); 
}

/**
 * Checks if a value e is in an array
 * of length n.
 */
int contains(edge_list *arr[], int len, edge_list e) {
    int i;
    for (i=0; i<len; ++i){
        if (arr[i] && arr[i]->a == e.a && arr[i]->b == e.b) 
            return 1;
    }
    return 0;
}

/**
 * Check if theres more than one ID in the
 * components array.
 */
int is_disjoint(int comp[], int n) { 
    int i, aux = comp[0];
    for (i=0; i<n; ++i) if (comp[i] != aux) return 1;
    return 0;
}

/**
 * Checks if graph is connected
 * Uses DFS.
 */
int is_connected(int **g, int *visited, int size){
    int i;
    
    for (i=0; i<size; i++){
        visited[i] = 0;
    }
    
    connected_dfs(g, visited, size, 0);
    
    return visited[min_array_idx(visited, size)];
}

void connected_dfs(int **g, int *visited, int n, int v){
    int j;
    
    visited[v]=1;
    for(j=0; j<n; j++){
        if(!visited[j] && g[v][j])
            connected_dfs(g, visited, n, j);
    }
}

/**
 * Wrapper for the to_array_dfs function.
 * DFS for converting a adjacency matrix
 * into a node-parent array.
 */
int* to_array (int **mx, int n, int *arr) {
    int i;
    int *visited = calloc(n, sizeof(int));

    for (i=0; i<n; ++i) arr[i] = -1;

    to_array_dfs(mx, n, visited, 0, arr);

    free(visited);
    return arr;
}

void to_array_dfs (int **mx, int n, int *visited, int v, int *arr) {
    int i;

    visited[v] = 1;
    for (i=0; i < n; ++i) {
        if (mx[v][i] >= 0 && !visited[i]) {
            arr[i] = v; 
            to_array_dfs(mx, n, visited, i, arr);
        }
    }

    return;
}

/**
 * DFS for tagging a new graph component.
 * Used for local search heuristic.
 */
void tag_component (int **mx, int n, int v, int *comp, int tag) {
    int i, visited[n];

    for (i=0; i<n; ++i) visited[i] = 0;

    tag_component_dfs(mx, n, visited, v, comp, tag);

    return;
}

void tag_component_dfs (int **mx, int n, int *visited, int v, int *comp, int tag) {
    int i;

    comp[v] = tag;

    visited[v] = 1;
    for (i=0; i < n; ++i) {
        if (mx[v][i] >= 0 && !visited[i]) 
            tag_component_dfs(mx, n, visited, i, comp, tag);
    }

    return;
}

/**
 * Calculates the amount of vacant degrees for each component.
 * Used for checking if a component is saturated or not.
 */
int* get_comp_gap (int n, int comp[], int deg[], int n_comp) {
    int i;

    n_comp++;
    int *comp_deg = malloc(n_comp*sizeof(int));

    for (i=0; i<n_comp; ++i) comp_deg[i] = 0;
    for (i=0; i<n; ++i) comp_deg[comp[i]] += deg[i];
    
    return comp_deg;
}

/**
 * Merge vacant degrees from two distinct components. Used when
 * inserting edges in a disjoint graph.
 */
void update_comp_gap (int n, int comp[], int deg[], int n_curr, int *curr) {
    int i;
    n_curr++;

    for (i=0; i<n_curr; ++i) curr[i] = 0;
    for (i=0; i<n; ++i) curr[comp[i]] += deg[i];
    
    return;
}

////// MEMORY HANDLING FUNCTIONS //////

/**
 * Alocate matrix to keep the edges as tabus and their
 * respective timers (count down to remove tabu).
 */
void alloc_tabus (char*** tabus, int*** timer, int n) {
    int i;

    (*tabus) = malloc(n*sizeof(char*));
    (*timer) = malloc(n*sizeof(int*));
    for (i=0; i<n; ++i) {
        (*tabus)[i] = calloc(n, sizeof(char));
        (*timer)[i] = calloc(n, sizeof(int));
    }
}

void tabus_free (char*** tabus, int*** timer, int n) {
    int i;

    for (i=0; i<n; ++i) {
        free((*tabus)[i]);
        free((*timer)[i]);
    }
    free(*tabus);
    free(*timer);
}

void reset_tabus (char*** tabus, int*** timer, int n) {
    int i, j;

    for (i=0; i<n; ++i) {
        for (j=0; j<n; ++j) {
            (*tabus)[i][j] = 0;
            (*timer)[i][j] = 0;
        }
    }

    return;
}

/**
 * Allocate struct with the best primal/dual found and a
 * parent array matrix representing the resulting mst.
 */
struct out* out_alloc(int primal, int dual, int n) {
    struct out* o = malloc(sizeof(struct out));

    o->primal = primal;
    o->dual = dual;
    o->mst = malloc(n*sizeof(int));
    
    return o;
}

/**
 * Alocate struct which will keep all the usefull
 * info for the heuristic and metaheuristic solutions.
 */
heu_graph* heu_graph_alloc (mat_graph *g) {
    int i, j;

    heu_graph *r = malloc(sizeof(heu_graph));
    r->primal = 0;
    r->n = g->n;
    r->m = g->m;
    r->deg = malloc(g->n*sizeof(int));
    r->mst = malloc(g->n*sizeof(int*));
    for (i=0; i<g->n; ++i) {
        r->deg[i] = g->deg[i];
        r->mst[i] = malloc(g->n*sizeof(int));
        for (j=0; j<g->n; ++j) r->mst[i][j] = -1;
    }

    return r;
}

void heu_graph_free (heu_graph *hg) {
    int i;

    free(hg->deg);
    for (i=0; i<hg->n; ++i) free(hg->mst[i]);
    free(hg->mst);
    free(hg);

    return;
}

/**
 * Allocate and initialize list of edges sorted by their
 * costs (used for heuristics).
 */
edge_list* edge_list_alloc (int **mx, int n, int m) {
    int i, j, k = 0;
    edge_list *e = malloc(m*sizeof(edge_list));

    // Initialize list of edges (permutations options)
    k = 0;
    for (i=0; i<n; ++i) {
        for(j=i+1; j < n; ++j) {
            e[k].a = i;
            e[k].b = j; 
            e[k].cost = mx[i][j];
            ++k;
        }
    }
    qsort(e, m, sizeof(edge_list), compare);

    return e;
} 
