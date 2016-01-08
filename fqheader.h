#ifndef fq_header_h
#define fq_header_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <zlib.h>

// Set the current version data:
#define PROG_NAME "fqtools"
#define FQTOOLS_VERSION "2.0 2016-01-07"

// Define the flag type:
typedef unsigned char fqflag;

// Set the function return types:
typedef unsigned char fqstatus;
#define FQ_STATUS_OK 0
#define FQ_STATUS_FAIL 1

// Define the sequence types:
#define SEQ_DNA (1 << 5)
#define SEQ_RNA (1 << 4)
#define SEQ_AMBIGUOUS (1 << 3)
#define SEQ_MASK (1 << 2)
#define SEQ_UPPERCASE (1 << 1)
#define SEQ_LOWERCASE (1 << 0)

// Define the FASTQ quality encodings:
#define FQ_QTYPE_UNKNOWN 0
#define FQ_QTYPE_SANGER 33
#define FQ_QTYPE_SOLEXA 59
#define FQ_QTYPE_ILLUMINA 64

// Define the file mode flag bits:
#define FQ_MODE_READ 0
#define FQ_MODE_WRITE 1

#define FQ_TYPE_PIPE 0
#define FQ_TYPE_FILE 1

#define FQ_FORMAT_UNKNOWN 0
#define FQ_FORMAT_FASTQ 1
#define FQ_FORMAT_FASTQ_GZ 2
#define FQ_FORMAT_BAM 3
#define FQ_FORMAT_FASTA 4
#define FQ_FORMAT_FASTA_GZ 5

// Define the file set types:
#define FQ_UNPAIRED 0
#define FQ_PAIRED 1

#define FQ_PAIR_1 0
#define FQ_PAIR_2 1

#define FQ_NONINTERLEAVED 0
#define FQ_INTERLEAVED 1

// Define a type for a number of bytes read:
typedef unsigned long int fqbytecount;

// Define a simple struct to hold the global options:
typedef struct {
    fqbytecount input_bufsize;
    fqbytecount output_bufsize;
    fqflag sequence_flags;
    fqflag quality;
    fqflag input_format;
    fqflag output_format;
    fqflag default_input_format;
    fqflag default_output_format;
    fqflag input_interleaving;
    fqflag output_interleaving;
    char output_fasta;
    char keep_headers;
    char output_filename_specified;
    char file_pair_replacement;
    char *file_output_stem;
} fqglobal;

// Define the subcommand processor functions:
fqstatus fqprocess_view(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_count(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_fasta(int argc, const char *argv[], fqglobal options);

#include "fqbuffer.h"
#include "fqfile.h"
#include "fqparser.h"
#include "fqfsin.h"
#include "fqfsout.h"
#include "fqfileprep.h"
#include "fqgenerics.h"
#include "fqhelp.h"

#endif
