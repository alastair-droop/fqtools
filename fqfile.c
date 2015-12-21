#include "fq-file.h"

void fqfile_readblock_fastq_uncompressed(char *buffer, size_t n){
    printf("Running fqfile_readblock_fastq_uncompressed\n");
}

int fq_file_init(fq_file *f, const char *filename, char type, char mode){    
    printf("Running fq_file_init\n");
    f->type = type;
    f->mode = mode;
    f->handle = fopen(filename, "r");
    return 0; 
}

void fq_file_close(fq_file *f){
    printf("Running fq_file_close\n");
    fclose(f->handle);
}
