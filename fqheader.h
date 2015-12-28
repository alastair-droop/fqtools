#ifndef fq_header_h
#define fq_header_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

// Set the function return types:
typedef unsigned char fqstatus;
#define FQ_STATUS_OK 0
#define FQ_STATUS_FAIL 1

// Define a type for a number of bytes read:
typedef unsigned long int fqbytecount;

#include "fqbuffer.h"
#include "fqfile.h"
#include "fqfileset.h"
#include "fqparser.h"

#endif
