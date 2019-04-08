CFLAGS = -Wall -std=c99 -pedantic

.PHONY: bnb-fs clean debug

bnb-fs:
	gcc $(CFLAGS) bnb-fs.c nodes.c io.c heap.c -o bnb-fs

debug:
	gcc -g $(CFLAGS) bnb-fs.c nodes.c io.c heap.c -o bnb-fs

clean:
	rm -f bnb-fs test
