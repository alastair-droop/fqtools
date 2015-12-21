#include "fqheader.h"

// The general file manipulations. These delegate to specific callbacks:
int fqfile_open(fqfile *f, const char *filename, char format, char type, char mode){
    if(format == FQFILE_FORMAT_FASTQ_UNCOMPRESSED) return fqfile_open_file(f, filename, type, mode);
    if(format == FQFILE_FORMAT_FASTQ_COMPRESSED) return fqfile_open_gzfile(f, filename, type, mode);
    return FQ_STATUS_FAIL;
}

void fqfile_close(fqfile *f){
    f->close_file(f);
}

int fqfile_readbuf(fqfile *f, fqbuffer *b){
    return f->read_buf(f, b);
}

char fqfile_eof(fqfile *f){
    return f->eof(f);
}

// Callbacks for opening files:

int fqfile_open_file(fqfile *f, const char *filename, char type, char mode){
    f->handle = (void*)fopen(filename, "r");
    f->close_file = fqfile_close_file;
    f->read_buf = fqfile_readbuf_file;
    f->eof = fqfile_eof_file;
    return FQ_STATUS_OK;
}

int fqfile_open_gzfile(fqfile *f, const char *filename, char type, char mode){
    f->handle = (void*)gzopen(filename, "r");
    f->close_file = fqfile_close_gzfile;
    f->read_buf = fqfile_readbuf_gzfile;
    f->eof = fqfile_eof_gzfile;
    return FQ_STATUS_OK;
}

// Callbacks for closing files:
void fqfile_close_file(void *f){
    fclose((FILE*)(((fqfile*)f)->handle));
}

void fqfile_close_gzfile(void *f){
    gzclose((gzFile*)(((fqfile*)f)->handle));
}

// Callbacks for reading file data into a buffer:
int fqfile_readbuf_file(void *f, fqbuffer *b){
    int bytes_read = (int)fread(b->data + b->offset, sizeof(char), fqbuffer_remaining(b), (FILE*)(((fqfile*)f)->handle));
    return bytes_read;
}

int fqfile_readbuf_gzfile(void *f, fqbuffer *b){
    int bytes_read = gzread((gzFile*)(((fqfile*)f)->handle), b->data + b->offset, (int)fqbuffer_remaining(b));
    return bytes_read;
}

// Callbacks to determine end of file:
char fqfile_eof_file(void *f){
    return feof((FILE*)(((fqfile*)f)->handle));
}

char fqfile_eof_gzfile(void *f){
    return gzeof((gzFile*)(((fqfile*)f)->handle));
}
