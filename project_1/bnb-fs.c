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

// Set and mantained until end of execution
int n_tasks;                                  // Amount of tasks read
int max_node, max_time;                       // maximum amount of nodes and execution time
task **sorted_id, **sorted_dm1, **sorted_dm2; // tasks sorted by attributes id, dm1 and dm2
// Will change during execution
int best_dual, best_primal;            // best bounds found so far
float t_best_dual, t_best_primal;      // time taken for each best bound

int main(int argc, char* argv[]){
    int i, n_nodes = 0;     // amount of nodes and current amount of nodes
    node *best_node;    //--> Is it the same as best_primal?

    // Input
    read_input(argv);

    // Execution
    best_node = bnb(&n_nodes);

    // Output
    printf(" %i %i\n", max_node, max_time);

    printf("\nPrinting \"sorted_id\"\n");
    for (i = 0; i < n_tasks; ++i)
      printf("%i->(%i|%i,%i) | ", i, sorted_id[i]->id, sorted_id[i]->dm1, sorted_id[i]->dm2);

    printf("\nPrinting \"sorted_dm1\"\n");
    for (i = 0; i < n_tasks; ++i) printf("%i<=", sorted_dm1[i]->dm1);

    printf("\nPrinting \"sorted_dm2\"\n");
    for (i = 0; i < n_tasks; ++i) printf("%i<=", sorted_dm2[i]->dm2);

    printf("\n");

    for (i = 0; i < n_tasks; ++i) free(sorted_id[i]);
    free(sorted_id);
    free(sorted_dm1);
    free(sorted_dm2);

    return 0;
}

void read_input(char *args[]){
    FILE *instance, *parameters;
    int i, dm1, dm2;

    instance = fopen(args[1], "r");
    parameters = fopen(args[2], "r");

    fscanf(parameters, " %i \n %i", &max_node, &max_time);

    fscanf(instance, " %i", &n_tasks);
    sorted_id = malloc(sizeof(task*)*n_tasks);
    sorted_dm1 = malloc(sizeof(task*)*n_tasks);
    sorted_dm2 = malloc(sizeof(task*)*n_tasks);

    for (i = 0; i < n_tasks; ++i){
      fscanf(instance, " %i %i \n", &dm1, &dm2);
      sorted_id[i] = add_task((i+1), dm1, dm2);
      sorted_dm1[i] = sorted_id[i];
      sorted_dm2[i] = sorted_id[i];
    }

    qsort(sorted_dm1, n_tasks, sizeof(task*), cmp_dm1);
    qsort(sorted_dm2, n_tasks, sizeof(task*), cmp_dm2);

    fclose(instance);
    fclose(parameters);
}

node* bnb(int *n_nodes){
    // int best_primal = INT_MAX;
    // int best_dual = 0;
    // node **active_nodes;                // Size?

    // TODO:
    // Usar heap de minimo como estrutura de dados usando o dual como chave.
    // Neste problema, não há nó raiz. Como começar?
    // O melhor primal e o melhor dual não necessariamente serão do mesmo nó. Como retornamos? Passamos apontador ou criamos um nó ficticio para voltar?
    // Medir tempo (depois).


    return 0x0;
}
