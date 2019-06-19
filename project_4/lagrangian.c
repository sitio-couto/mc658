#include "dcmstp-solver.h"

#define INIT_PI 2
#define MIN_PI 0.005
#define MAX_ITER_PI 30

/**
 * Lagrangian heuristic implementation for DCMSTP.
 * Solutions and dual bounds acquired through Lagrangian Relaxation.
 * @param g Instance graph
 * @param max_time Execution time limit.
 * @return out Best solution and dual bound found within time "max_time"
 */
struct out *lagrangian_heuristic(mat_graph *g, int max_time){
    double pi = INIT_PI, step;
    int i, j, iter = 0;
    double *mult;			// Lagrange Multipliers.
    double **lg;			// Lagrangian Graph
    int *mst = NULL;		// Minimum spanning tree: mst[i] is the parent of vertex i.
    double dual, primal;	// Current dual or primal.
    double *subgrad, subgrad_sum=0;		
    struct out *ans;
	
    // Initializing
    time_t start_time = clock();
    ans = malloc(sizeof(struct out));
    
    // Allocating lagrangian graph and multiplier array.
    mult = malloc(sizeof(double)*g->n);
    lg = malloc(sizeof(double*)*g->n);
    for(i=0; i<g->n; i++)
        lg[i] = malloc(sizeof(double)*g->n);
    
    // Initializing lagrange multipliers.
    for(i=0; i<g->n; i++)
		mult[i]=1;
    
    subgrad = malloc(sizeof(double)*g->n);
    
    ans->dual = 0;
    ans->primal = first_primal(g)->primal;
    
    // End conditions: pi too small, too much time elapsed and optimum found.
    while (pi > MIN_PI && curr_time(start_time) < max_time && (ans->primal - ans->dual) >= 1){
		
		//Generating lagrangian costs
		for (i=0; i<g->n; i++){
			for(j=0; j<g->n; j++){
				if (g->mat[i][j] >= 0)
					lg[i][j] = g->mat[i][j] + mult[i] + mult[j];
			}
		}
		
		// Solving Lagrangian Primal Problem
		// 1- Solving MST for the lagrangian graph.
		// 2- Calculating dual solution value (MST - mult*deg)
		free(mst);
		mst = mst_prim(lg, g->n);
		dual = mst_value(mst, g->n, lg) - mult_deg(mult, g->deg, g->n);
		
		iter++;
		if (dual > ans->dual){
			ans->dual = dual;
			iter = 0;
			printf("%lf\n", ans->dual);
		}
		
		// If the dual hasn't increased in a few iterations, take half pi.
		else if (iter == MAX_ITER_PI)
			pi/=2;
		
		// Subgradients for lagrange multipliers step.
		subgrad_sum = 0;
		for(i=0; i<g->n; i++){
			subgrad[i] = subgradient(i, g->deg[i], g->n, mst);
			if (subgrad[i] > 0 || mult[i] > 0)
				subgrad_sum+= subgrad[i]*subgrad[i];
		}
		
		// Finished execution if Gi=0 for every i.
		// If solution is viable, it's the optimum.
		if (subgrad_sum == 0){
			if (check_viability(g->n, g->deg, mst))
				ans->primal = ans->dual;
			break;
		}
			
		step = pi*(1.05*ans->primal - ans->dual)/subgrad_sum;
		
		// Updating lagrange multipliers
		for (i=0; i< g->n; i++)
			mult[i] = max(0, mult[i]+step*subgrad[i]);
		
		//for(i=0; i< g->n; i++)
		//	printf("%lf ", mult[i]);
		//printf("\n");
		//getchar();
    }
    
    // Freeing lagrangian graph and multiplier array.
    for(i=0; i<g->n; i++)
		free(lg[i]);
	free(lg);
	free(mult);
	free(subgrad);
	
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
int* mst_prim(double **g, int size){
	
	// Parents[i] has the index of the parent of vertex i in the MST.
	int *parents = malloc(sizeof(int)*size);
	double *values = malloc(sizeof(double)*size);
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
			if(g[vertex][i] >= 0 && !is_in_mst[i] && g[vertex][i] < values[i]){
				parents[i] = vertex;
				values[i] = g[vertex][i];
			}
		}
	}
    
	//printf("PRIM=(%lf)\n", mst_value(parents, size, g));

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
int min_value(double *values, char *mst_flag, int size){
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
double mult_deg(double *mult, int *deg, int size){
	double res=0;
	int i;
	
	for (i=0; i<size; i++){
		res+=mult[i]*deg[i];
	}
	
	return res;
}

/**
 * Calculates subgradients for the relaxed constraint.
 * Given by the difference between the max degree of the vertex and the amount of neighbors in the MST.
 */
double subgradient(int v, int deg, int size, int *mst){
	int count=0, i;
	
	// Checking if it has parent.
	if (mst[v] > -1)
		count++;
	
	// Checking children.
	for (i=1; i<size; i++)
		if (mst[i] == v)
			count++;
			
	return count - deg;
}

/**
 * Checks if the MST found is viable for DCMSTP.
 * @param g Original graph
 * @param mst Tree to be checked.
 * @return 1 if viable, 0 otherwise
 */
int check_viability(int size, int *r_deg, int *mst){
	int i;
	int *degs = calloc((short)size, sizeof(int));
	int res = 1;
	
	// Calculating degrees.
	for(i=0; i<size; i++){
		if (mst[i] >= 0){
			degs[mst[i]]++;
			degs[i]++;
		}
	}
	
	// Checking restrictions.
	for(i=0; i<size; i++){
		if (degs[i] > r_deg[i]){
			res = 0;
			break;
		}
	}
	
	free(degs);
	return res;
}
