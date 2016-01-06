#ifndef fq_header_h
#define fq_header_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

// Define the flag type:
typedef unsigned char fqflag;

// Set the function return types:
typedef unsigned char fqstatus;
#define FQ_STATUS_OK 0
#define FQ_STATUS_FAIL 1

//Define the sequence types:
#define SEQ_DNA 64
#define SEQ_RNA 32
#define SEQ_AMBIGUOUS 16
#define SEQ_MASK 8
#define SEQ_LOWERCASE 4
#define SEQ_UPPERCASE 2


// Define a type for a number of bytes read:
typedef unsigned long int fqbytecount;

// #include "fqbuffer.h"
#include "fqfile.h"
// #include "fqfilebuffer.h"
// #include "fqfileset.h"
#include "fqparser.h"
#include "fqgenerics.h"

#endif
