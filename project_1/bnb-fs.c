/* Victor Ferreira Ferrari  - RA 187890
 * Vinícius Couto Espindola - RA 188815
 *
 * MC658 - T1 - Exact Branch-and-Bound algorithm for solving FSP.
 *
 * Receives the instance to be solved, and some extra parameters:
 *  - Maximum execution time for that instance.
 *  - Upper bound on the number of nodes to be explored.
 *
 * The returned values may not be the optimal, depending on these parameters.
 *
 * TODO: Adicionar descrição do algoritmo.
 */

#include "bnb-fs.h"

int pb1_count = 0, pb2_count = 0, in_heap = 1;
node *best_node = NULL;
int best_dual = 0, best_primal = INT_MAX;
float t_best_dual, t_best_primal;

int main(int argc, char* argv[]){
    int n_nodes=0;  // current amount of explored nodes

    // Input
    read_input(argv);

    // Execution
    start_time = clock();

    insert_heap(make_root(), n_tasks);   // Insert root node in heap
    bnb(&n_nodes);

    end_time = clock();

    print_results(argv[1], start_time, end_time, n_nodes);

    return 0;
}

void bnb(int *n_nodes){
  node *min_node, *new_node, *aux_node = malloc(sizeof(node));
  int i;

  // Get min from heap
  while ((min_node = remove_min()) != NULL && *n_nodes < max_nodes && curr_time() < max_time) {

    // Try to update optimal dual bound
    if (min_node->dual > best_dual){
      best_dual = min_node->dual;
      t_best_dual = curr_time();

      // Since a min heap is being used, if we reach a node which has a dual bound
      // higher than or equal to the best primal bound, there are no nodes with
      // lower estimates than a known solution. Hence, the best primal is the optimal solution.
      if (best_dual >= best_primal) {
        best_dual = best_primal;
        break;
      }
    }

    // Expand min_nodes child nodes
    for (i = 0; (i < n_tasks) && (*n_nodes < max_nodes); ++i) {
      if(min_node->result[i] == 0) { // if task (i+1) not part of solution yet, expand
        new_node = add_node(min_node, aux_node, i);

        // Limitant prunning
        if (new_node->dual >= best_primal) continue;

        // Check if has new optimal primal bound
        if (new_node->primal < best_primal) {
          best_primal = new_node->primal;
          t_best_primal = curr_time();
          // If it offers a better primal, it becomes the new best node
          if (best_node != NULL && !in_heap)  // Free it only if best node exists and does not point to a heap element
            free(best_node);
          new_node = alocate_node(new_node);
          best_node = new_node;
          in_heap = 1;
        }

        // Remove dominated nodes and, if node is dominated by other node, do not insert.
        if (!check_dominance(new_node)) {
          if (best_node == new_node) in_heap = 0; // If its dominated but still the best, singnalize not in_heap
          continue;
        }

        // Optimality prunning
        if (new_node->primal == new_node->dual) {
          if (best_node == new_node) in_heap = 0; // If its pruned but still the best, singnalize not in_heap
          continue;
        }

        // Checks if node has already been alocated and insert it in the best bound min heap
        (*n_nodes)++;
        if (best_node == new_node) // If new node has already been alocated, insert
            insert_heap(new_node, n_tasks);
        else                       // otherwise, alocate and insert
            insert_heap(alocate_node(new_node), n_tasks);
      }
    }

    if (best_node != min_node){
      free(min_node);
    } else in_heap = 0;
  }

    // If min_node is null there are two possibilities: either ther tree was fully
    // generated, meaning the best primal and dual bounds have already met, or the tree
    // was completly pruned, meanining there is no better solution than ther current best
    // primal bound.
    if (min_node == NULL)
      best_dual = best_primal;

    // If n_nodes >= max_nodes, free last node removed.
    // If NULL, nothing happens.
    if (min_node != NULL && min_node != best_node)
      free(min_node);
    free(aux_node);
    return;
}

// Calculates a certain inferior limitant for the node. The higher the inferior
// limitan, the closer to the possible optimal solution it is, and more lilkely
// to be punned as well.
int dual_bound(char result[], usi f1tr, usi f2tr, usi sumf2){
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
int primal_bound(char result[], usi f1tr, usi f2tr, usi sumf2){
  int i;
  int f1aux, f2aux, first_bound = INT_MAX, second_bound = INT_MAX;


  // Calculate primal bound using sorted_dm1
  f1aux = f1tr;
  f2aux = f2tr;
  first_bound = sumf2;
  for (i = 0; i < n_tasks; ++i) {
    if (result[sorted_dm1[i]->id-1] == 0) {
      f1aux = f1aux + sorted_dm1[i]->dm1;
      if (f1aux > f2aux) f2aux = f1aux + sorted_dm1[i]->dm2;
      else f2aux += sorted_dm1[i]->dm2;
      first_bound += f2aux;
    }
  }

  // Calculate primal bound using sorted_dm2
  // f1aux = f1tr;
  // f2aux = f2tr;
  // second_bound = sumf2;
  // for (i = 0; i < n_tasks; ++i) {
  //   if (result[sorted_dm2[i]->id-1] == 0) {
  //     f1aux = f1aux + sorted_dm2[i]->dm1;
  //     if (f1aux > f2aux) f2aux = f1aux + sorted_dm2[i]->dm2;
  //     else f2aux += sorted_dm2[i]->dm2;
  //     second_bound += f2aux;
  //   }
  // }

  if (first_bound < second_bound) {
    ++pb1_count;
    return first_bound;
  } else {
    ++pb2_count;
    return second_bound;
  }
}

int check_dominance(node *new_node){
  node *heap_node;
  char c1, c2, c3, c4, c5;
  int aux, i, j;

  for (i = 0; i < size_used; ++i) {
    heap_node = min_heap[i];

    c1 = 1; // Must have same set of alocated tasks
    for (j = 0; j < n_tasks; ++j) {
      aux = (new_node->result[j] == 0 && heap_node->result[j] == 0);
      aux = aux ||  (new_node->result[j] != 0 && heap_node->result[j] != 0);
      c1 = (c1 && aux);
      if (!c1) break;
    }
    // If new_node dominates, c2 and c3 must be true.
    c2 = new_node->sumf2 <= heap_node->sumf2; // Must have a smaler sum of finishing times on
    c3 = new_node->f2tr <= heap_node->f2tr;   // Must have a smaller finishing time on machine 2
    // If heap_node dominates, c2 and c3 must be true.
    c4 = new_node->sumf2 >= heap_node->sumf2; // Must have a smaler sum of finishing times on
    c5 = new_node->f2tr >= heap_node->f2tr;   // Must have a smaller finishing time on machine 2

    // Cond must be true for a dominance relation, it indicates if both nodes
    // have a permutation of the same set of alocated tasks so far.
    // if heap_node dominates, returns false and does not alocate new node.
    // else if new_node dominates, remove heap_node from heap and insert new_node.
    if (c1 && c2 && c3) { // if new_node is dominates heap_node, kill it.
      remove_heap(i);
    } else if (c1 && c4 && c5) { // if new_node is dominated, kill it.
      return 0;
    }
  }

  // if the condition for a dominance relation is not fullfilled,
  // returns true inserting the node in the heap.
  return 1;
}
