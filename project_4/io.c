#include "dcmstp-solver.h"

/**
 * Reads input from file to adjacency list.
 * @param filename Path to the instance file.
 * @return graph in adjacency list.
 */
graph *read_input_list(char *filename){
    FILE *in;
    graph *g;
    int start, end, cost, deg;
    edge *new;
    int i;
    
    in = fopen(filename, "r");
    
    // Reads dimensions and creates list.
    g = malloc(sizeof(graph));
    fscanf(in, "%d %d \n", &g->n, &g->m);
    g->list = calloc(g->n, sizeof(node));
    
    // Reads edges and inserts them in the graph.
    for (i=0; i<(g->m); i++){
        fscanf(in, "%d %d %d \n", &start, &end, &cost);
        new = malloc(sizeof(edge));
        new->idx = end-1;
        new->cost = cost;
        new->next = g->list[start-1].edges;
        g->list[start-1].edges = new;
        
        new = malloc(sizeof(edge));
        new->idx = start-1;
        new->cost = cost;
        new->next = g->list[end-1].edges;
        g->list[end-1].edges = new;
    }
    
    // Reads degree constraints
    for (i=0; i<(g->n); i++){
        fscanf(in, "%d %d \n", &start, &deg);
        g->list[start-1].deg = deg;
    }
    
    fclose(in);
    return g;
}

/**
 * Reads input from file to adjacency matrix.
 * @param filename Path to the instance file.
 * @return graph in adjacency matrix.
 */
mat_graph *read_input_matrix(char *filename){
    FILE *in;
    mat_graph *g;
    int start, end, cost, deg;
    int i;
    
    in = fopen(filename, "r");
    
    // Reads dimensions and creates matrix.
    g = malloc(sizeof(mat_graph));
    fscanf(in, "%d %d \n", &g->n, &g->m);
    g->deg = calloc(g->n, sizeof(int));
    g->mat = calloc(g->n, sizeof(int*));
    for (i=0; i<(g->n); i++)
        g->mat[i] = calloc(g->n, sizeof(int));
    
    // Reads edges and inserts them in the graph.
    for (i=0; i<(g->m); i++){
        fscanf(in, "%d %d %d \n", &start, &end, &cost);
        g->mat[start-1][end-1] = cost;
        g->mat[end-1][start-1] = cost;
    }
    
    // Reads degree constraints
    for (i=0; i<(g->n); i++){
        fscanf(in, "%d %d \n", &start, &deg);
        g->deg[start-1] = deg;
    }
    
    fclose(in);
    return g;
}

/**
 * Function that generates .out file with the edges of the best tree found.
 */
void generate_out_file(char *filename, struct out *ans){
    printf("Answer here\n");
    
    if (ans)
		free(ans->mst);
		
    free(ans);
}

/**
 * Auxiliary function: prints current graph.
 */
void print_graph_list(graph *g){
    int i;
    edge *curr;
    
    printf("%d %d \n", g->n, g->m);
    for (i=0; i<g->n; i++){
        printf("%d %d \n", i+1, g->list[i].deg);
        curr = g->list[i].edges;
        while (curr){
            printf("%d %d %d \n", i+1, curr->idx+1, curr->cost);
            curr = curr->next;
        }
    }
}

/**
 * Auxiliary function: prints current graph.
 */
void print_graph_matrix(mat_graph *g){
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
void free_graph_list(graph *g){
    int i;
    edge *curr, *prev;
    
    // Edge freeing
    for (i=0; i<(g->n); i++){
        curr = g->list[i].edges;
        while(curr){
            prev = curr;
            curr = curr->next;
            free(prev);
        }
    }
    
    free(g->list);
    free(g);
}

/**
 * Frees graph memory allocated while reading input.
*/
void free_graph_matrix(mat_graph *g){
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
int max(int a, int b){
    return ((a >= b) ? a:b);
}
