#include "fqheader.h"

// Create the fqbuffer object:
int fqbuffer_init(fqbuffer *b, size_t size){
    b->size = 0;
    b->offset = 0;
    b->data = (char*)malloc((size + 1) * sizeof(char));
    if(b->data == NULL) return FQ_STATUS_FAIL;
    b->data[size] = '\0';
    b->size = size;
    return FQ_STATUS_OK;
}

// Free an fqbuffer object:
void fqbuffer_free(fqbuffer *b){
    free(b->data);
    b->size = 0;
    b->offset = 0;
}

// Reset an fqbuffer object by clearing its contents and
// setting the offset to the start:
void fqbuffer_reset(fqbuffer *b){
	memset(b->data, 0, b->size);
	b->offset = 0;
}

// Return the remaining space in the buffer as it is currently sized:
size_t fqbuffer_remaining(fqbuffer *b){
    return b->size - b->offset;
}

// Add extra space to the end of the buffer:
int fqbuffer_extend(fqbuffer *b, size_t extra){
	b->data = (char*)realloc(b->data, ((b->size + 1)* sizeof(char)) + (extra * sizeof(char)));
	if(b->data == NULL){
		fqbuffer_free(b);
		return FQ_STATUS_FAIL;
	}
	b->size += extra;
    b->data[b->size] = '\0';
	return FQ_STATUS_OK;
}

// Add string data to the buffer, extending if necessary:
int fqbuffer_append(fqbuffer *b, char *data, size_t size){
	if(size > fqbuffer_remaining(b)){
		if(fqbuffer_extend(b, size - fqbuffer_remaining(b)) != 0) return FQ_STATUS_FAIL;
	}
	memcpy(b->data + (b->offset * sizeof(char)), data, size);
	b->offset += size;
	return FQ_STATUS_OK;
}

// Add a single character to the buffer, extending if necessary:
int fqbuffer_appendchar(fqbuffer *b, char c){
	if(fqbuffer_remaining(b) == 0){
		if(fqbuffer_extend(b, 1) != 0) return FQ_STATUS_FAIL;
	}
	b->data[b->offset] = c;
	b->offset ++;
	return FQ_STATUS_OK;
}
