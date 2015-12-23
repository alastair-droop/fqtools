#include "fqheader.h"

size_t random_size(size_t min, size_t max){
    double x = (double)rand() / (double)RAND_MAX;
    return x * (max - min + 1) + min;
}

int main(int argc, const char *argv[]){
    int status;
    fqbuffer buffer;
    unsigned int repeats = 10000000;
    unsigned int i;
    size_t size;
    
    // Hammer the creation and freeing of the fqbuffer object:
    for(i = 0; i < repeats; i++){
        size = random_size(1, 1048576);
        status = fqbuffer_init(&buffer, size);
        printf("[%8d/%8d] (%7zd): %d\n", i + 1, repeats, size, status);
        if(status != FQ_STATUS_OK) return 1;
        fqbuffer_free(&buffer);
    }
    
    // Hammer writing to a single buffer, and therefore extending it if necessary:
    status = fqbuffer_init(&buffer, 10);
    if(status != FQ_STATUS_OK) return 1;
    for(i = 0; i < repeats; i++){
        status = fqbuffer_appendchar(&buffer, 'A');
        printf("[%8d/%8d] (%7zd): %d\n", i + 1, repeats, buffer.size, status);
        if(status != FQ_STATUS_OK){
            fqbuffer_free(&buffer);
            return 1;
        }
    }
    fqbuffer_free(&buffer);
    return 0;        
}