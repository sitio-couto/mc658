// FOR TESTING //////////////////////////////////////////
#include "dcmstp-solver.h"

// /* TEST */
// void test_mst(int **mx, int deg[], int n, int comp[]);
// void dfs_test(int **mx, int *flag, int deg[], int n, int v, int p);
// int is_new_result (int *(***list), int *qnt, heu_graph *r);
// int is_equal (int *old[], int *new[], int n);
// int*** append_result (int ***list, int qnt, int **new, int n);
// void free_results (int ***list, int qnt, int n);
// void print_report(int, int, int, int*, int, int, int, int);


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
	int i;
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