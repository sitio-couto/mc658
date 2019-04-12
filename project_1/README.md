Branch and Bound algorithm for FSP
==================================

Exact algorithm for solving the Flow Shop Scheduling problem (2 machines).
Complete detailed description and result analysis in file `grupo03-relatorio.pdf` (in Portuguese)
Compilation: `make` (needs [GCC](https://www.gnu.org/software/gcc/)).

Files:
`bnb-fs.c` - Main and most important functions.
`bnb-fs.h` - Header file: all the struct declarations, function signatures, libraries and global variables.
`io.c`     - Functions for reading standardized input, sending output and auxiliary functions.
`heap.c`   - Heap data structure implementation and manipulation.
`nodes.c`  - Auxiliary functions for dealing with nodes.

`gap.py`           - Script for output analysis (optimality gap calculations).
`change_format.py` - Script for changing schedule representation.

`script.sh`      - Shell script for running all instances.
`leak_script.sh` - Runs all instances through Valgrind.

`params.txt` - Time and amount of nodes explored limits.
`Instances/` - Standardized instances of FSP.
`Outputs/`   - Standardized outputs: for different strategies and limits.

