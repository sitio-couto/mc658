CFLAGS = -Wall -pedantic -std=c99 -O3
FILES = dcmstp-solver.c io.c lagrangian.c metaheuristic.c util.c -lm
TEST = test.c

.PHONY: dcmstp-solver clean

dcmstp-solver:
	gcc $(CFLAGS) $(FILES) $(TEST) -o dcmstp-solver

debug:
	gcc -g $(CFLAGS) $(FILES) -o dcmstp-solver

clean:
	rm -f dcmstp-solver
