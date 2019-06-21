#include "dcmstp-solver.h"

#define _PERM_ 100
#define _TIME_ 10

/**
 * Metaheuristic implementation for DCMSTP.
 * Metaheuristic chosen: ???
 * @param g Instance graph
 * @param max_time Execution time limit.
 * @return out Best solution found within time "max_time"
 */

struct out *metaheuristic(mat_graph *g, int max_time, time_t start_time){
    int i, j, k;
    int iterations = 0, updates = 0;
    int comp[g->n], deg[g->n]; // Components ID and initial degree constrains
    char **tabus, **timer;
    heu_graph *state = first_primal(g);
    edge_list *e = edge_list_alloc(g->mat, g->n, g->m);
    struct out *best = out_alloc(state->primal, 0, g->n);
    
    // TESTING //
    int*** results = NULL;
    int qnt = 0, nodes = 0;
    ////////////

    // Initializations
    alloc_tabus(&tabus, &timer, g->n);
    for (i=0; i<g->n; ++i){
        comp[i] = 0;
        deg[i] = g->deg[i];
        for (j=0; j<g->n; ++j) {
            tabus[i][j] = 0;
            timer[i][j] = 0;
        }
    } 

    // Iterate trough the solutions graph
    while (curr_time(start_time) < max_time) { 
        // TESTING //
        test_mst(state->mst, deg, state->n, comp);
        if (is_new_result(&results, &qnt, state)) ++nodes;
        /////////////
        heuristic(state, e, tabus, timer); // Get new solution
        if (state->primal < (int)best->primal) {
            best->primal = state->primal;
            to_array(state->mst, state->n, best->mst);
            ++updates;
        }
        update_tabus(tabus, timer, g->n);
        ++iterations;
    }

    // TESTING //
    print_report((int)best->primal, iterations, nodes, updates);
    free_results(results, qnt, state->n);
    /////////////

    free(e);
    heu_graph_free(state);
    return best;
}

void heuristic(heu_graph* r, edge_list* e, char** tabus, char** timer) {
    int i, k;
    int c1, c2, c3, c4, c5;
    int comp[r->n], *vacant;
    edge_list *changed[_PERM_];

    // Initialization
    for (i=0; i<_PERM_; ++i) changed[i] = NULL;
    for (i=0; i<r->n; ++i) comp[i] = 0;

    // Remove _PERM_ edges from current solution
    k = 0; 
    for (i=(r->m-1); i>=0; --i) {
        // Checks if amount of desired edges have been removed
        if (k == _PERM_) break;

        // checks if edge is in the current solution
        c1 = (r->mst[e[i].a][e[i].b] > -1);

        // Remove edges spliting graph in +1 components
        if (c1) {
            // Add to tabu list
            add_tabu(tabus, timer, e[i]);
            remove_edge(r, vacant, comp, e[i], (++k));
        }
    }

    // Get the exact amount of vacant degrees per component
    vacant = get_comp_gap(r->n, comp, r->deg, _PERM_);

    // Insert _PERM_ edges creating new solution 
    for (i=0; i<r->m; ++i) {
        // Checks if amount of desired edges have been inserted
        if (k == 0) break;
        
        // checks if edge is NOT in the current solution
        c1 = !(r->mst[e[i].a][e[i].b] > -1);
        // Checks if degree cosntraint are obeyed
        c2 = (r->deg[e[i].a] > 0 && r->deg[e[i].b] > 0);
        // Checks if the edge connects different components
        c3 = (comp[e[i].a] != comp[e[i].b]);
        // If not last edge, must not constraint both components.
        // This prevents the insertion to saturate components and result in a disjoint graph.
        c4 = (k == 1 || vacant[comp[e[i].a]] > 1 || vacant[comp[e[i].b]] > 1);
        // Checks if it is not a re-insertion
        c5 = !is_tabu(tabus, e[i]);

        // Insert edges forming new solution
        if (c1 && c2 && c3 && c4 && c5) {
            add_tabu(tabus, timer, e[i]);
            insert_edge(r, vacant, comp, e[i]);
            --k;
        }
    }

    // Violate tabu by returning edges in case the only viable edge was 
    // the one removed in this iteration (ensures solution viability).
    if (is_disjoint(comp, r->n)) {
        for (i=0; i<_PERM_; ++i) {
            // In case there are not enough edges to permutated, gets NULL and breaks.
            if (changed[i] == NULL) break; 

            // Checks if degree cosntraint are obeyed
            c2 = (r->deg[changed[i]->a] > 0 && r->deg[changed[i]->b] > 0);
            // Checks if the edge connects different components
            c3 = (comp[changed[i]->a] != comp[changed[i]->b]);

            // Insert edges forming new solution
            if (c2 && c3) {
                insert_edge(r, vacant, comp, *changed[i]);
                --k;
            }  
        }
    }

    free(vacant);
    return;
}

void remove_edge(heu_graph *r, int vacant[], int comp[], edge_list e, int k) {
    int start_v, new_id;
    
    // Remove edge from solution
    r->mst[e.a][e.b] = -1;
    r->mst[e.b][e.a] = -1;
    r->primal -= e.cost;

    // Relax degree constraint
    ++r->deg[e.a]; 
    ++r->deg[e.b];

    // Tag new component with new ID 
    start_v = e.a;
    new_id = k;
    tag_component(r->mst, r->n, start_v, comp, new_id);

    return;
}

void insert_edge(heu_graph *r, int vacant[], int comp[], edge_list e) {
    int start_v, new_id, old_id;
    
    // if (contains(changed, _PERM_, e)) printf("Re-insertion!!\n");

    // Insert edge
    r->mst[e.a][e.b] = e.cost;
    r->mst[e.b][e.a] = e.cost;
    r->primal += e.cost;

    // Update degree constraint on vertices
    --r->deg[e.a]; 
    --r->deg[e.b];

    // Update vacant degrees per component
    --vacant[comp[e.a]]; 
    --vacant[comp[e.b]];

    // merge component higher ID to lowest ID
    new_id = min(comp[e.a], comp[e.b]);
    old_id = max(comp[e.a], comp[e.b]);
    if (comp[e.b] < comp[e.a]) start_v = e.a;
    else start_v = e.a;

    // Update components vacants degrees
    vacant[new_id] += vacant[old_id];
    vacant[old_id] = 0;

    // DFS merge components
    tag_component(r->mst, r->n, start_v, comp, new_id);

    return;
}

////*** TABU SEARCH IMPLEMENTATIONS ***////
/** add_tabu
 * Adds an edge to the list of tabus.
 * Params:
 *  Tabus - Boolean matrix indicating tabu edges.
 *  Timer - Byte matrix indicating remaining time.
 *  e     - Edge to be marked as tabu.
 * Returns: void.
 */
void add_tabu (char **tabus, char **timer, edge_list e) {
    tabus[e.a][e.b] = 1;
    tabus[e.b][e.a] = 1;
    timer[e.a][e.b] = _TIME_;
    timer[e.b][e.a] = _TIME_;
    return;
}

/** is_tabu
 * Verifies if an edge is currently a tabu.
 * Params:
 *  Tabus - Boolean matrix indicating tabu edges.
 *  e     - Edge to be marked as tabu.
 * Returns: True if is tabu, False otherwise.
 */
int is_tabu (char **tabus, edge_list e) {
    return tabus[e.a][e.b] && tabus[e.b][e.a];
}

/** update_tabus
 * Updates tabus timers and flags.
 * Params:
 *  Tabus - Boolean matrix indicating tabu edges.
 *  Timer - Byte matrix indicating remaining time.
 *  n     - Tabus/timers matrix size.
 * Returns: void.
 */
void update_tabus (char** tabus, char** timer, int n) {
    int i, j;

    for (i=0; i<n; ++i) {
        for (j=0; j<n; ++j) {
            if (timer[i][j]) --timer[i][j];
            else tabus[i][j] = 0;
        }
    }

    return;
}