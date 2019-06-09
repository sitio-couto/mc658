#include "dcmstp-solver.h"

/**
 * Reads input from file.
 * @param filename Path to the instance file.
 * @return graph in adjacency list.
 */
graph *read_input(char *filename){
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
 * Auxiliary function: prints current graph.
 */
void print_graph(graph *g){
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
 * Frees graph memory allocated while reading input.
 */
void free_graph(graph *g){
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
