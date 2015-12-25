#include "fqheader.h"

fqstatus fqfilebuffer_open(fqfilebuffer *fb, const char *filename, fqflag mode, fqflag format, size_t buffer_size){
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

fqstatus fqfilebuffer_read(fqfilebuffer *fb){
    if(fb->file.mode != FQ_MODE_READ) return FQ_STATUS_FAIL;
    fqfile_read(&(fb->file), &(fb->buffer));
    return FQ_STATUS_OK;
}

fqstatus fqfilebuffer_write(fqfilebuffer *fb){
    if(fb->file.mode != FQ_MODE_WRITE) return FQ_STATUS_FAIL;
    fqfile_write(&(fb->file), &(fb->buffer));
    return FQ_STATUS_OK;
}


fqstatus fqfileset_open_paired(fqfileset *fs, const char *filename_1, const char *filename_2, fqflag mode, fqflag format, size_t buffer_size){
    fqstatus result;
    result = fqfilebuffer_open(&(fs->file_1), filename_1, mode, format, buffer_size);
    if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
    result = fqfilebuffer_open(&(fs->file_2), filename_2, mode, format, buffer_size);
    if(result != FQ_STATUS_OK){
        fqfilebuffer_close(&(fs->file_1));
        return FQ_STATUS_FAIL;
    }
    fs->paired = FQ_FILESET_PAIRED;
    fs->mode = mode;
    fs->format = format;
    return FQ_STATUS_OK;
}

void fqfileset_close(fqfileset *fs){
    fqfilebuffer_close(&(fs->file_1));
    fqfilebuffer_close(&(fs->file_2));
}

fqstatus fqfileset_read(fqfileset *fs){
    fqfilebuffer_read(&(fs->file_1));
    if(fs->paired == FQ_FILESET_PAIRED) fqfilebuffer_read(&(fs->file_2));
    return FQ_STATUS_OK;
}


// fqstatus fqfileset_open_paired(fqfileset *fs, const char *filename_1, const char *filename_2, fqflag mode, fqflag format){
//     fqstatus result;
//     result = fqfile_open(&(fs->pair_1), filename_1, mode, format);
//     if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
//     result = fqfile_open(&(fs->pair_2), filename_2, mode, format);
//     if(result != FQ_STATUS_OK){
//         fqfile_close(&(fs->pair_1));
//         return FQ_STATUS_FAIL;
//     }
//     fs->paired = FQ_FILESET_PAIRED;
//     fs->mode = mode;
//     fs->format = format;
//     return FQ_STATUS_OK;
// }
//
// void fqfileset_close(fqfileset *fs){
//     fqfile_close(&(fs->pair_1));
//     if (fs->paired == FQ_FILESET_PAIRED) fqfile_close(&(fs->pair_2));
// }
//
