#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include "bnb-fs.h"
#include "heap.h"

void main(void) {
  int i, j;
  task* tasks[3];
  int n_tasks = 3;
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
}
