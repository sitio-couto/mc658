#ifndef BNBFS
#define BNBFS 1

using namespace std;

//C libraries
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <ctime>

//C++ libraries
#include <queue>
#include <vector>

typedef struct{
  int f1tr;       // End time of last task alocated in machine 1
  int f2tr;       // End time of last task alocated in machine 2
  int sumf2;      // Sum of end  times in M2 so far
  int dual;
  int primal;
  int result[64]; // Indicates the resolved permutation of tasks (pos=task.id | val=first-second...)
} node;

struct comparator
{
    bool operator()(const node* a, const node* b)
    {
        return a->dual > b->dual;
    }
};

typedef struct{
  int id;   // Val that represents the order in which the task was read from file
  int dm1;  // Duration of taskID in machine 1
  int dm2;  // Duration of taskID in mcahine 2
} task;

// GLOBALS
// Set and mantained until end of execution
extern int n_tasks;                                  // Amount of tasks read
extern int max_nodes, max_time;                      // maximum amount of nodes and execution time
extern task **sorted_id, **sorted_dm1, **sorted_dm2; // tasks sorted by attributes id, dm1 and dm2
extern float start_time, end_time;

// Will change during execution
extern node *best_node;                    // best node found so far (best solution node)
extern int best_dual, best_primal;         // best bounds found so far
extern float t_best_dual, t_best_primal;   // time taken for each best bound

// Heap
extern priority_queue<node*, vector<node*>, comparator> heap;

// Reads the input files into arrays (instance) and variables (parameters), and returns the amount of tasks.
void read_input(char *args[]);

// Branch-and-bound algorithm implementation.
void bnb(int *n_nodes);

int primal_bound(int result[], int f1tr, int f2tr, int sumf2);

int dual_bound(int result[], int f1tr, int f2tr, int sumf2);

task* add_task(int, int, int);

node* add_node(node *parent, int id);

void copy_best_node(node*);

node* make_root(void);

void print_results(int, int, int);

float curr_time(void);

int cmp_dm1(const void*, const void*);

int cmp_dm2(const void*, const void*);

#endif
