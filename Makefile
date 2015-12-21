CC=gcc
CFLAGS=-Wall

all:
	$(CC) $(CFLAGS) -o fqtools -lz *.c

clean:
	-rm fqtools
