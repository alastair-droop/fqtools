#include "fqheader.h"

typedef struct {
    fqfile file;
    fqbuffer buffer;
} fqfilebuffer;

fqstatus fqfilebuffer_open(fqfilebuffer *fb, const char *filename, fqflag mode, fqflag format, size_t buffer_size);
void fqfilebuffer_close(fqfilebuffer *fb);

fqbytecount fqfilebuffer_read(fqfilebuffer *fb);
fqstatus fqfilebuffer_write(fqfilebuffer *fb);
