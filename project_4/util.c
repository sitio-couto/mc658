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
 * Wrapper for the to_array_dfs function..
 */
int* to_array (int **mx, int n, int *arr) {
	int i;
	int visited[n];

	for (i=0; i<n; ++i) arr[i] = -1;

	to_array_dfs(mx, n, visited, 0, arr);

	return arr;
}

/**
 * DFS for converting a adjacency matrix
 * into a node-parent array.
 */
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

void update_comp_gap (int n, int comp[], int deg[], int n_curr, int *curr) {
	int i;
	n_curr++;

	for (i=0; i<n_curr; ++i) curr[i] = 0;
	for (i=0; i<n; ++i) curr[comp[i]] += deg[i];
	
	return;
}

// MEMORY HANDLING FUNCTIONS ////////////////////////////
void alloc_tabus (char*** tabus, char*** timer, int n) {
	int i;

	(*tabus) = malloc(n*sizeof(char*));
	(*timer) = malloc(n*sizeof(char*));
	for (i=0; i<n; ++i) {
		(*tabus)[i] = calloc(n, sizeof(char));
		(*timer)[i] = calloc(n, sizeof(char));
	}
}

void tabus_free (char*** tabus, char*** timer, int n) {
	int i;

	for (i=0; i<n; ++i) {
		free((*tabus)[i]);
		free((*timer)[i]);
	}
	free(*tabus);
	free(*timer);
}

struct out* out_alloc(int primal, int dual, int n) {
	struct out* o = malloc(sizeof(struct out));

	o->primal = primal;
	o->dual = dual;
	o->mst = malloc(n*sizeof(int));
	
	return o;
}

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

// FOR TESTING //////////////////////////////////////////

/**
 * Save obtained MSTs to ensures new nodes of the solutions
 * graph are being visited.
 */
int is_new_result (int *(***list), int *qnt, heu_graph *r) {
	int k;

	for (k=0; k<(*qnt); ++k) {
		if (is_equal((*list)[k], r->mst, r->n)) 
			return 0;
	}

	(*list) = append_result((*list), ++(*qnt), r->mst, r->n);

	return 1;
}

int*** append_result (int ***list, int qnt, int **new, int n) {
	int i, j;
	int ***new_list = malloc(qnt*sizeof(int**));

	memcpy(new_list, list, (qnt-1)*sizeof(int**));
	new_list[qnt-1] = malloc(n*sizeof(int*));
	for (i=0; i<n; ++i) {
		new_list[qnt-1][i] = malloc(n*sizeof(int)); 
		for (j=0; j<n; ++j) {
			new_list[qnt-1][i][j] = new[i][j];
		}
	}

	free(list);
	return new_list;
}

int is_equal (int *old[], int *new[], int n) {
	int i, j;

	for (i=0; i<n; ++i) {
		for (j=0; j<n; ++j) {
			if (old[i][j] != new[i][j])
				return 0; 
		}
	}

	return 1;
}

void free_results (int ***list, int qnt, int n) {
	int i, k;

	for (k=0; k<qnt; ++k) {
		for (i=0; i<n; ++i) {
			free(list[k][i]);
		}
		free(list[k]);
	}

	free(list);
}

/**
 * Print report for the metahuristic local search.
 */
void print_report(int best, int high, int first, int* hash, int n, int iters, int nodes, int updates) {
	int i, sum, avg, reps;
	long long int count;

	reps = 0;
	count = 0;
	sum = 0;
	for (i=0; i<n; ++i) {
		if (hash[i] > 0) reps++;
		count += hash[i];
		sum += i*hash[i];
	}
	avg = (int)floor(sum/count);
	reps = (int)floor(reps/count);
	
	
	printf("   _______________________________________\n");
    printf("  |  Best   |  First  |   Avg   |  High   |\n");
	printf("  |%7d  |%7d  |%7d  |%7d  |\n", best, first, avg, high);
    printf("  |---------------------------------------|\n");
    printf("  | iterations | nodes explored | updates |\n");
    printf("  | %10d | %14d | %7d |  \n", iters, nodes, updates);
    printf("   --------------------------------------- \n");
	return;
}

/**
 * DFS for testing if a graph is a tree.
 * Used for checking result correctness.
 */
void test_mst(int **mx, int deg[], int n, int comp[]){
	int i, j, k, count_deg, gap;
	int hash[n];
	int *visited = calloc(n, sizeof(int));

	// for (i=0; i<n; ++i) hash[i] = 0;
	// for (i=0; i<n; ++i) ++hash[comp[i]];
	// for (i=0; i<n; ++i) {
	// 	if (hash[i]) {
	// 		for (j=0; j<n; ++j) {
	// 			if (comp[j] == i) {
	// 				count_deg = 0;
	// 				for (k=0; k<n; ++k) {
	// 					if (mx[j][k] >= 0) ++count_deg;
	// 				}
	// 				if (count_deg - deg[j] < 0) {
	// 					printf("GAP=>%d\n", i);
	// 					break;
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	// printf("\n");

	// for (i=0; i<n; ++i) {
	// 	for (j=0; j<n; ++j) {
	// 		if (mx[i][j] > -1)
	// 			printf("%d|", mx[i][j]);
	// 		else 
	// 			printf("*|", mx[i][j]);
	// 	}
	// 	printf("\n");
	// }

	// Initilize search
	dfs_test(mx, visited, deg, n, 0, 0);
	
	// Checks if all vertex are reached
	for (i=0; i<n; ++i) {
		if (!visited[i]) {
			printf("FLAWED!! (Not a connected graph)\n");
			exit(0);
		}
	}

	free(visited);
	return;
}

/**
 * DFS for testing if a graph is a tree.
 * Used for checking result correctness.
 */
void dfs_test(int **mx, int *visited, int deg[], int n, int v, int p) {
    int j, count = 0;
    visited[v] = 1;
	// printf("V->(%d)\n",v);

	// Check if the vertex respects the degree contraint
    for (j=0; j < n; ++j) if (mx[v][j] >= 0) ++count;
    if (deg != NULL && count > deg[v]) {
		printf("%d->(%d <= %d)\n", v, count, deg[v]);
        printf("FLAWED!! (degree constraint violated)\n");
        exit(0);
    }

	// Check for loops
    for (j=0; j < n; ++j) {
		// if next vertex is visited and not the parent, theres a cycle.
        if (j!=p && mx[v][j] >= 0 && visited[j]) {
			// for (i=0; i<n; ++i) printf("-(%d)",visited[i]);
			// printf("\n");
			printf("p=(%d)|v->j=(%d,%d)|e=(%d)\n",p,v,j,mx[v][j]);
            printf("FLAWED!! (cycle detected)\n");
            exit(0);
        }
		// Recursive search on next available edge
        else if (j!=p && mx[v][j] >= 0) {
            dfs_test(mx, visited, deg, n, j, v);
        }
    }

    return;
}
