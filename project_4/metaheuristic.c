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
    int i, j, k, updates = 0;
    heu_graph *state = first_primal(g);
    edge_list *e = edge_list_alloc(g->mat, g->n, g->m);
    struct out *best = out_alloc(state->primal, 0, g->n);

    int comp[g->n];
    for (i=0; i<g->n; ++i) comp[i];
    // char hash[15696];
    printf("FIRST=>(%d)\n", (int)best->primal);

    // Iterate trough the solutions graph
    while (curr_time(start_time) < max_time) {
        heuristic(state, e); // Get new solution
        // return; /// TESTING ///
        if (state->primal < (int)best->primal) {
            best->primal = state->primal;
            to_array(state->mst, state->n, best->mst);
            printf("IHMST=>(%d)\n", (int)best->primal);
            // test_mst(state->mst, g->deg, state->n, comp);
            updates++;
        };
    }

    free(e);
    heu_graph_free(state);
    return best;
}

void heuristic(heu_graph *r, edge_list *e) {
    int i, k;
    int c1, c2, c3, c4;
    int start_v, new_id;
    int comp[r->n];
    edge_list changed[_PERM_];

    // Initialization
    for (i=0; i<r->n; ++i) comp[i] = 0;
    // test_mst(r->mst, r->deg, r->n, comp);

    // Remove _PERM_ edges from current solution
    k = 0; 
    for (i=(r->m-1); i>=0; --i) {
        // Checks if amount of desired edges have been removed
        if (k == _PERM_) break;

        // checks if edge is in the current solution
        c1 = (r->mst[e[i].a][e[i].b] > -1);

        // Remove edges spliting graph in +1 components
        if (c1) {
            // Remove edge from solution
            r->mst[e[i].a][e[i].b] = -1;
            r->mst[e[i].b][e[i].a] = -1;
            r->primal -= e[i].cost;

            // Relax degree constraint
            ++r->deg[e[i].a]; 
            ++r->deg[e[i].b];

            // Tag edge as changed 
            changed[k] = e[i];

            // Tag new component with new ID 
            start_v = e[i].a;
            new_id = ++k;
            tag_component(r->mst, r->n, start_v, comp, new_id);
        }
    }

    // for (i=0; i<r->n; ++i) {
    //     printf("%d|", comp[i]);
    // }
    // printf("\n");

    // Insert _PERM_ edges creating new solution 
    for (i=0; i<r->m; ++i) {
        // Checks if amount of desired edges have been inserted
        if (k == 0) break;
        
        // checks if edge is NOT in the current solution
        c1 = !(r->mst[e[i].a][e[i].b] > -1);
        // Checks if degree cosntraint are obeied
        c2 = (r->deg[e[i].a] > 0 && r->deg[e[i].b] > 0);
        // Checks if the edge connects components
        c3 = (comp[e[i].a] != comp[e[i].b]);
        // If not last edge, must not constraint both components.
        // This prevents the insertion to saturate vertices and result in a disjoint graph.
        c4 = (k == 1 || (r->deg[e[i].a] > 1 || r->deg[e[i].b] > 1));
        

        // printf("(%d,%d)->(%d|%d|%d|%d)\n", e[i].a+1, e[i].b+1, c1, c2, c3, c4);
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
            new_id = min(comp[e[i].a], comp[e[i].b]);
            if (new_id < comp[e[i].a]) start_v = e[i].a;
            else start_v = e[i].a;
            
            tag_component(r->mst, r->n, start_v, comp, new_id);
            --k;
        }
    }

    // printf("_________________\n");
    // for (i=0; i<r->n; ++i) {
    //     printf("%d|", comp[i]);
    // }
    // printf("\n");
    // test_mst(r->mst, r->deg, r->n, comp);

    return;
}