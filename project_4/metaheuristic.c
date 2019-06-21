#include "dcmstp-solver.h"

#define _PERM_ 3

/**
 * Metaheuristic implementation for DCMSTP.
 * Metaheuristic chosen: ???
 * @param g Instance graph
 * @param max_time Execution time limit.
 * @return out Best solution found within time "max_time"
 */

struct out *metaheuristic(mat_graph *g, int max_time, time_t start_time){
    int i, j, k, updates = 0;
    int comp[g->n], deg[g->n]; // Components ID and initial degree constrains
    heu_graph *state = first_primal(g);
    edge_list *e = edge_list_alloc(g->mat, g->n, g->m);
    struct out *best = out_alloc(state->primal, 0, g->n);
    
    // TESTING //
    int hash[(int)best->primal*2];
    for (i=0; i<best->primal*2; ++i) hash[i] = 0;
    ////////////

    for (i=0; i<g->n; ++i){
        comp[i] = 0;
        deg[i] = g->deg[i];
    } 
    
    printf("FIRST=>(%d)\n", (int)best->primal);

    // Iterate trough the solutions graph
    while (curr_time(start_time) < max_time) { 
        heuristic(state, e); // Get new solution
        test_mst(state->mst, deg, state->n, comp);
        if (hash[state->primal]) {
            printf("Old value ==>(%d)\n", state->primal);
        } else {
            hash[state->primal] = 1;
            printf("NEW VALUE!!==>(%d)\n", state->primal);
        }

        if (state->primal < (int)best->primal) {
            best->primal = state->primal;
            to_array(state->mst, state->n, best->mst);
            printf("IHMST=>(%d)\n", (int)best->primal);
            updates++;
        };
    }

    free(e);
    heu_graph_free(state);
    return best;
}

void heuristic(heu_graph *r, edge_list *e) {
    int i, k;
    int c1, c2, c3, c4, c5;
    int start_v, new_id, old_id;
    int comp[r->n], *vacant;
    edge_list changed[_PERM_];

    // Initialization
    for (i=0; i<r->n; ++i) {
        comp[i] = 0;
    }

    // Remove _PERM_ edges from current solution
    k = 0; 
    for (i=(r->m-1); i>=0; --i) {
        // Checks if amount of desired edges have been removed
        if (k == _PERM_) break;

        // checks if edge is in the current solution
        c1 = (r->mst[e[i].a][e[i].b] > -1);

        // Remove edges spliting graph in +1 components
        if (c1) {
            // printf("%d->(%d,%d)\n", k, e[i].a, e[i].b);

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

    // Get the exact amount of vacant degrees per component
    vacant = get_comp_gap(r->n, comp, r->deg, _PERM_);

    // int *hash = calloc(r->n, sizeof(int));
    // printf("_________________\n");
    // for (i=0; i<r->n; ++i) {
    //     if (r->deg[i]) {
    //         hash[i] = 1;
    //         printf("[(%d)-%d|%d]", i+1, comp[i], r->deg[i]);
    //     }
    // }
    // printf("\n");
    // printf("----------------------------\n");

    // Insert _PERM_ edges creating new solution 
    for (i=0; i<r->m; ++i) {
        // Checks if amount of desired edges have been inserted
        if (k == 0) break;
        
        // checks if edge is NOT in the current solution
        c1 = !(r->mst[e[i].a][e[i].b] > -1);
        // Checks if degree cosntraint are obeyed
        c2 = (r->deg[e[i].a] > 0 && r->deg[e[i].b] > 0);
        // Checks if the edge connects components
        c3 = (comp[e[i].a] != comp[e[i].b]);
        // If not last edge, must not constraint both components.
        // This prevents the insertion to saturate components and result in a disjoint graph.
        c4 = (k == 1 || vacant[comp[e[i].a]] > 1 || vacant[comp[e[i].b]] > 1);
        // Checks if is not a re-insertion
        c5 = !contains(changed, _PERM_, e[i]);

        // if (hash[e[i].a] && hash[e[i].b] && c2) 
        //     printf("(%d,%d)->(%d|%d|%d|%d)\n", e[i].a+1, e[i].b+1, c1, c2, c3, c4);
        // Insert edges forming new solution
        if (c1 && c2 && c3 && c4 && c5) {
            // if (contains(changed, _PERM_, e[i])) printf("Re-insertion!!\n");

            // Insert edge
            r->mst[e[i].a][e[i].b] = e[i].cost;
            r->mst[e[i].b][e[i].a] = e[i].cost;
            r->primal += e[i].cost;

            // Update degree constraint on vertices
            --r->deg[e[i].a]; 
            --r->deg[e[i].b];

            // Update vacant degrees per component
            --vacant[comp[e[i].a]]; 
            --vacant[comp[e[i].b]];

            // merge component higher ID to lowest ID
            new_id = min(comp[e[i].a], comp[e[i].b]);
            old_id = max(comp[e[i].a], comp[e[i].b]);
            if (comp[e[i].b] < comp[e[i].a]) start_v = e[i].a;
            else start_v = e[i].a;

            // Update components vacants degrees
            vacant[new_id] += vacant[old_id];
            vacant[old_id] = 0;

            // DFS merge components
            tag_component(r->mst, r->n, start_v, comp, new_id);
            --k;
        }
    }

    if (is_disjoint(comp, r->n)) {
        printf("DISJOINT!!");
    }

    // int count, j;
    // printf("_________________\n");
    // for (i=0; i<r->n; ++i) {
    //     count = 0;
    //     for (j=0; j<r->n; ++j) 
    //         if (r->mst[i][j] >= 0) count++;
        
    //     if (count > 2) printf("%d-(%d|%d)", i, comp[i], r->deg[i]);
    // }
    // printf("\n");
    // free(hash);

    free(vacant);
    return;
}
