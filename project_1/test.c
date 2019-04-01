#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

typedef struct{
  int id;   // Val that represents the order in which the task was read from file
  int dm1;  // Duration of taskID in machine 1
  int dm2;  // Duration of taskID in mcahine 2
} task;

task* add_task(int id, int dm1, int dm2) {
  task *node = malloc(sizeof(task));
  node->id = id;
  node->dm1 = dm1;
  node->dm2 = dm2;
  return node;
}

void main(void) {
  int i;
  task *node = add_task(1, 2, 3);
  task *node2 = add_task(4, 5, 6);
  task** sort = malloc(sizeof(task*));
  sort[0] = node;
  sort[1] = node2;

  for (i = 0; i < 2; ++i) {
    printf("%d\n", sort[i]->id);
  }
}
