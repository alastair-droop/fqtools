CC=gcc
CFLAGS=-Wall

SRC=.
BIN=.

TEST_SRC=./tests
TEST_BIN=./tests

MODULES=fqfile.c fqfileset.c fqparser.c fqgenerics.c 
MODULE_LIST=$(addprefix $(SRC)/, $(MODULES))

EXEC=fqtools

.PHONY: all tests clean fqtools

fqtools:
	$(CC) $(CFLAGS) -o$(BIN)/$(EXEC) -lz $(MODULE_LIST) $(SRC)/fqtools.c

tests:
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test-fqbuffer -I$(SRC) -I$(TEST_SRC) $(MODULE_LIST) -lz $(TEST_SRC)/test-fqbuffer.c

scratch:
	$(CC) $(CFLAGS) -o$(BIN)/fqtest -lz $(MODULE_LIST) $(SRC)/scratch.c

all: fqtools tests

clean:
	-rm $(BIN)/fqtools
	-rm $(TEST_BIN)/test-fqbuffer
	-rm $(BIN)/fqtest
	-rm $(BIN)/out_*.fastq
