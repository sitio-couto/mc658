/* Victor Ferreira Ferrari  - RA 187890
 * Vinícius Couto Espindola - RA 188815
 * 
 * MC658 - T1 - Exact Branch-and-Bound algorithm for solving FSP.
 * 
 * Receives the instance to be solved, and some extra parameters:
 *  - Maximum execution time for that instance.
 *  - Upper bound on the number of nodes to be explored.
 * 
 * The returned values may not be the best, depending on these parameters.
 * 
 * TODO: Adicionar descrição do algoritmo.
 */

#include "bnb-fs.h"

int main(int argc, char* argv[]){
    int *start_t, *end_t;                    // Arrays for input (start and end times).
    int n_tasks, max_node, max_time, n_nodes = 0;
    int i;
    node *best_solution;
    
    // Input
    n_tasks = read_input(argv, &start_t, &end_t, &max_node, &max_time);

    // Execution
    best_solution = bnb(max_node, max_time, &n_nodes);

    // Output
    printf(" %i %i\n", max_node, max_time);

    for (i = 0; i < n_tasks; ++i)
        printf(" %i |", start_t[i]);

    printf("\n");
    for (i = 0; i < n_tasks; ++i)
        printf(" %i |", end_t[i]);

    return 0;
}

int read_input(char *args[], int **start_t, int **end_t, int *max_node, int *max_time){
    FILE *instance, *parameters;
    int n_tasks, i;
    
    instance = fopen(args[1], "r");
    parameters = fopen(args[2], "r");

    fscanf(parameters, " %i \n %i", max_node, max_time);

    fscanf(instance, " %i", &n_tasks);
    *start_t = malloc(sizeof(int)*n_tasks);
    *end_t = malloc(sizeof(int)*n_tasks);

    for (i = 0; i < n_tasks; ++i)
        fscanf(instance, " %i %i \n", (*start_t)+i, (*end_t)+i);
    
    fclose(instance);
    fclose(parameters); 
    return n_tasks;
}

node* bnb(int max_node, int max_time, int *n_nodes){
    int best_primal = INT_MAX;
    
    return 0x0;
}
