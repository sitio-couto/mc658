#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  FILE *instance, *parameters;
  int *start_t, *end_t, n_tasks, max_node, max_time, i;

  instance = fopen(argv[1], "r");
  parameters = fopen(argv[2], "r");

  fscanf(parameters, " %i \n %i", &max_node, &max_time);

  fscanf(instance, " %i", &n_tasks);
  start_t = malloc(sizeof(int)*n_tasks);
  end_t = malloc(sizeof(int)*n_tasks);

  for (i = 0; i < n_tasks; ++i)
    fscanf(instance, " %i %i \n", &start_t[i], &end_t[i]);


  printf(" %i %i\n", max_node, max_time);

  for (i = 0; i < n_tasks; ++i)
	printf(" %i |", start_t[i]);

  printf("\n");
  for (i = 0; i < n_tasks; ++i)
	printf(" %i |", end_t[i]);

  fclose(instance);
  fclose(parameters);

  return 0;
}
