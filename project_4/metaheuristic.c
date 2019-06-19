#include "dcmstp-solver.h"

/**
 * Metaheuristic implementation for DCMSTP.
 * Metaheuristic chosen: ???
 * @param g Instance graph
 * @param max_time Execution time limit.
 * @return out Best solution found within time "max_time"
 */

#define _PERM_ 3

struct out *metaheuristic(mat_graph *g, int max_time){
    time_t start_time = clock();
    int i, j, k, best_primal = INT_MAX;
    heu_graph *state = first_primal(g);
    edge2vert *e = malloc(g->m*sizeof(edge2vert));

    // Initialize list of edges (permutations options)
    k = 0;
    for (i=0; i<_PERM_; ++i) {
        for(j=i+1; j < g->n; ++j) {
            e[k].a = i;
            e[k].b = j; 
            e[k].cost = g->mat[i][j];
            ++k;
        }
    }
    qsort(e, g->m, sizeof(edge2vert), compare);

    while (curr_time(start_time) < max_time) {
        state = heuristic(state, e);
        best_primal = min(best_primal, state->primal);
    }
    
    return NULL;
}

heu_graph* heuristic(heu_graph *r, edge2vert *e) {
    int i, j, k;
    int c1, c2, c3, c4;
    int merge, master;
    int comp[r->n], aux[r->n];
    edge2vert changed[_PERM_];

    // Initilize lists for greedy heuristic
    for(i=0; i < r->n; ++i) {
        // comp[i] = 0;
    }

    // Remove _PERM_ edges from current solution
    k = 0; 
    for (i=(r->m-1); i>=0; --i) {
        // Checks if amount of desired edges have been removed
        if (k == _PERM_) break;
        // checks if edge is in the current solution
        c1 = (r->mst[e[i].a,e[i].b] > -1);

        // Remove edges forming new component
        if (c1) {
            // Remove edge from secondary component
            r->mst[e[i].a][e[i].b] = -1;
            r->mst[e[i].b][e[i].a] = -1;
            r->primal -= e[i].cost;

            // Relax degree constraint on vertices
            ++r->deg[e[i].a]; 
            ++r->deg[e[i].b];

            // Add edge to affected list
            changed[k] = e[i];

            // Tag new component
            tag_component(r->mst, aux, comp, k+1, r->n, e[i].a);
        }
    }

    // Insert _PERM_ edges creating new solution
    k = 0; 
    for (i=0; i<r->m; ++i) {
        // Checks if amount of desired edges have been inserted
        if (k == 0) break;
        // checks if edge is NOT in the current solution
        c1 = !(r->mst[e[i].a,e[i].b] > -1);
        // Checks if degree cosntraint are obeied
        c2 = (r->deg[e[i].a] > 0 && r->deg[e[i].b] > 0);
        // Checks if the edge connects components
        c3 = (comp[e[i].a] != comp[e[i].b]);
        // Checks if not inserting recently removed edge
        c4 = !contains(changed, _PERM_, e[i]);

        // Insert edges forming new solution
        if (c1 && c2 && c3 && c4) {
            // Insert edge
            r->mst[e[i].a][e[i].b] = e[i].cost;
            r->mst[e[i].b][e[i].a] = e[i].cost;
            r->primal += e[i].cost;

            // Update degree constraint on vertices
            --r->deg[e[i].a]; 
            --r->deg[e[i].b];

            // merge component higher ID to lowest ID
            master = min(comp[e[i].a], comp[e[i].b]);
            merge = max(comp[e[i].a], comp[e[i].b]);
            tag_component(r->mst, aux, comp, master, r->n, merge);
        }
    }
}