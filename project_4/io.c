#include "dcmstp-solver.h"

/**
 * Reads input from file to adjacency matrix.
 * @param filename Path to the instance file.
 * @return graph in adjacency matrix.
 */
mat_graph *read_input(char *filename){
    FILE *in;
    mat_graph *g;
    int start, end, cost, deg;
    int i;
    
    in = fopen(filename, "r");
    
    // Reads dimensions and creates matrix.
    g = malloc(sizeof(mat_graph));
    if (!fscanf(in, "%d %d \n", &g->n, &g->m)) exit(0);
    g->deg = calloc(g->n, sizeof(int));
    g->mat = calloc(g->n, sizeof(int*));
    for (i=0; i<(g->n); i++)
        g->mat[i] = calloc(g->n, sizeof(int));
    
    // Reads edges and inserts them in the graph.
    for (i=0; i<(g->m); i++){
        if (!fscanf(in, "%d %d %d \n", &start, &end, &cost)) exit(0);
        g->mat[start-1][end-1] = cost;
        g->mat[end-1][start-1] = cost;
    }
    
    // Initializes (i,i) with -1.
    for (i=0; i<(g->n); i++)
		g->mat[i][i] = -1;
    
    // Reads degree constraints
    for (i=0; i<(g->n); i++){
        if (!fscanf(in, "%d %d \n", &start, &deg)) exit(0);
        g->deg[start-1] = deg;
    }
    
    fclose(in);
    return g;
}

/**
 * Function that generates .out file with the edges of the best tree found.
 */
void generate_out_file(char *filename, struct out *ans, int size){
    int i,j;
    int edges=0;
    
    //Finds children for each vertex.
    for(i=0; i<size-1; i++)
		for(j=i; j<size; j++)
			if (ans->mst[j] == i || ans->mst[i] == j){
				printf("%d %d\n", i+1, j+1);
				edges++;
			}
			
	// Test
	if(edges != size-1)
		printf("THIS SHOULDN'T HAPPEN! Out tree has %d edges, should have %d\n", edges, size-1);
}

/**
 * Auxiliary function: prints current graph.
 */
void print_graph(mat_graph *g){
    int i,j;
    int cost;
    
    for (i=0; i< (g->n); i++)
        for (j=0; j<(g->n); j++)
            if ((cost=g->mat[i][j]) > 0)
                printf("%d %d %d\n", i+1, j+1, cost);
    for (i=0; i<(g->n); i++)
        printf("%d %d\n", i+1, g->deg[i]);
}

/**
 * Frees graph memory allocated while reading input.
*/
void free_graph(mat_graph *g){
    int i;
    free(g->deg);
    for (i=0; i<(g->n); i++)
        free(g->mat[i]);
    free(g->mat);
    free(g);
}

/**
 * Auxiliary function to print found MST to stdout.
 */
void print_mst(int *mst, int size, int **g){
	int i;
	for(i=1; i<size; i++){
		printf("%d %d %d\n", i+1, mst[i]+1, g[i][mst[i]]);
	}
}
