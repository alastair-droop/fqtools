#include "fqheader.h"

fqstatus fqfilebuffer_open(fqfilebuffer *fb, const char *filename, fqflag mode, fqflag format, fqbytecount buffer_size){
    fqstatus result;
    result = fqbuffer_init(&(fb->buffer), buffer_size);
    if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
    result = fqfile_open(&(fb->file), filename, mode, format);
    if(result != FQ_STATUS_OK){
        fqbuffer_free(&(fb->buffer));
        return FQ_STATUS_FAIL;
    }
    return FQ_STATUS_OK;
}

void fqfilebuffer_close(fqfilebuffer *fb){
    fqfile_close(&(fb->file));
    fqbuffer_free(&(fb->buffer));
}

fqbytecount fqfilebuffer_read(fqfilebuffer *fb){
    if(fb->file.mode != FQ_MODE_READ) return FQ_STATUS_FAIL;
    return fqfile_read(&(fb->file), &(fb->buffer));
}

fqstatus fqfilebuffer_write(fqfilebuffer *fb){
    if(fb->file.mode != FQ_MODE_WRITE) return FQ_STATUS_FAIL;
    fqfile_write(&(fb->file), &(fb->buffer));
    return FQ_STATUS_OK;
}
