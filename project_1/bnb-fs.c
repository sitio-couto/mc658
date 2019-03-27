#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  FILE *instance, *parameters;
  int *start_t, *end_t, n_lines, max_node, max_time, i;

  instance = fopen(argv[1], "r");
  parameters = fopen(argv[2], "r");

  fscanf(parameters, " %i \n %i", &max_node, &max_time);

  fscanf(instance, " %i", &n_lines);
  start_t = malloc(sizeof(int)*n_lines);
  end_t = malloc(sizeof(int)*n_lines);

  for (i = 0; i < n_lines; ++i)
    fscanf(instance, " %i %i \n", &start_t[i], &end_t[i]);

  fclose(instance);
  fclose(parameters);

  return 0;
}
