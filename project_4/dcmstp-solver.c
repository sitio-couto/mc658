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
