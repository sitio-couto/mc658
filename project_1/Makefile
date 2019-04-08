CFLAGS = -Wall -pedantic

.PHONY: bnb-fs clean debug

bnb-fs:
	g++ $(CFLAGS) bnb-fs.c nodes.c io.c -o bnb-fs

debug:
	g++ -g $(CFLAGS) bnb-fs.c nodes.c io.c -o bnb-fs

clean:
	rm -f bnb-fs test
