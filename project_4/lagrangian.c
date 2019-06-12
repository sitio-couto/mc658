#include "dcmstp-solver.h"

#define INIT_STEP 2
#define MIN_STEP 0.005
#define MAX_ITER_STEP 30

/**
 * Lagrangian heuristic implementation for DCMSTP.
 * Solutions and dual bounds acquired through Lagrangian Relaxation.
 * @param g Instance graph
 * @param max_time Execution time limit.
 * @return out Best solution and dual bound found within time "max_time"
 */
struct out *lagrangian_heuristic(mat_graph *g, int max_time){
    double step = INIT_STEP;
    int i, iter = 0;  
    struct out *ans;
    int **mat;
    
    // Initializing
    time_t start_time = clock();
    ans = malloc(sizeof(struct out));
    
    // Allocating lagrangian graph.
    mat = malloc(sizeof(int*)*g->n);
    for(i=0; i<g->n; i++)
        mat[i] = malloc(sizeof(int)*g->n);
    
    while (step > MIN_STEP && curr_time(start_time) < max_time){
    }
    
    return NULL;
}

/**
 * Implementation of Prim's Algorithm for constructing Minimum Spanning Trees
 * @param g Graph to generate tree from.
 * @return Minimum spanning tree for graph g.
 */
void mst_prim(int **g){

    
}
