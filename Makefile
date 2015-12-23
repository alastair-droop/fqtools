CC=gcc
CFLAGS=-Wall

all: fqtools tests

fqtools:
	$(CC) $(CFLAGS) -o fqtools -lz fqbuffer.c fqfile.c main.c

tests:
	$(CC) $(CFLAGS) -o test-fqbuffer fqbuffer.c test-fqbuffer.c

clean:
	-rm fqtools
	-rm test-fqbuffer
