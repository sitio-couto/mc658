Metaheuristic and Lagrangian Heuristic for DCMSTP
=================================================

Heuristics for solving the Degree Constrained Minimum Spanning Tree Problem ([DCMSTP](https://en.wikipedia.org/wiki/Degree-constrained_spanning_tree)).
Complete detailed description and result analysis in file `report.pdf` (in Portuguese)

Compilation: `make` (needs [GCC](https://www.gnu.org/software/gcc/)).

Files
-----

- `dcmstp-solver.c` - Main and simple heuristic functions.
- `dcmstp-solver.h` - Header file: all the struct declarations, function signatures, libraries.
- `io.c`     		- Functions for reading standardized input, sending output.
- `lagrangian.c`    - Lagrangian Heuristic implementation (subgradient optimization and viabilization).
- `metaheuristic.c` - Metaheuristic implementation: tabu search with increasing parameters and multi-start experiment.
- `util.c`			- Auxiliary generic functions.
- `test.c`			- Auxiliary function for testing.

- `test_lagrange.sh`      - Shell script for running all instances with lagrangian heuristic.
- `test_meta.sh`	      - Shell script for running all instances with metaheuristic.

- `Instances/` - Standardized instances of DCMSTP. Complete graphs and vertex degree restrictions.
- `Results/`   - Standardized outputs for lagrangian heuristic and metaheuristic. Resulting tree, best dual (if possible), best primal and execution time.
