#ifndef fq_header_h
#define fq_header_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// Define a type for a number of bytes read:
typedef unsigned long int fqbytecount;

#include "fqbuffer.h"
#include "fqfile.h"
#include "fqparser.h"
#include "fqfileset.h"
#include "fqgenerics.h"
#include "fqhelp.h"

#endif
