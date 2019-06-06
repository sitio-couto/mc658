Constraint Programming MiniZinc Model for SPLC
==============================================

[MiniZinc](https://www.minizinc.org/) is a constraint modeling language. 
Used for [SPLC](http://www.ic.unicamp.br/~cid/SPLC/SPLC.html): Scheduling Problem under Labour Constraints.

Complete detailed description, alternate models and result analysis in file `model_report.pdf` (in Portuguese) or within the main file (in English).

MiniZinc model examples can be found [here](https://www.minizinc.org/doc-2.2.1/en/downloads/).

Usage
-----
`minizinc -s -t <time-limit> pert.mzn <instance-file>`

Files
-----
- `Instances/`     - Instance files for SPLC.
- `script_gen.py`  - Script for generating shell execution script (modify parameters for different scripts).
- `search_type.py` - Script for creating test model with different search parameters.
- `pert.mzn`       - Main model.

References
----------
- [MiniZinc Handbook](https://www.minizinc.org/doc-2.2.3/en/index.html)
- [SPLC Benchmarks](http://www.ic.unicamp.br/~cid/SPLC/SPLC.html)
