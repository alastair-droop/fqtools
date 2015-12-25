#include "fqheader.h"

#define FQ_FILESET_UNPAIRED 0
#define FQ_FILESET_PAIRED 1

typedef struct {
    fqfile file;
    fqbuffer buffer;
} fqfilebuffer;

fqstatus fqfilebuffer_open(fqfilebuffer *fb, const char *filename, fqflag mode, fqflag format, size_t buffer_size);
void fqfilebuffer_close(fqfilebuffer *fb);

fqstatus fqfilebuffer_read(fqfilebuffer *fb);
fqstatus fqfilebuffer_write(fqfilebuffer *fb);


typedef struct {
    fqflag mode;
    fqflag paired;
    fqflag format;
    fqfilebuffer file_1;
    fqfilebuffer file_2;
} fqfileset;

fqstatus fqfileset_open_paired(fqfileset *fs, const char *filename_1, const char *filename_2, fqflag mode, fqflag format, size_t buffer_size);
void fqfileset_close(fqfileset *fs);
fqstatus fqfileset_read(fqfileset *fs);
