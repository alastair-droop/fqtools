#include "fqheader.h"

// int fqfile_readblock_fastq_uncompressed(void *f){
//     fqfile *f_handle = (fqfile *)f;
//     int bytes_to_read = fqbuffer_remaining(&(f_handle->buffer));
//     int bytes_read = (int)fread(f_handle->buffer.data + f_handle->buffer.offset, sizeof(char), bytes_to_read, f_handle->handle);
//     f_handle->buffer.offset += (size_t)bytes_read;
//     return bytes_read;
// }
//
int fqfile_init(fqfile f, const char *filename, char type, char mode, size_t buffer_size){
    // int res;
    // printf("Running fq_file_init\n");
    // f->type = type;
    // f->mode = mode;
    // f->handle = fopen(filename, "r");
    // res = fqbuffer_init(&(f->buffer), buffer_size);
    // if (res != FQ_STATUS_OK){
    //     fclose(f->handle);
    //     fqbuffer_free(&(f->buffer));
    //     return FQ_STATUS_FAIL;
    // }
    // f->read_buf = fqfile_readblock_fastq_uncompressed;
    return FQ_STATUS_OK;
}

void fqfile_close(fqfile f){
    printf("Running fq_file_close\n");
    // fclose(f->handle);
    // fqbuffer_free(&(f->buffer));
}
//
// int fqfile_readbuf(fqfile *f){
//     printf("Running fqfile_readbuf\n");
//     return f->read_buf(f);
// }