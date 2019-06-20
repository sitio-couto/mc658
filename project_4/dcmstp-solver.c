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

    // Methods: 'l' = Lagrangian Relaxation. 'm' = Metaheuristic
    if (argv[3][0] == 'l'){
        printf("Lagrangian\n");
        o = lagrangian_heuristic(g, atoi(argv[2]));
        printf("%s,%lf,%lf\n", argv[1], o->dual, o->primal);
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
    int i, j, k, master, merge;
    int c1, c2, c3;
    int relaxed[g->n]; // Indicates which components where relaxed
    int degGap[g->n];  // Indicates the remaining edges alowed per vertex
    int comp[g->n];    // Indicates the component of the vertex
    edge2vert *e = malloc(g->m*sizeof(edge2vert)); // List of edges for greedy algrithims

    // Alocate resulting struct to keep results
    heu_graph *result = malloc(sizeof(heu_graph));
    result->primal = 0;
    result->n = g->n;
    result->deg = malloc(g->n*sizeof(int));
    result->mst = malloc(g->n*sizeof(int*));
    for (i=0; i<g->n; ++i) {
        result->deg[i] = g->deg[i];
        result->mst[i] = malloc(g->n*sizeof(int));
        for (j=0; j<g->n; ++j) result->mst[i][j] = -1;
    }

    // Initilize lists for greedy heuristic
    k = 0;
    for(i=0; i < g->n; ++i) {
        degGap[i] = g->deg[i]; // Max vertices degrees
        comp[i] = i;        // Vertex component ID
        relaxed[i] = 0;     // Relaxation hash table
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
        c2 = (degGap[e[i].a] > 0 && degGap[e[i].b] > 0); // Degrees constraint must be respected

        if (c1 && c2) {
            master  = min(comp[e[i].a],comp[e[i].b]);
            merge = max(comp[e[i].a],comp[e[i].b]);
            
            // Merge components
            for (j=0; j<g->n; ++j) {
                if (comp[j] == merge) comp[j] = master;
            }

            // Decrement vertices avalable connections
            --degGap[e[i].a]; 
            --degGap[e[i].b];

            // Add edge to result struct
            result->mst[e[i].a][e[i].b] = e[i].cost;
            result->mst[e[i].b][e[i].a] = e[i].cost;
            result->primal += e[i].cost;
        }
    }

    if (!is_disjoint(comp, g->n)) {
        test_mst(result->mst, result->deg, result->n, comp);
        free(e);
        return result;
    }

    // MERGING GRAPH COMPONENTS //
    /** In case the generated graph consists of multiple distinct
     *  connected components, the following code will merge such 
     *  components based on tree propositions:
     *  1 - The input graph is complete.
     *  2 - The disjoint components have no cycles.
     *  3 - Only one component has relaxed vertices.
     *  NOTE: the latter two are ensured by the greedy heuristic
     *        used to generate the current disjoint graph.
    */

    // Get the only relaxed component after greedy steps
    master = -1;
    for (i=0; i<g->n; ++i){
        if (degGap[i]) {
            master = comp[i];
            relaxed[master] = 1;
            break;
        }
    }

	// Removes heaviest edge from constrained components
    for (i=(g->m-1); i>=0; --i) {
        // Is the edge present in the current graph?.
        c1 = result->mst[e[i].a][e[i].b] > -1;
        // Is a constrained component?
        c2 = !relaxed[comp[e[i].a]];
    
        // Relax vertices and tag vertices/components
        if (c1 && c2) {
            // Remove edge from secondary component
            result->mst[e[i].a][e[i].b] = -1;
            result->mst[e[i].b][e[i].a] = -1;
            result->primal -= e[i].cost;

            // Relax degree constraint on vertices
            ++degGap[e[i].a]; 
            ++degGap[e[i].b];

            // Tag component as relaxed
            relaxed[comp[e[i].a]] = 1;
        }
    }
    
    // Connect relaxed vertices to master component by the lightest edge
    for (i=0; i<g->m; ++i) {
        // One of the vertices MUST belong to the master component.
        c1 = (comp[e[i].a] == master || comp[e[i].b] == master);
        // One of the vertices must NOT belong to the master component.
        c2 = (comp[e[i].a] != master || comp[e[i].b] != master);
        // Both vertices must have enough room for new connections  
        c3 = (degGap[e[i].a] > 0 && degGap[e[i].b] > 0);
        
        if (c1 && c2 && c3) {
            // printf("Conne->(%d,%d)\n", e[i].a, e[i].b);
            // Insert edge from in solution
            result->mst[e[i].a][e[i].b] = e[i].cost;
            result->mst[e[i].b][e[i].a] = e[i].cost;
            result->primal += e[i].cost;

            // Constrain degree
            --degGap[e[i].a]; 
            --degGap[e[i].b];
        }
    }

    test_mst(result->mst, result->deg, result->n, comp);

    free(e);
    return result;
}

void fix_disjuction(heu_graph *result, int degGap[]) {

}

// void local_search() {
    
// }
