#include <stdlib.h>
#include <stdio.h>

#include "fqbuffer.h"

int main(int argc, const char *argv[]){
    fqbuffer b;
    size_t buf_size;
    int res;
    
    buf_size = 100;
    res = fqbuffer_init(&b, buf_size);
    if(res != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: Failes to initialize buffer\n");
        return 1;
    }
    fqbuffer_free(&b);
    fprintf(stdout, "OK\n");
    return 0;
}