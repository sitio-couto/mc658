#include "bnb-fs.h"

// Creates a root for the state space tree
node* make_root(void) {
  node* root = calloc(1, sizeof(node));
  root->dual = dual_bound(root->result, root->f1tr, root->f2tr, root->sumf2);
  root->primal = primal_bound(root->result, root->f1tr, root->f2tr, root->sumf2);
  return root;
}

// This function creates a child node acording to the parent info
node* add_node(node *parent, node *child, int idx){
  int i, r;

  // Restriction: a task must wait other tasks in M1 (starts after parent-f1tr)
  child->f1tr = parent->f1tr + sorted_id[idx]->dm1;

  // Restriction: a task must end in M1 and wait other taks in M2
  if (child->f1tr > parent->f2tr) { // If no task executinf in M2
    child->f2tr = child->f1tr + sorted_id[idx]->dm2;
  } else { // If theres a task executing in M2
    child->f2tr = parent->f2tr + sorted_id[idx]->dm2;
  }

  // Add end time in M2 for the newly computed task to the current result
  child->sumf2 = parent->sumf2 + child->f2tr;

  // If the task is the n-th alocated in the machines, gets the value n (n-th place)
  // Also copies the order of previously alocated tasks
  r = 1;
  for (i = 0; i < n_tasks; ++i) {
    if (parent->result[i] != 0) ++r;
    child->result[i] = parent->result[i];
  }
  child->result[idx] = r;

  // Add dual_bound for this node to be used as priority key (and primal for prunning)
  child->dual = dual_bound(child->result, child->f1tr, child->f2tr, child->sumf2);
  child->primal = primal_bound(child->result, child->f1tr, child->f2tr, child->sumf2);

  return child;
}

node* alocate_node(node *values) {
  int i;
  node *new_node = malloc(sizeof(node));
  new_node->f1tr = values->f1tr;
  new_node->f2tr = values->f2tr;
  new_node->sumf2 = values->sumf2;
  new_node->dual = values->dual;
  new_node->primal = values->primal;
  for (i = 0; i < n_tasks; ++i)
    new_node->result[i] = values->result[i];
  return new_node;
}
