SRC=.
BIN=.
HTSDIR=./htslib

CFLAGS+=-O2 -g
CPPFLAGS+=-Wall -Wextra -Wno-unused-parameter -I$(HTSDIR)
LIBS=-lz -lhts -lm

TEST_SRC=./tests
TEST_BIN=./tests

SUBPROCESSES=view head count blockview fasta basetab qualtab lengthtab type validate find trim qualmap
SUBPROCESS_FILES=$(addsuffix .c, $(addprefix fqprocess_, $(SUBPROCESSES)))
SUBPROCESS_OBJECTS=$(addsuffix .o, $(addprefix fqprocess_, $(SUBPROCESSES)))

MODULES=fqbuffer fqfile fqfsin fqfsout fqfileprep fqparser fqgenerics fqhelp
MODULE_FILES=$(addsuffix .c, $(addprefix $(SRC)/, $(MODULES)))
MODULE_OBJECTS=$(addsuffix .o, $(addprefix $(SRC)/, $(MODULES)))

EXEC=fqtools

.PHONY: all tests clean

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

fqtools: $(SUBPROCESS_OBJECTS) $(MODULE_OBJECTS) $(SRC)/fqtools.o
	$(CC) $(CFLAGS) -L$(HTSDIR) -o$(BIN)/$(EXEC) $^ $(LIBS)

tests:
	$(CC) $(CFLAGS) -I$(HTSDIR) -L$(HTSDIR) -o $(TEST_BIN)/test-fqbuffer -I$(SRC) -I$(TEST_SRC) $(MODULE_LIST) $(TEST_SRC)/test-fqbuffer.c -lz -lhts 

scratch:
	$(CC) $(CFLAGS) -I$(HTSDIR) -L$(HTSDIR) -o$(BIN)/fqtest $(MODULE_LIST) $(SRC)/scratch.c -lz -lhts 

all: fqtools tests

clean:
	rm -f *.o
	rm -f $(BIN)/fqtools
	rm -f $(TEST_BIN)/test-fqbuffer
	rm -f $(BIN)/fqtest
	rm -f $(BIN)/out_*.fastq
