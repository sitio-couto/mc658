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
#include "heap.h"

// GLOBALS
// Set and mantained until end of execution
int n_tasks;                                  // Amount of tasks read
int max_node, max_time;                       // maximum amount of nodes and execution time
task **sorted_id, **sorted_dm1, **sorted_dm2; // tasks sorted by attributes id, dm1 and dm2
// Will change during execution
node *best_node;                          // best node found so far (best solution node)
int best_solution, t_solution;            // best solution found so far
int best_dual = INT_MAX, best_primal = 0; // best bounds found so far
float t_best_dual, t_best_primal;         // time taken for each best bound

int main(int argc, char* argv[]){
    int i, n_nodes;  // amount of nodes and current amount of nodes

    // Input
    read_input(argv);

    // Execution
    ++n_nodes;
    best_node = calloc(1, sizeof(node)); // Initialize empty node
    insert_heap(best_node, n_tasks);
    bnb(&n_nodes);

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

void bnb(int *n_nodes){
  node *min_node;
  int i, node_primal, node_dual;

  // Get min from heap
  while ((min_node = remove_min()) != NULL) {
    // Get bounds for node
    node_dual = dual_bound(min_node->result, min_node->f1tr, min_node->f2tr, min_node->sumf2);
    node_primal = primal_bound(min_node->result, min_node->f1tr, min_node->f2tr, min_node->sumf2);

    // Try to update optimal bounds
    if (node_dual > best_dual)
      best_dual = node_dual;

    if (node_primal < best_primal) {
      free(best_node);
      best_node = min_node;
      best_primal = node_primal;
    }

    // Check if prune is possible (otimality or limitant)
    // If my best solution in this node is worst than a known solution, kill it
    if (node_dual > best_primal){
      free(min_node);
      continue;
    }

    // If the calculated solution for this node is already optimal, kill it
    if (node_primal == node_dual && min_node == best_node) {
      continue;
    } else {
      free(min_node);
      continue;
    }
    // NOTE: If the solution found is optimal for the node and better than the
    // former best solution, then best_node and best_primal have already been
    // updated and the node cannot be freed (due to NULL pointer in best_node).
    // If it isnt better, then we should free the node, for it has no purpose.

    // Expand min_nodes child nodes
    for (i = 0; i < n_tasks; ++i) {
      if(min_node->result[i] == 0) // if task i not part of solution yet, expand
        insert_heap(add_node(min_node, i), n_tasks);
    }
  }


    // int best_primal = INT_MAX;
    // int best_dual = 0;
    // node **active_nodes;                // Size?

    // TODO:
    // Usar heap de minimo como estrutura de dados usando o dual como chave.
    // Neste problema, não há nó raiz. Como começar?
    // O melhor primal e o melhor dual não necessariamente serão do mesmo nó. Como retornamos? Passamos apontador ou criamos um nó ficticio para voltar?
    // Medir tempo (depois).


    return;
}

int dual_bound(int result[], int f1tr, int f2tr, int sumf2){
  int first_bound, second_bound; // dual bounds
  int i, k, d1tk, d2tk, aux;

  // Find how many task have been alocated so far (fiding r for k = r+1)
  k = 0;
  for (i = 0; i < n_tasks; ++i) if (result[i] > 0) ++k;

  // Calculate first bound (machine 2 has ignored overlaps)
  first_bound = sumf2;
  for (i = 0; i < n_tasks; ++i){
    if(result[sorted_dm1[i]->id]) {
      ++k;
      d1tk = sorted_dm1[i]->dm1;
      d2tk = sorted_dm1[i]->dm2;
      first_bound += f1tr + (n_tasks - k + 1)*d1tk + d2tk;
    }
  }

  // get best lower bound starter
  while (result[sorted_dm1[i]->id]) ++i;
  if (f2tr > f1tr + sorted_dm1[i]->dm1) aux = f2tr;
  else aux = f1tr + sorted_dm1[i]->dm1;

  // Calculate second bound (machine 1 has ignored overlaps)
  second_bound = sumf2;
  for (i = 0; i < n_tasks; ++i){
    if(result[sorted_dm2[i]->id]) {
      ++k;
      d2tk = sorted_dm2[i]->dm2;
      first_bound += aux + (n_tasks - k + 1)*d2tk;
    }
  }

  if (first_bound > second_bound) return first_bound;
  else return second_bound;
}

int primal_bound(int result[], int f1tr, int f2tr, int sumf2){
  int i;

  for (i = 0; i < n_tasks; ++i) {
    if (result[sorted_dm1[i]->id] == 0) {
      f1tr = f1tr + sorted_dm1[i]->dm1;
      if (f1tr > f2tr) f2tr = f1tr + sorted_dm1[i]->dm2;
      else f2tr = f2tr + sorted_dm1[i]->dm2;
      sumf2 += f2tr;
    }
  }

  return sumf2;
}


// This function creates a new node acording to the parent info
node* add_node(node *parent, int id){
  node *new_node = malloc(sizeof(node));
  int i, r;

  new_node->f1tr = parent->f1tr + sorted_id[id]->dm1;

  if (new_node->f1tr > parent->f2tr) {
    new_node->f2tr = new_node->f1tr + sorted_id[id]->dm1;
  } else {
    new_node->f2tr = parent->f2tr + sorted_id[id]->dm1;
  }

  new_node->sumf2 = parent->sumf2 + new_node->f2tr;

  r = 0;
  for (i = 0; i < n_tasks; ++i) {
    if (parent->result[i] != 0) ++r;
    new_node->result[i] = parent->result[i];
  }
  new_node->result[id] = r+1;

  return new_node;
}
