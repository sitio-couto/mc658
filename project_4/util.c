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
    return (((edge2vert*)a)->cost - ((edge2vert*)b)->cost); 
}

/**
 * DFS for testing if a graph is a tree.
 * Used for checking result correctness.
 */
void test_mst(int **mx, int deg[], int n, int comp[]){
	int i, j;
	int *visited = calloc(n, sizeof(int));

	// Initilize search
	dfs(mx, visited, deg, n, 0, 0);
	
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
void dfs(int **mx, int *visited, int deg[], int n, int v, int p) {
    int i, j, count = 0;
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
            dfs(mx, visited, deg, n, j, v);
        }
    }

    return;
}