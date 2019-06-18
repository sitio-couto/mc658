CFLAGS = -Wall -pedantic -std=c99 -O3
FILES = dcmstp-solver.c io.c lagrangian.c metaheuristic.c util.c
TEST_FILES = test.c io.c lagrangian.c metaheuristic.c util.c

.PHONY: dcmstp-solver clean

dcmstp-solver:
	gcc $(CFLAGS) $(FILES) -o dcmstp-solver

debug:
	gcc -g $(CFLAGS) $(FILES) -o dcmstp-solver

tester:
	gcc -g $(CFLAGS) $(TEST_FILES) -o test

clean:
	rm -f dcmstp-solver test
