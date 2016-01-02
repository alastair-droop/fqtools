#include "fqheader.h"

typedef struct{
    char *data;
    fqbytecount size;
    fqbytecount offset;
} fqbuffer;

fqstatus fqbuffer_init(fqbuffer *b, fqbytecount size);
void fqbuffer_free(fqbuffer *b);

void fqbuffer_reset(fqbuffer *b);
fqbytecount fqbuffer_remaining(fqbuffer *b);
fqstatus fqbuffer_extend(fqbuffer *b, fqbytecount extra);

fqstatus fqbuffer_append(fqbuffer *b, char *data, fqbytecount size);
fqstatus fqbuffer_appendchar(fqbuffer *b, char c);

fqstatus fqbuffer_appendchar_noextend(fqbuffer *b, char c);
