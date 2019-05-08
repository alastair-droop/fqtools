SRC=./src
BIN=./bin
HTSDIR=./htslib

CFLAGS+=-O2 -g
CPPFLAGS+=-Wall -Wextra -Wno-unused-parameter -I$(HTSDIR)
LIBS=-lz -lhts -lm

SUBPROCESSES=view head count blockview fasta basetab qualtab lengthtab type validate find trim qualmap tabseq
SUBPROCESS_FILES=$(addsuffix .c, $(addprefix $(SRC)/fqprocess_, $(SUBPROCESSES)))
SUBPROCESS_OBJECTS=$(addsuffix .o, $(addprefix $(SRC)/fqprocess_, $(SUBPROCESSES)))

MODULES=fqbuffer fqfile fqfsin fqfsout fqfileprep fqparser fqgenerics fqhelp
MODULE_FILES=$(addsuffix .c, $(addprefix $(SRC)/, $(MODULES)))
MODULE_OBJECTS=$(addsuffix .o, $(addprefix $(SRC)/, $(MODULES)))

EXEC=fqtools

.PHONY: all clean

$(SRC)/%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $(SRC)/$@ $^

fqtools: $(SUBPROCESS_OBJECTS) $(MODULE_OBJECTS) $(SRC)/fqtools.o
	mkdir -p bin
	$(CC) $(CFLAGS) -L$(HTSDIR) -o$(BIN)/$(EXEC) $^ $(LIBS)

all: fqtools

clean:
	rm -f $(SRC)/*.o
	rm -f $(BIN)/fqtools
	rm -f $(BIN)/fqtest
	rm -f $(BIN)/out_*.fastq
	rm -f ./tests/test-commands/temp/*
	rm -f ./tests/test-conversion/temp/*
