#define FQ_STATUS_OK 0
#define FQ_STATUS_FAIL 1

typedef struct{
    char *data;
    size_t size;
    size_t offset;
} fqbuffer;

int fqbuffer_init(fqbuffer *b, size_t size);
void fqbuffer_free(fqbuffer *b);
