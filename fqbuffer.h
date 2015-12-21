#include "fqheader.h"

typedef struct{
    char *data;
    size_t size;
    size_t offset;
} fqbuffer;

int fqbuffer_init(fqbuffer *b, size_t size);
void fqbuffer_free(fqbuffer *b);
size_t fqbuffer_remaining(fqbuffer *b);
