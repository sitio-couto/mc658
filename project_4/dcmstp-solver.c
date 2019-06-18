/* MC658 (Algorithms III) - P4 - Heuristic Algoritms for DCMSTP.
 * DCMSTP: Degree Constrained Minimum Spanning Tree.
 * Problem details: https://en.wikipedia.org/wiki/Degree-constrained_spanning_tree
 * 
 * Receives the instance to be solved, maximum execution time and solving method.
 * 
 * Authors:
 * Victor Ferreira Ferrari  - RA 187890
 * Vinícius Couto Espindola - RA 188815
 * University of Campinas - 26/06/2019
*/

#include "dcmstp-solver.h"

int main(int argc, char *argv[]){
    //graph *g;
    mat_graph *g;
    heu_graph *hg;
    struct out *o;
    
    // Verifying arguments
    if (argc < 4){
        printf("Usage: dcmstp-solver <instance> <max_time> <method>\n");
        return 1;
    }
    
    g = read_input_matrix(argv[1]);
    
    hg = first_primal(g);
    printf("HMST=(%d)\n", hg->primal);
    exit(0);

    // Methods: 'l' = Lagrangian Relaxation. 'm' = Metaheuristic
    if (argv[3][0] == 'l'){
        printf("Lagrangian\n");
        o = lagrangian_heuristic(g, atoi(argv[2]));
        //printf("%s,%lf,%lf\n", argv[1], o->dual, o->primal);
    }
    else if (argv[3][0] == 'm'){
        printf("Metaheuristics\n");
        o = metaheuristic(g, atoi(argv[2]));
        //printf("%s,%lf\n", argv[1], o->primal);
    }
    else{
        printf("Invalid method.\nMethods: 'l' = Lagrangian Relaxation. 'm' = Metaheuristic\n");
        return 1;
    }
    
    generate_out_file(argv[1], o);
    //print_graph_list(g);
    //print_graph_matrix(g);
    //free_graph_list(g);
    free_graph_matrix(g);
    
    return 0;
}

heu_graph* first_primal(mat_graph *g) {
    int i, j, k, component, merge;
    int c1, c2;
    int deg[g->n];  // Indicates the remaining edges alowed per vertex
    int comp[g->n]; // Indicates the component of the vertex
    edge2vert *e = malloc(g->m*sizeof(edge2vert)); // List of edges for greedy algrithims

    // Alocate resulting struct with primal and mst
    heu_graph *result = malloc(sizeof(heu_graph));
    result->primal = 0;
    result->n = g->n;
    result->deg = malloc(g->n*sizeof(int));
    result->mst = malloc(g->n*sizeof(int*));
    for (i=0; i<g->n; ++i) {
        result->deg[i] = g->deg[i];
        result->mst[i] = malloc(g->n*sizeof(int*));
        for (j=0; j<g->n; ++j) result->mst[i][j] = -1;
    }

    // Initilize lists for greedy heuristic
    k = 0;
    for(i=0; i < g->n; ++i) {
        deg[i] = g->deg[i]; // Max vertices degrees
        comp[i] = i;        // Vertex component ID
        for(j=i+1; j < g->n; ++j) {
            e[k].a = i;
            e[k].b = j; 
            e[k].cost = g->mat[i][j];
            ++k;
        }
    }

    // Sort edges in ascending weight order
    qsort(e, g->m, sizeof(edge2vert), compare);

    // Insert edges in ascending order avoiding cycles and respecting degrees
    for (i=0; i < g->m; ++i) {
        c1 = (comp[e[i].a] != comp[e[i].b]); // Vertices must be in different components
        c2 = (deg[e[i].a] > 0 && deg[e[i].b] > 0); // Degrees constraint must be respected

        if (c1 && c2) {
            component  = min(comp[e[i].a],comp[e[i].b]);
            merge = max(comp[e[i].a],comp[e[i].b]);
            
            // Merge components
            for (j=0; j<g->n; ++j) {
                if (comp[j] == merge) comp[j] = component;
            }

            // Decrement vertices avalable connections
            --deg[e[i].a]; 
            --deg[e[i].b];

            // Add edge to result struct
            result->mst[e[i].a][e[i].b] = e[i].cost;
            result->mst[e[i].b][e[i].a] = e[i].cost;
            result->primal += e[i].cost;
        }
    }

    test_mst(result->mst, result->deg, result->n);

    free(e);
    return result;
}

// void local_search() {
    
// }