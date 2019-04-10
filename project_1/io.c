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

    if (fscanf(parameters, " %i \n %i", &max_nodes, &max_time) != 2) exit(0);

    if (fscanf(instance, " %i", &n_tasks) != 1) exit(0);
    sorted_id = malloc(sizeof(task*)*n_tasks);
    sorted_dm1 = malloc(sizeof(task*)*n_tasks);
    sorted_dm2 = malloc(sizeof(task*)*n_tasks);

    for (i = 0; i < n_tasks; ++i){
      if (fscanf(instance, " %i %i \n", &dm1, &dm2) != 2) exit(0);
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

  for(i = 0; instance[i] != '/'; ++i);
  printf("%s,%d,%d,%d,%.2f,%.2f,%.2f,", &instance[++i], best_primal, best_dual, n_nodes, t_best_primal, t_best_dual, (end_time-start_time)/(float)CLOCKS_PER_SEC);

  if (best_primal != best_node->primal) printf("FAILURE\n");
  get_best_sched(best_node->result, best_node->f1tr, best_node->f2tr, best_node->sumf2);
  if (!schedule_check()) printf("-----\nERROR\n-----\n");

  printf("{");
  for (i = 0; i < n_tasks; ++i){
    printf("%i", best_node->result[i]);
    if (i != n_tasks-1) printf(",");
    else printf("}");
  }

  printf(",%.2f%%", ((float)pb1_count)*100/((float)(pb2_count + pb1_count)));

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
  if (!in_heap) free(best_node);

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

void get_best_sched(char result[], int f1tr, int f2tr, int sumf2) {
  int i, r, k;
  int f1aux, f2aux, first_bound = INT_MAX, second_bound = INT_MAX;
  char first_sched[32], second_sched[32];

  // Find how many task have been alocated so far (fiding r)
  r = 0;
  for (i = 0; i < n_tasks; ++i){
    if (result[i] > 0) ++r;
    first_sched[i] = result[i];
    second_sched[i] = result[i];
  }

  // Calculate primal bound using sorted_dm1
  k = r;
  f1aux = f1tr;
  f2aux = f2tr;
  first_bound = sumf2;
  for (i = 0; i < n_tasks; ++i) {
    if (result[sorted_dm1[i]->id-1] == 0) {
      first_sched[sorted_dm1[i]->id-1] = ++k;
      f1aux = f1aux + sorted_dm1[i]->dm1;
      if (f1aux > f2aux) f2aux = f1aux + sorted_dm1[i]->dm2;
      else f2aux += sorted_dm1[i]->dm2;
      first_bound += f2aux;
    }
  }

  // Calculate primal bound using sorted_dm2
  // k = r;
  // f1aux = f1tr;
  // f2aux = f2tr;
  // second_bound = sumf2;
  // for (i = 0; i < n_tasks; ++i) {
  //   if (result[sorted_dm2[i]->id-1] == 0) {
  //     second_sched[sorted_dm2[i]->id-1] = ++k;
  //     f1aux = f1aux + sorted_dm2[i]->dm1;
  //     if (f1aux > f2aux) f2aux = f1aux + sorted_dm2[i]->dm2;
  //     else f2aux += sorted_dm2[i]->dm2;
  //     second_bound += f2aux;
  //   }
  // }

  if (first_bound < second_bound) {
    for (i = 0; i < n_tasks; ++i)
      best_node->result[i] = first_sched[i];
  } else {
    for (i = 0; i < n_tasks; ++i)
      best_node->result[i] = second_sched[i];
  }

  return;
}

int schedule_check(void) {
  int next, i, f1tr = 0, f2tr = 0, sumf2 = 0;
  char *best_sched = best_node->result;

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
  return 1;
}
