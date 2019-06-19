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
    int i,j;
    mat_graph *g;
    struct out *o;
    
    // Verifying arguments
    if (argc < 4){
        printf("Usage: dcmstp-solver <instance> <max_time> <method>\n");
        return 1;
    }
    
    printf("Reding input...\n");
    g = read_input_matrix(argv[1]);

    int sum_max = 0;
    for(i=0; i<g->n; ++i){
        for(j=i+1; j<g->n; ++j){
            sum_max += g->mat[i][j];
        }
    }
    printf("MAX=(%d)\n",sum_max);

    printf("Calculating primal...\n");
    first_primal(g);

    double **lg = malloc(sizeof(double*)*g->n);
    for(i=0; i<g->n; i++)
        lg[i] = malloc(sizeof(double)*g->n);
    for (i=0; i<g->n; i++){
        for(j=0; j<g->n; j++){	
            lg[i][j] = g->mat[i][j];
        }
    }

    printf("Calculating dual...\n");
    mst_prim(lg, g->n);

    // // Methods: 'l' = Lagrangian Relaxation. 'm' = Metaheuristic
    // if (argv[3][0] == 'l'){
    //     printf("Lagrangian\n");
    //     o = lagrangian_heuristic(g, atoi(argv[2]));
    //     //printf("%s,%lf,%lf\n", argv[1], o->dual, o->primal);
    // }
    
    for(i=0; i<g->n; i++) {
        free(lg[i]);
        free(g->mat[i]);
    }
    free(lg);
    free(g->mat);
    free(g->deg);
    free(g);

    return 0;
}

int first_primal(mat_graph *g) {
    int i, j, k, main, merge, min_edge;
    int c1, c2, primal = 0;
    int deg[g->n];  // Indicates the remaining edges alowed per vertex
    int comp[g->n]; // Indicates the component of the vertex
    edge2vert *e = malloc(g->m*sizeof(edge2vert));

    k = 0;
    for(i=0; i < g->n; ++i) {
        deg[i] = g->deg[i];
        comp[i] = i;
        for(j=i+1; j < g->n; ++j) {
            e[k].a = i;
            e[k].b = j; 
            e[k].cost = g->mat[i][j];
            ++k;
        }
    }

    min_edge = INT_MAX;
    k = -1;
    for (i=0; i<g->n; ++i) {
        if (deg[i] == 1){
            for (j=0; j<g->n; ++j) {
                if (deg[j] > 1 && g->mat[i][j] < min_edge) {
                    k = j;
                }
            }

            if (k > -1) {
                for (j=0; j<g->m; ++j) {
                    if (e[j].a == i && e[j].b == k) break;
                }
                
                --deg[e[j].a]; 
                --deg[e[j].b];

                main  = min(comp[e[i].a],comp[e[i].b]);
                merge = max(comp[e[i].a],comp[e[i].b]);

                for (j=0; j<g->n; ++j){
                    if (comp[j] == merge) comp[j] = main;
                }

                primal += e[i].cost;
            }
        }
    }


    qsort(e, g->m, sizeof(edge2vert), compare);

    k = 0;
    for (i=0; i < g->m; ++i) {
        c1 = (comp[e[i].a] != comp[e[i].b]);
        c2 = (deg[e[i].a] > 0 && deg[e[i].b] > 0);

        if (c1 && c2) {
            main  = min(comp[e[i].a],comp[e[i].b]);
            merge = max(comp[e[i].a],comp[e[i].b]);

            for (j=0; j<g->n; ++j){
                if (comp[j] == merge) comp[j] = main;
            }

            --deg[e[i].a]; 
            --deg[e[i].b];

            primal += e[i].cost;
            ++k;
        }
        
        if (k == g->n) break;
    }

    printf("HMST=(%d)\n", primal);
    for (i=0; i < g->n; ++i){
        if(comp[i] != 0 || deg[i] < 0) {
            printf("FLAWED(0)=>(%d|%d)!!\n",comp[i], deg[i]);
        }
    }
    
    free(e);
    return primal;
}

int compare(const void * a, const void * b) { 
    return (((edge2vert*)a)->cost - ((edge2vert*)b)->cost); 
}
