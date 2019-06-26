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
    time_t start_time = clock();
    mat_graph *g;
    struct out *o;
    
    // Verifying arguments
    if (argc < 4){
        printf("Usage: dcmstp-solver <instance> <max_time> <method>\n");
        return 1;
    }
    
    g = read_input(argv[1]); 

    // Methods: 'l' = Lagrangian Relaxation. 'm' = Metaheuristic
    if (argv[3][0] == 'l'){
        o = lagrangian_heuristic(g, atoi(argv[2]), start_time);
        printf("%s,%.4lf,%.4lf\n", argv[1], o->dual, o->primal);
    }
    else if (argv[3][0] == 'm'){
        o = metaheuristic(g, atoi(argv[2]), start_time);
        printf("%s,%.4lf\n", argv[1], o->primal);
    }
    else{
        printf("Invalid method.\nMethods: 'l' = Lagrangian Relaxation. 'm' = Metaheuristic\n");
        return 1;
    }
    
    //generate_out_file(argv[1], o, g->n);
    free_graph(g);
    free(o->mst);
    free(o);
    
    return 0;
}

/** first_primal
 *  Creates a initial viable solution for the DCMSTP using greedy 
 *  heuristics (inserts edges by their wheight kruskals style).
 *  Params:
 *  g - Adjacency matrix representing input graph.
 *  Return:
 *  heu_graph - Struct holding usefull data from the generated solution.
 */
heu_graph* first_primal(mat_graph *g) {
    int c1, c2, c3;
    int i, count, *vacant;
    int comp[g->n]; // Indicates the component of the vertex
    heu_graph *r = heu_graph_alloc(g); // Alocate struct to keep results

    // Initialize sorted edges for greedy heuristic
    edge_list *e = edge_list_alloc(g->mat, g->n, g->m);
    // Initialize vertex component ID (every vertex start as a component)
    for(i=0; i < g->n; ++i) comp[i] = i; 
    // Initilize vacant vertices per component
    vacant = get_comp_gap(g->n, comp, g->deg, g->n);

    // Insert edges in ascending order avoiding cycles and respecting degrees
    count = 1;
    for (i=0; i < g->m; ++i) {
        // NO CYCLES => Vertices must be in different components.
        c1 = (comp[e[i].a] != comp[e[i].b]);
        // DEGREE CONSTRAINT => Degrees constraint must be respected
        c2 = (r->deg[e[i].a] > 0 && r->deg[e[i].b] > 0);
        // CONNECTED GRAPH => If not last edge, must not saturate both components.
        // This prevents the insertion to saturate components and result in a disjoint graph.
        c3 = (count == (g->n-1) || vacant[comp[e[i].a]] > 1 || vacant[comp[e[i].b]] > 1);

        // Merge and update compoenents by inserting new edge.
        if (c1 && c2 && c3) {
            insert_edge(r, vacant, comp, e[i]);
            ++count;
        }
    }

    free(vacant);
    free(e);
    return r;
}

/** random_primal
 *  Creates a initial viable solution for the DCMSTP using random 
 *  heuristics (randomly inserts edges until a DCMST is achieved).
 *  Params:
 *  g - Adjacency matrix representing input graph.
 *  Return:
 *  heu_graph - Struct holding usefull data from the generated solution.
 */
heu_graph* random_primal(mat_graph *g) {
    int c1, c2, c3;
    int i, count, *vacant;
    int comp[g->n];    // Indicates the component of the vertex
    heu_graph *r = heu_graph_alloc(g); // Alocate struct to keep results
    srand((unsigned)time(NULL)); // Seed random number generator

    // Initialize sorted edges for greedy heuristic
    edge_list *e = edge_list_alloc(g->mat, g->n, g->m);
    // Initialize vertex component ID (every vertex start as a component)
    for(i=0; i < g->n; ++i) comp[i] = i; 
    // Initilize vacant vertices per component
    vacant = get_comp_gap(g->n, comp, g->deg, g->n);

    
    // Insert edges randomly
    count = 1;
    while (count < g->n) {
        // Randomly select an edge
        i = rand()%g->m;
        
        // NO CYCLES => Vertices must be in different components.
        c1 = (comp[e[i].a] != comp[e[i].b]);
        // DEGREE CONSTRAINT => Degrees constraint must be respected
        c2 = (r->deg[e[i].a] > 0 && r->deg[e[i].b] > 0);
        // CONNECTED GRAPH => If not last edge, must not saturate both components.
        // This prevents the insertion to saturate components and result in a disjoint graph.
        c3 = (count == (g->n-1) || vacant[comp[e[i].a]] > 1 || vacant[comp[e[i].b]] > 1);

        // Merge and update compoenents by inserting new edge.
        if (c1 && c2 && c3) {
            insert_edge(r, vacant, comp, e[i]);
            ++count;
        }
    }

    free(vacant);
    free(e);
    return r;
}