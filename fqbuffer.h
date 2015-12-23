#include "fqheader.h"

typedef struct{
    char *data;
    size_t size;
    size_t offset;
} fqbuffer;

int fqbuffer_init(fqbuffer *b, size_t size);
void fqbuffer_free(fqbuffer *b);

void fqbuffer_reset(fqbuffer *b);
size_t fqbuffer_remaining(fqbuffer *b);
int fqbuffer_extend(fqbuffer *b, size_t extra);

int fqbuffer_append(fqbuffer *b, char *data, size_t size);
int fqbuffer_appendchar(fqbuffer *b, char c);