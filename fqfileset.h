#include "fqheader.h"

#define FQ_FILESET_UNPAIRED 0
#define FQ_FILESET_PAIRED 1

#define FQ_FILESET_PAIR_1 1
#define FQ_FILESET_PAIR_2 2


typedef struct {
    fqflag mode;
    fqflag paired;
    fqflag format;
    fqfilebuffer file_1;
    fqfilebuffer file_2;
} fqfileset;

fqstatus fqfileset_open(fqfileset *fs, const char *filename_1, const char *filename_2, fqflag mode, fqflag format, size_t buffer_size);
void fqfileset_close(fqfileset *fs);

fqstatus fqfileset_read(fqfileset *fs, fqbytecount *bytes_read_1, fqbytecount *bytes_read_2);
void fqfileset_write(fqfileset *fs);

fqbuffer* fqfileset_getBuffer(fqfileset *fs, char pair);

fqstatus fqfileset_appendchar(fqfileset *fs, char pair, char c);
fqstatus fqfileset_append(fqfileset *fs, char pair, char *data, fqbytecount size);
