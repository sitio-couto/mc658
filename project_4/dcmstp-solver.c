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
    
    g = read_input_matrix(argv[1]);  

    // Methods: 'l' = Lagrangian Relaxation. 'm' = Metaheuristic
    if (argv[3][0] == 'l'){
        printf("Lagrangian\n");
        o = lagrangian_heuristic(g, atoi(argv[2]), start_time);
        printf("%s,%lf,%lf\n", argv[1], o->dual, o->primal);
    }
    else if (argv[3][0] == 'm'){
        printf("Metaheuristics\n");
        o = metaheuristic(g, atoi(argv[2]), start_time);
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
    int c1, c2, c3;
    int i, count, *vacant;
    int comp[g->n];    // Indicates the component of the vertex
    edge_list *e = edge_list_alloc(g->mat, g->n, g->m); // List of edges for greedy algrithims
    heu_graph *r = heu_graph_alloc(g); // Alocate struct to keep results

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

/// SIMPLIFIED CODE ///
// if (!is_disjoint(comp, g->n)) {
    //     for (i=0; i<g->n; ++i) r->deg[i] = r->deg[i];
    //     free(e);
    //     return r;
    // }

    // // MERGING GRAPH COMPONENTS //
    // /** In case the generated graph consists of multiple distinct
    //  *  connected components, the following code will merge such 
    //  *  components based on tree propositions:
    //  *  1 - The input graph is complete.
    //  *  2 - The disjoint components have no cycles.
    //  *  3 - Only one component has relaxed vertices.
    //  *  NOTE: the latter two are ensured by the greedy heuristic
    //  *        used to generate the current disjoint graph.
    // */

    // // Get the only relaxed component after greedy steps
    // master = -1;
    // for (i=0; i<g->n; ++i){
    //     if (r->deg[i]) {
    //         master = comp[i];
    //         relaxed[master] = 1;
    //         break;
    //     }
    // }

	// // Removes heaviest edge from constrained components
    // for (i=(g->m-1); i>=0; --i) {
    //     // Is the edge present in the current graph?.
    //     c1 = r->mst[e[i].a][e[i].b] > -1;
    //     // Is a constrained component?
    //     c2 = !relaxed[comp[e[i].a]];
    
    //     // Relax vertices and tag vertices/components
    //     if (c1 && c2) {
    //         // Remove edge from secondary component
    //         r->mst[e[i].a][e[i].b] = -1;
    //         r->mst[e[i].b][e[i].a] = -1;
    //         r->primal -= e[i].cost;

    //         // Relax degree constraint on vertices
    //         ++r->deg[e[i].a]; 
    //         ++r->deg[e[i].b];

    //         // Tag component as relaxed
    //         relaxed[comp[e[i].a]] = 1;
    //     }
    // }
    
    // // Connect relaxed vertices to master component by the lightest edge
    // for (i=0; i<g->m; ++i) {
    //     // One of the vertices MUST belong to the master component.
    //     c1 = (comp[e[i].a] == master || comp[e[i].b] == master);
    //     // One of the vertices must NOT belong to the master component.
    //     c2 = (comp[e[i].a] != master || comp[e[i].b] != master);
    //     // Both vertices must have enough room for new connections  
    //     c3 = (r->deg[e[i].a] > 0 && r->deg[e[i].b] > 0);
        
    //     if (c1 && c2 && c3) {
    //         // printf("Conne->(%d,%d)\n", e[i].a, e[i].b);
    //         // Insert edge from in solution
    //         r->mst[e[i].a][e[i].b] = e[i].cost;
    //         r->mst[e[i].b][e[i].a] = e[i].cost;
    //         r->primal += e[i].cost;

    //         // Constrain degree
    //         --r->deg[e[i].a]; 
    //         --r->deg[e[i].b];
    //     }
    // }