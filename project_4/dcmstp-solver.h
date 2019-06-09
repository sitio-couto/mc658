#ifndef DCM
#define DCM 1

#include <stdio.h>
#include <stdlib.h>

// Graph edge
typedef struct edge{
    int idx;
    int cost;
    struct edge *next;
} edge;

// Graph vertex
typedef struct node{
    int deg;
    struct edge *edges;
} node;

// Graph
typedef struct{
    int n,m;
    node *list;
} graph;

graph *read_input(char *filename);
void print_graph(graph *g);
void free_graph(graph *g);

#endif
