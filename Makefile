CC=gcc
CFLAGS=-Wall

SRC=.
BIN=.
HTSDIR=./htslib

TEST_SRC=./tests
TEST_BIN=./tests

SUBPROCESSES=view head count blockview fasta basetab qualtab lengthtab type validate find trim qualmap
SUBPROCESS_FILES=$(addsuffix .c, $(addprefix fqprocess_, $(SUBPROCESSES)))
MODULES=fqbuffer.c fqfile.c fqfsin.c fqfsout.c fqfileprep.c fqparser.c fqgenerics.c fqhelp.c
MODULE_LIST=$(addprefix $(SRC)/, $(MODULES))

EXEC=fqtools

.PHONY: all tests clean fqtools

fqtools:
	$(CC) $(CFLAGS) -I$(HTSDIR) -L$(HTSDIR) -o$(BIN)/$(EXEC) $(MODULE_LIST) $(SUBPROCESS_FILES) $(SRC)/fqtools.c -lz -lhts

tests:
	$(CC) $(CFLAGS) -I$(HTSDIR) -L$(HTSDIR) -o $(TEST_BIN)/test-fqbuffer -I$(SRC) -I$(TEST_SRC) $(MODULE_LIST) $(TEST_SRC)/test-fqbuffer.c -lz -lhts 

scratch:
	$(CC) $(CFLAGS) -I$(HTSDIR) -L$(HTSDIR) -o$(BIN)/fqtest $(MODULE_LIST) $(SRC)/scratch.c -lz -lhts 

all: fqtools tests

clean:
	-rm $(BIN)/fqtools
	-rm $(TEST_BIN)/test-fqbuffer
	-rm $(BIN)/fqtest
	-rm $(BIN)/out_*.fastq
