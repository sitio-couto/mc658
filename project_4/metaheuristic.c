#include "dcmstp-solver.h"

int perm = 3;
int max_cost;

/**
 * Metaheuristic implementation for DCMSTP.
 * Metaheuristic chosen: ???
 * @param g Instance graph
 * @param max_time Execution time limit.
 * @return out Best solution found within time "max_time"
 */

struct out *metaheuristic(mat_graph *g, int max_time, time_t start_time){
    int i, j;
    char **tabus, **timer;
    int threshold = 100, stagnated = 0;
    heu_graph *state = first_primal(g);
    edge_list *e = edge_list_alloc(g->mat, g->n, g->m);
    struct out *best = out_alloc(state->primal, 0, g->n);

    // TESTING //
    int iterations = 1, updates = 0;
    int comp[g->n], deg[g->n];
    int n_hash = 0;
    for (i=g->m-1; i>=(g->m-g->n); --i) n_hash += e[i].cost; 
    int*** results = NULL;
    int qnt = 0, nodes = 0;
    int max_primal = state->primal;
    int first_primal = state->primal;
    int *hash = calloc(n_hash, sizeof(int));
    hash[first_primal] = 1;
    for (i=0; i<g->n; ++i){
        comp[i] = 0;
        deg[i] = g->deg[i];
    } 
    /////////////

    // PARAMS //
    max_cost = e[g->m-1].cost;
    ////////////

    // Initializations
    alloc_tabus(&tabus, &timer, g->n);

    // Iterate trough the solutions graph
    while (curr_time(start_time) < max_time) { 
        // TESTING //
        test_mst(state->mst, deg, state->n, comp);
        if (is_new_result(&results, &qnt, state)) ++nodes;
        if (!hash[state->primal]) {
            if (state->primal > max_primal)
                max_primal = state->primal;
        }
        hash[state->primal]++;
        ++iterations;
        /////////////
        heuristic(state, e, tabus, timer); // Get new solution
        if (state->primal < (int)best->primal) {
            best->primal = state->primal;
            to_array(state->mst, state->n, best->mst);
            ++updates; // TESTING //
        } else if (++stagnated > threshold) {
            stagnated = 0;
            perm = min(perm+1, (int)floor((g->n)/2));
        }
        update_tabus(tabus, timer, g->n);
    }

    // TESTING //
    print_report((int)best->primal, max_primal, first_primal, hash, n_hash, iterations, nodes, updates);
    free_results(results, qnt, state->n);
    free(hash);
    /////////////

    free(e);
    tabus_free(&tabus, &timer, g->n);
    heu_graph_free(state);
    return best;
}

/** heuristic
 *  Applies a local search visiting the neighboors in the solutions graph by
 *  removing certain edges from the current solution and inserting new ones.
 *  In order to prevent stagnation in a local optima, uses tabu search params.
 *  Params:
 *  r     - Struct containing data of the current solution.
 *  e     - List of available edges to be removed/inserted.
 *  tabus - Matrix marking edges which are considered tabu (must not be inserted).
 *  timer - Matrix containing remaining number of iterations for the tabus.
 *  Return: void.
 */
void heuristic(heu_graph* r, edge_list* e, char** tabus, char** timer) {
    int i, count;
    int comp[r->n], *vacant;

    // Initialization
    for (i=0; i<r->n; ++i) comp[i] = 0; // Tag graph as connected.
    count = 0; // Set count of removed edged to zero.

    // Remove "perm" edges from current solution 
    for (i=(r->m-1); (i>=0 && count<perm) ; --i) {
        // Check if edge is in solution and remove it (splits graph in +1 components)
        if (r->mst[e[i].a][e[i].b] > -1) 
            remove_edge(r, vacant, comp, e[i], (++count));
    }

    // Get the exact amount of vacant degrees per component
    vacant = get_comp_gap(r->n, comp, r->deg, perm);

    // Insert "perm" edges creating new solution 
    for (i=0; (i<r->m && count>0); ++i) {
        // Insert edges forming new solution
        if (viable_insertion(r, vacant, comp, tabus, e[i], count)) {
            add_tabu(tabus, timer, e[i]);
            insert_edge(r, vacant, comp, e[i]);
            --count; // Decrease count of removed edges
        }
    }

    // It is possible that the current tabu restrictions cause a infeasible
    // solution (in this case, a disjoint graph). The following code allows
    // the insertion of edges which violate the tabus constraints in order 
    // to change the current solution into a feasible one.
    if (is_disjoint(comp, r->n)) {
        // Insert remaining "count" edges into graph
        for (i=0; (i<r->m && count>0); ++i) {
            // Insert edges ingnoring tabus (NULL parameter).
            if (viable_insertion(r, vacant, comp, NULL, e[i], count)) {
                add_tabu(tabus, timer, e[i]); 
                insert_edge(r, vacant, comp, e[i]);
                --count;
            }
        }
    }

    free(vacant);
    return;
}

/** viable_insertion
 *  Verifies if an edge can be inserted into the graph being currently worked
 *  on without generating an infeasible solution and without violating the 
 *  current tabu list. Note: if tabu list is NULL, the latter is ignored.
 *  Params:
 *  r      - Holds the current graph adjacency matrix and vertices degrees.
 *  vacant - Hash of total available connections per component (vacant[component_id] = space_available).
 *  comp   - Hash of vertices and theirs compoents (comp[vertex] = its_compoent_id).
 *  tabus  - Matrix marking edges which are currently considered tabus.
 *  e      - Edge which will be iserted int the current graph.
 *  count  - Amount of edges currently removed (can be used as component ID)
 *  Return: void.
 */
int viable_insertion(heu_graph* r, int* vacant, int* comp, char** tabus, edge_list e, int count) {
    int c1, c2, c3, c4, c5; // Conditions

    // checks if edge is NOT in the current solution
    c1 = !(r->mst[e.a][e.b] > -1);
    // Checks if degree cosntraint are obeyed
    c2 = (r->deg[e.a] > 0 && r->deg[e.b] > 0);
    // Checks if the edge connects different components
    c3 = (comp[e.a] != comp[e.b]);
    // If not last edge, must not constraint both components.
    // This prevents the insertion to saturate components and result in a disjoint graph.
    c4 = (count == 1 || vacant[comp[e.a]] > 1 || vacant[comp[e.b]] > 1);
    // Checks if it is not a re-insertion (if no tabu list, ignore this condition)
    if (tabus) c5 = !is_tabu(tabus, e);
    else c5 = 1;

    return (c1 && c2 && c3 && c4 && c5);
}

/** remove_edge
 *  Removes an edge from the graph being worked on. Uppon removal,
 *  also updates vertices degrees, components and components degrees.
 *  Params:
 *  r      - Holds the current graph adjacency matrix and vertices degrees.
 *  vacant - Hash of total available connections per component (vacant[component_id] = space_available).
 *  comp   - Hash of vertices and theirs compoents (comp[vertex] = its_compoent_id)
 *  e      - Edge which will be removed from the current graph.
 *  count  - Amount of edges currently removed (can be used as component ID)
 *  Return: void.
 */
void remove_edge(heu_graph *r, int vacant[], int comp[], edge_list e, int count) {
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
    new_id = count;

    // Split: mark new component with a new id
    tag_component(r->mst, r->n, start_v, comp, new_id);

    return;
}

/** insert_edge
 *  Adds an edge to the graph being worked on. Uppon insertion,
 *  also updates vertices degrees, components and components degrees.
 *  Params:
 *  r      - Holds the current graph adjacency matrix and vertices degrees.
 *  vacant - Hash of total available connections per component (vacant[component_id] = space_available).
 *  comp   - Hash of vertices and theirs compoents (comp[vertex] = its_compoent_id)
 *  e      - Edge which will be iserted int the current graph.
 *  Return: void.
 */
void insert_edge(heu_graph *r, int vacant[], int comp[], edge_list e) {
    int start_v, new_id, old_id;

    // Merge component higher ID to lowest ID
    new_id = min(comp[e.a], comp[e.b]);
    old_id = max(comp[e.a], comp[e.b]);

    // Get vertex from component that will be merged
    if (comp[e.b] < comp[e.a]) start_v = e.a;
    else start_v = e.b;

    // Merge: Change vertices from old_id component to new_id component.
    tag_component(r->mst, r->n, start_v, comp, new_id); 

    // Update degree constraint on vertices
    --r->deg[e.a]; 
    --r->deg[e.b];

    // Update vacant degrees per component
    --vacant[comp[e.a]]; 
    --vacant[comp[e.b]];

    // Update components vacants degrees
    vacant[new_id] += vacant[old_id];
    vacant[old_id] = 0;

    // Insert edge
    r->mst[e.a][e.b] = e.cost;
    r->mst[e.b][e.a] = e.cost;
    r->primal += e.cost;

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
    int start_time = tabu_time(e); 
    tabus[e.a][e.b] = 1;
    tabus[e.b][e.a] = 1;
    timer[e.a][e.b] = start_time;
    timer[e.b][e.a] = start_time;
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

/** tabu_time
 * Define how long an edge must wait to be removed.
 * Params:
 *  e   - Newly inserted edge.
 * Returns:
 *  int - Amount of time to remove edge from tabu list.
 */
int tabu_time(edge_list e) {
    int e_cost = e.cost+1;
    return (int)floor(max_cost/e_cost);
}