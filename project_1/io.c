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

void print_results(int start_time, int end_time, int n_nodes) {
  int i;
  printf("\n");

  printf("Best primal: %d\n", best_primal);
  printf("Time taken: %.2f\n\n", t_best_primal);

  printf("Best dual: %d\n",best_dual);
  printf("Time taken: %.2f\n\n", t_best_dual);

  printf("Explored nodes: %d\n", n_nodes);
  printf("Total time: %.2f\n", (end_time - start_time)/(float)CLOCKS_PER_SEC);

  // Freeing EVERYTHING.
  for (i = 0; i < n_tasks; ++i)
      free(sorted_id[i]);
  for (i=0; i<size_used; i++)
      free(min_heap[i]);
  free(min_heap);
  free(sorted_id);
  free(sorted_dm1);
  free(sorted_dm2);

  return;
}

task* add_task(int id, int dm1, int dm2) {
  task *new_task = malloc(sizeof(task));
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
