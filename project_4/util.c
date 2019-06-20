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
int contains(edge_list arr[], int len, edge_list e) {
	int i;
	for (i=0; i<len; ++i)
		if (arr[i].a == e.a && arr[i].b == e.b) return 1;
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
	int visited[n];

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

// MEMORY HANDLING FUNCTIONS ////////////////////////////
struct out* out_alloc(int primal, int dual, int n) {
	int i;
	struct out* o = malloc(sizeof(struct out));

	o->primal = primal;
	o->dual = dual;
	o->mst = malloc(n*sizeof(int));
	
	return o;
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
 * DFS for testing if a graph is a tree.
 * Used for checking result correctness.
 */
void test_mst(int **mx, int deg[], int n, int comp[]){
	int i;//, j, k, count_deg, gap;
	//int hash[n];
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
    if (count > deg[v]) {
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
