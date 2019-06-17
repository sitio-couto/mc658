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
int max(int a, int b){
    return ((a >= b) ? a:b);
}

/**
 * Calculates value of MST solution.
 * Returns the sum of all edge costs.
 */
float mst_value(int *mst, int size, float **g){
	int i;
	float sum=0;
	
	for(i=0; i<size; i++){
		sum+= g[i][mst[i]];
	}
	
	return sum;
}
