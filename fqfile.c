#include "fqheader.h"

int fqfile_open(fqfile *f, const char *filename, char type, char mode){
    if(type == FQFILE_TYPE_FASTQ_UNCOMPRESSED) return fqfile_open_file(f, filename, mode);
    if(type == FQFILE_TYPE_FASTQ_COMPRESSED) return fqfile_open_gzfile(f, filename, mode);
    return FQ_STATUS_FAIL;
}

void fqfile_close(fqfile *f){
    f->close_file(f);
}

int fqfile_readbuf(fqfile *f, fqbuffer *b){
    return f->read_buf(f, b);
}

int fqfile_open_file(fqfile *f, const char *filename, char mode){
    f->handle = (void*)fopen(filename, "r");
    f->close_file = fqfile_close_file;
    f->read_buf = fqfile_readbuf_file;
    return FQ_STATUS_OK;
}

int fqfile_open_gzfile(fqfile *f, const char *filename, char mode){
    f->handle = (void*)gzopen(filename, "r");
    f->close_file = fqfile_close_gzfile;
    f->read_buf = fqfile_readbuf_gzfile;
    return FQ_STATUS_OK;
}

void fqfile_close_file(void *f){
    fclose((FILE*)(((fqfile*)f)->handle));
}

void fqfile_close_gzfile(void *f){
    gzclose((gzFile*)(((fqfile*)f)->handle));
}

int fqfile_readbuf_file(void *f, fqbuffer *b){
    int bytes_read = (int)fread(b->data + b->offset, sizeof(char), fqbuffer_remaining(b), (FILE*)(((fqfile*)f)->handle));
    return bytes_read;
}

int fqfile_readbuf_gzfile(void *f, fqbuffer *b){
    int bytes_read = gzread((gzFile*)(((fqfile*)f)->handle), b->data + b->offset, (int)fqbuffer_remaining(b));
    return bytes_read;
}
