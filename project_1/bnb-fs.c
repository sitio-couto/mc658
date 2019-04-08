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

node *best_node;                          // best node found so far (best solution node)
int best_dual = 0, best_primal = INT_MAX; // best bounds found so far
float t_best_dual, t_best_primal;

priority_queue<node*, vector<node*>, comparator> heap;

int main(int argc, char* argv[]){
    int n_nodes=0;  // current amount of explored nodes

    start_time = clock();

    // Input
    read_input(argv);

    // Execution
    best_node = (node*)calloc(1, sizeof(node)); // Initialize empty best node
    heap.push(make_root());
    bnb(&n_nodes);

    end_time = clock();

    print_results(start_time, end_time, n_nodes);

    return 0;
}

void bnb(int *n_nodes){
  node *min_node, *new_node;
  int i;

  // Debug limit changer
  //max_nodes = 100000;
  //float max_time = 0.00050;

  // Get min from heap
  while ( !heap.empty() && *n_nodes < max_nodes && curr_time() < max_time) {
      
    min_node = heap.top();
    heap.pop();
        
    (*n_nodes)++; // Node is maturing

    // Expand min_nodes child nodes
    for (i = 0; i < n_tasks; ++i){
        if(min_node->result[i] == 0) {// if task i not part of solution yet, expand
            new_node = add_node(min_node, i); 
            
            // Check if prune is possible (limitant)
            // If my best solution in this node is worst than a known solution, kill it
            if (new_node->dual > best_primal)
                free(new_node);
            else{
                
                // Try to update optimal primal bound
                if (new_node->primal < best_primal) {
                    best_primal = new_node->primal;
                    t_best_primal = curr_time();
                    copy_best_node(new_node); // TODO: only for debugging
                }

                // Try to update optimal dual bound
                if (new_node->dual > best_dual){
                    best_dual = new_node->dual;
                    t_best_dual = curr_time();
                } 
            
                // Check if prune is possible (optimality)
                // If the primal bound for this node is already optimal, kill it
                if(new_node->primal == new_node->dual)
                    free(new_node);
                else
                    heap.push(new_node);
            }
        }
    }
    
    free(min_node);

    // Complete solution
    if (best_dual == best_primal)
        break;
  }

    return;
}

// Calculates a certain inferior limitant for the node. The higher the inferior
// limitan, the closer to the possible optimal solution it is, and more lilkely
// to be punned as well.
int dual_bound(int result[], int f1tr, int f2tr, int sumf2){
  int first_bound, second_bound; // dual bounds
  int i, r, k, d1tk, d2tk, aux;

  // Find how many task have been alocated so far (fiding r)
  r = 0;
  for (i = 0; i < n_tasks; ++i)
    if (result[i] > 0) ++r;

  // Calculate first bound (machine 2 has ignored overlaps)
  k = r;
  first_bound = sumf2;
  for (i = 0; i < n_tasks; ++i){
    if(result[sorted_dm1[i]->id-1] == 0) {
      d1tk = sorted_dm1[i]->dm1;
      d2tk = sorted_dm1[i]->dm2;
      first_bound += f1tr + (n_tasks - (++k) + 1)*d1tk + d2tk;
    }
  }

  // get best lower bound starter
  i=0;
  while (result[sorted_dm1[i]->id-1] && i < n_tasks-1) ++i;
  if (f2tr > f1tr + sorted_dm1[i]->dm1) aux = f2tr;
  else aux = f1tr + sorted_dm1[i]->dm1;

  // Calculate second bound (machine 1 has ignored overlaps)
  k = r;
  second_bound = sumf2;
  for (i = 0; i < n_tasks; ++i){
    if(result[sorted_dm2[i]->id-1] == 0) {
      d2tk = sorted_dm2[i]->dm2;
      second_bound += aux + (n_tasks - (++k) + 1)*d2tk;
    }
  }

  if (first_bound > second_bound) return first_bound;
  else return second_bound;
}

// Calculates a possible solution for the current node. If the calculated
// solution is good, prunning is more likely.
int primal_bound(int result[], int f1tr, int f2tr, int sumf2){
  int i;

  for (i = 0; i < n_tasks; ++i) {
    if (result[sorted_dm1[i]->id-1] == 0) {
      f1tr = f1tr + sorted_dm1[i]->dm1;
      if (f1tr > f2tr) f2tr = f1tr + sorted_dm1[i]->dm2;
      else f2tr = f2tr + sorted_dm1[i]->dm2;
      sumf2 += f2tr;
    }
  }

  return sumf2;
}
