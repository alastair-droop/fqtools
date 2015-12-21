#include "fqheader.h"

int fqbuffer_init(fqbuffer *b, size_t size){
    printf("Running fqbuffer_init\n");
    b->size = 0;
    b->offset = 0;
    b->data = (char*)malloc(size * sizeof(char));
    if(b->data == NULL) return FQ_STATUS_FAIL;
    b->size = size;
    return FQ_STATUS_OK;
}

void fqbuffer_free(fqbuffer *b){
    printf("Running fqbuffer_free\n");
    free(b->data);
    b->size = 0;
    b->offset = 0;
}

size_t fqbuffer_remaining(fqbuffer *b){
    return b->size - b->offset;
}
