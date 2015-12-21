CC=gcc
CFLAGS=-Wall

all:
	$(CC) $(CFLAGS) -o fqtools *.c

clean:
	-rm fqtools
