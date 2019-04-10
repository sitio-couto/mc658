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

void print_results(char* instance, int start_time, int end_time, int n_nodes) {
  int i;

  if (!schedule_check()) printf("-----\nERROR\n-----\n");

  for(i = 0; instance[i] != '/'; ++i);
  printf("%s,%d,%d,%d,%.2f,%.2f,%.2f,", &instance[++i], best_primal, best_dual, n_nodes, t_best_primal, t_best_dual, (end_time-start_time)/(float)CLOCKS_PER_SEC);

  printf("{");
  for (i = 0; i < n_tasks; ++i) {
    if(i < n_tasks-1) printf("%i,", best_sched[i]);
    else  printf("%i}", best_sched[i]);
  }

  // printf(",%.2f%%", ((float)pb1_count)*100/((float)(pb2_count + pb1_count)));

  printf("\n");

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

int schedule_check(void) {
  int next, i, f1tr = 0, f2tr = 0, sumf2 = 0;

  for (next = 1; next <= n_tasks; ++next) {
    for (i = 0; i < n_tasks && best_sched[i] != next; ++i);
    f1tr += sorted_id[i]->dm1;
    if (f1tr > f2tr) f2tr = f1tr + sorted_id[i]->dm2;
    else f2tr += sorted_id[i]->dm2;
    sumf2 += f2tr;
  }

  if (sumf2 == best_primal) return 1;
  else {
    printf("got %i and expected %i!\n", sumf2, best_primal);
    return 0;
  }
}
