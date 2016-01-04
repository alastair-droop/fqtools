#include "fqheader.h"

#define FQ_FILESET_UNPAIRED 0
#define FQ_FILESET_PAIRED 1

typedef struct {
    fqflag mode;
    fqflag paired;
    fqflag format;
    fqfilebuffer file_1;
    fqfilebuffer file_2;
} fqfileset;

fqstatus fqfileset_open_paired(fqfileset *fs, const char *filename_1, const char *filename_2, fqflag mode, fqflag format, size_t buffer_size);
void fqfileset_close(fqfileset *fs);
fqstatus fqfileset_read(fqfileset *fs, fqbytecount *bytes_read_1, fqbytecount *bytes_read_2);
fqstatus fqfileset_write(fqfileset *fs, fqbytecount *bytes_written_1, fqbytecount *bytes_written_2);
