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
    int i, j, iter = 0;
    float *mult;			// Lagrange Multipliers.
    float **lg;				// Lagrangian Graph
    int *mst = NULL;		// Minimum spanning tree: mst[i] is the parent of vertex i.
    struct out *ans;
	
    // Initializing
    time_t start_time = clock();
    ans = malloc(sizeof(struct out));
    
    // Allocating lagrangian graph and multiplier array.
    mult = malloc(sizeof(double)*g->n);
    lg = malloc(sizeof(int*)*g->n);
    for(i=0; i<g->n; i++)
        lg[i] = malloc(sizeof(int)*g->n);
    
    // Initializing lagrange multipliers.
    for(i=0; i<g->n; i++)
		mult[i]=1;
    
    while (step > MIN_STEP && curr_time(start_time) < max_time){
		
		//Generating lagrangian costs
		for (i=0; i<g->n; i++){
			for(j=0; j<g->n; j++){	
				lg[i][j] = g->mat[i][j] + mult[i] + mult[j];
			}
		}
		
		// Solving Lagrangian Primal Problem
		// 1- Solving MST for the lagrangian graph.
		// 2- Calculating dual solution value (MST + mult*deg)
		mst = mst_prim(lg, g->n);
		ans->dual = mst_value(mst, g->n, lg) + mult_deg(mult, g->deg, g->n);
    }
    
    // Freeing lagrangian graph and multiplier array.
    for(i=0; i<g->n; i++)
		free(lg[i]);
	free(lg);
	free(mult);
	
	//print_mst(mst, g->n, g->mat);
	ans->mst = mst;
    
    return ans;
}

/**
 * Implementation of Prim's Algorithm for constructing Minimum Spanning Trees
 * Uses adjacency matrix instead of list.
 * @param g Graph to generate tree from.
 * @return Minimum spanning tree for graph g.
 */
int* mst_prim(float **g, int size){
	
	// Parents[i] has the index of the parent of vertex i in the MST.
	int *parents = malloc(sizeof(int)*size);
	int *values = malloc(sizeof(int)*size);
	char *is_in_mst = calloc(size,sizeof(char));
    int edges, i;
    int vertex;
    
    // Initialization
    for(i=0; i<size; i++) values[i]=INT_MAX;
    
    // Vertex 0 is the first to be chosen
    parents[0] = -1;
    values[0] = 0;
    
    // Main loop: chooses n-1 edges to add.
    for(edges=0; edges<size-1;edges++){
		
		vertex = min_value(values, is_in_mst, size);
		
		// Adds vertex in MST.
		is_in_mst[vertex] = 1;
		
		// Updates neighbors which are not in the MST yet.
		for(i=0; i<size; i++){
			
			// Updates if edge exists (cost>0), isn't in the MST and choice value is higher than the edge cost.
			if(g[vertex][i] && !is_in_mst[i] && g[vertex][i] < values[i]){
				parents[i] = vertex;
				values[i] = g[vertex][i];
			}
		}
	}
    
    free(values);
    free(is_in_mst);
    return parents;
}

/**
 * 
 * Finds index of the minimum value in an array if the corresponding vertex is not in the MST.
 * @param values Comparison values for the graph.
 * @param mst_flag Flags that indicate if each vertex is in the MST.
 * @return index of the minimum valid value.
 */
int min_value(int *values, char *mst_flag, int size){
	int minn, idx=0, i;
	
	minn = INT_MAX;
	for (i=0; i<size; i++){
		if(!mst_flag[i] && values[i] < minn){
			minn = values[i];
			idx = i;
		}
	}
	return idx;
}

/**
 * Calculates independent term in the primal lagrangian problem.
 * Multiplies the lagrangian terms by the degree constraints for each vertex.
 */
float mult_deg(float *mult, int *deg, int size){
	float res=0;
	int i;
	
	for (i=0; i<size; i++){
		res+=mult[i]*deg[i];
	}
	
	return res;
}
