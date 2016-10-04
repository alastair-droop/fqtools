// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef fq_header_h
#define fq_header_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <zlib.h>
#include <htslib/sam.h>

// Set the current version data:
#define PROG_NAME "fqtools"
#define FQTOOLS_VERSION "2.1 2016-10-04"

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
    char single_input;
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
fqstatus fqprocess_head(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_count(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_blockview(int argc, const char *argv[], fqglobal options, char blockview);
fqstatus fqprocess_fasta(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_basetab(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_qualtab(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_lengthtab(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_type(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_validate(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_find(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_trim(int argc, const char *argv[], fqglobal options);
fqstatus fqprocess_qualmap(int argc, const char *argv[], fqglobal options);

#include "fqbuffer.h"
#include "fqfile.h"
#include "fqparser.h"
#include "fqfsin.h"
#include "fqfsout.h"
#include "fqfileprep.h"
#include "fqgenerics.h"
#include "fqhelp.h"

#endif
