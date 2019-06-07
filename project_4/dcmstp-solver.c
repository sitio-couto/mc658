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

#include "dcmstp-solver.h";

int main(int argc, char *argv[]){
    
    // Verifying arguments
    if (argc < 4){
        printf("Usage: dcmstp-solver <instance> <time> <method>\n");
        return 1;
    }
    
    // Methods: 'l' = Lagrangian Relaxation. 'm' = Metaheuristic
    if (argv[3][0] == 'l')
        printf("Lagrangian\n");
    else if (argv[3][0] == 'm')
        printf("Metaheuristics\n");
    else
        printf("Invalid method.\nMethods: 'l' = Lagrangian Relaxation. 'm' = Metaheuristic\n");
        
    return 0;
}
