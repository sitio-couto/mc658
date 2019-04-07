#include "bnb-fs.h"

int n_tasks;                                  // Amount of tasks read
int max_nodes, max_time;                       // maximum amount of nodes and execution time
task **sorted_id, **sorted_dm1, **sorted_dm2; // tasks sorted by attributes id, dm1 and dm2
float start_time, end_time;

void read_input(char *args[]){
    FILE *instance, *parameters;
    int i, dm1, dm2;

    instance = fopen(args[1], "r");
    parameters = fopen(args[2], "r");

    fscanf(parameters, " %i \n %i", &max_nodes, &max_time);

    fscanf(instance, " %i", &n_tasks);
    sorted_id = (task**)malloc(sizeof(task*)*n_tasks);
    sorted_dm1 = (task**)malloc(sizeof(task*)*n_tasks);
    sorted_dm2 = (task**)malloc(sizeof(task*)*n_tasks);

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

void print_results(int start_time, int end_time, int n_nodes) {
  int i;
  // // Output
  // printf(" %i %i\n", max_nodes, max_time);
  //
  // printf("\nPrinting \"sorted_id\"\n");
  // for (i = 0; i < n_tasks; ++i)
  //   printf("%i->(%i|%i,%i) | ", i, sorted_id[i]->id, sorted_id[i]->dm1, sorted_id[i]->dm2);
  //
  // printf("\nPrinting \"sorted_dm1\"\n");
  // for (i = 0; i < n_tasks; ++i) printf("%i<=", sorted_dm1[i]->dm1);
  //
  // printf("\nPrinting \"sorted_dm2\"\n");
  // for (i = 0; i < n_tasks; ++i) printf("%i<=", sorted_dm2[i]->dm2);

  printf("\n");

  printf("\nBest sum: %d\nResults:\n", best_node->sumf2);
  for (i=0; i< n_tasks; ++i)
      printf("%d: %d | ", i+1, best_node->result[i]);

  printf("\n\nBest primal: %d\n", best_primal);
  printf("Time taken: %.2f\n\n", t_best_primal);

  printf("Best dual: %d\n",best_dual);
  printf("Time taken: %.2f\n\n", t_best_dual);

  printf("Explored nodes: %d\n", n_nodes);
  printf("Total time: %.2f\n", (end_time - start_time)/(float)CLOCKS_PER_SEC);

  // Freeing EVERYTHING.
  while(!heap.empty()){
    free(heap.top());
    heap.pop();
  }
  
  for (i = 0; i < n_tasks; ++i)
      free(sorted_id[i]);
  free(best_node);
  free(sorted_id);
  free(sorted_dm1);
  free(sorted_dm2);

  return;
}

task* add_task(int id, int dm1, int dm2) {
  task *new_task = (task*)malloc(sizeof(task));
  new_task->id = id;
  new_task->dm1 = dm1;
  new_task->dm2 = dm2;
  return new_task;
}

int cmp_dm1(const void *a, const void *b) {
  return ( (*(task**)a)->dm1 -(*(task**)b)->dm1 );
}

int cmp_dm2(const void *a, const void *b) {
   return ( (*(task**)a)->dm2 - (*(task**)b)->dm2 );
}

float curr_time(void){
    return (clock() - start_time)/(float)CLOCKS_PER_SEC;
}
