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
    struct out *o;
    
    // Verifying arguments
    if (argc < 4){
        printf("Usage: dcmstp-solver <instance> <max_time> <method>\n");
        return 1;
    }
    
    //g = read_input_list(argv[1]);
    g = read_input_matrix(argv[1]);
    
    first_primal(g);

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

int first_primal(mat_graph *g) {
    int i, j, k;
    int c1, c2, primal = 0;
    int v[g->n];
    int deg[g->n];
    edge2vert e[g->m];

    k = 0;
    for(i=0; i < g->n; ++i) {
        v[i] = 0;
        deg[i] = g->deg[i];
        for(j=i+1; j < g->n; ++j) {
            if (g->mat[i][j] <= 0) continue;
            e[k].a = i;
            e[k].b = j; 
            e[k].cost = g->mat[i][j];
            ++k;
        }
    }

    qsort(e, g->m, sizeof(edge2vert), compare);

    k = 0;
    for (i=0; i < g->m; ++i) {
        // printf("(%d,%d)->%d\n",e[i].a,e[i].b,e[i].cost);
        c1 = (!v[e[i].a] || !v[e[i].b]);
        c2 = (deg[e[i].a] > 0 && deg[e[i].b] > 0);

        if (c1 && c2) {
            if (!v[e[i].a]) {
                v[e[i].a] = 1;
                ++k;
            }
            if (!v[e[i].b]){
                v[e[i].b] = 1;
                ++k;
            }
            --deg[e[i].a]; 
            --deg[e[i].b];

            primal += e[i].cost;
        }
        
        if (k == g->n) break;
    }

    printf("HMST=(%d)\n", primal);
    for (i=0; i < g->n; ++i){
        if(v[i] == 0 || deg[i] < 0) printf("FLAWED!!\n");
    }
    
    return primal;
}

int compare(const void * a, const void * b) { 
    return (((edge2vert*)a)->cost - ((edge2vert*)b)->cost); 
}