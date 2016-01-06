#include "fqheader.h"

#define FQ_FILESET_UNPAIRED 0
#define FQ_FILESET_PAIRED 1

#define FQ_FILESET_PAIR_1 1
#define FQ_FILESET_PAIR_2 2

#define FQ_FILESET_NONINTERLEAVED 0
#define FQ_FILESET_INTERLEAVED 1

typedef struct {
    fqflag mode;
    fqflag paired;
    fqflag format;
    fqflag interleaved;
    fqfile file_1;
    fqfile file_2;
} fqfileset;

fqstatus fqfileset_open_single(fqfileset *fs, const char *filename, fqflag mode, fqflag format, fqflag interleaved);
fqstatus fqfileset_open_pair(fqfileset *fs, const char *filename_1, const char *filename_2, fqflag mode, fqflag format);

fqbytecount fqfileset_read(fqfileset *fs, char file, char *buffer, fqbytecount buffer_n);
fqbytecount fqfileset_write(fqfileset *fs, char file, char *buffer, fqbytecount buffer_n);
fqbytecount fqfileset_writechar(fqfileset *fs, char file, char c);

void fqfileset_close(fqfileset *fs);
//
// fqstatus fqfileset_read(fqfileset *fs, fqbytecount *bytes_read_1, fqbytecount *bytes_read_2);
// void fqfileset_write(fqfileset *fs);
//
// fqbuffer* fqfileset_getBuffer(fqfileset *fs, char pair);
//
// fqstatus fqfileset_appendchar(fqfileset *fs, char pair, char c);
// fqstatus fqfileset_append(fqfileset *fs, char pair, char *data, fqbytecount size);
