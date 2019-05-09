Integer Programming Project
===========================

Integer Programming models and implementations for five problems. Each problem has two implementations of the same model.

The problems modeled are graph and task scheduling problems.
A brief description of each one is available at the beginning of the `.jl` files, along with input variable descriptions.

The models are explained in file `models-report.pdf` (in Portuguese).

Each problem has a folder, containing instance files, a Gnumeric sheet with the solution for a specific instance and a `.jl` file, containing the implementation in Julia.

When executing the Julia implementations, a report with stats is printed after the problem is solved (**in Portuguese**).
This report contains the solution status, node count, the best dual bound (D), the best primal bound (P), the optimality gap ((abs(D-P)/P)* 100) and total execution time.

Implementations
---------------
- [Julia](https://julialang.org/) programming language with [JuMP](https://github.com/JuliaOpt/JuMP.jl) and [Gurobi Optimizer](http://www.gurobi.com/).
- [Gnumeric](http://www.gnumeric.org/) sheets with [LPSolve](http://lpsolve.sourceforge.net/5.5/) solver.

Notable Files
-------------
- `Examples/`              - Examples (documented in Portuguese) of other problems with IP model implementations in Julia and Gnumeric sheets.
- `p2-instance-format.txt` - Instance format for each problem. 
- `large-tests.txt`        - Output for the largest instances of 3 of the problems for 6000 seconds of runtime (in Portuguese).
