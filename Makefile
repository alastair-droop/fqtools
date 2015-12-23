CC=gcc
CFLAGS=-Wall

.PHONY: all tests clean fqtools

fqtools:
	$(CC) $(CFLAGS) -o fqtools -lz fqbuffer.c fqfile.c main.c

tests:
	$(CC) $(CFLAGS) -o test-fqbuffer fqbuffer.c test-fqbuffer.c

all: fqtools tests

clean:
	-rm fqtools
	-rm test-fqbuffer
