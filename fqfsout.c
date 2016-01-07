#include "fqheader.h"

fqstatus fqbfile_open(fqbfile *f, const char *filename, fqflag format, fqbytecount b_size){
    fqstatus result;
    result = fqfile_open(&(f->file), (char*)filename, FQ_MODE_WRITE, format);
    if(result != FQ_STATUS_OK) return result;
    result = fqbuffer_init(&(f->buffer), b_size);
    if(result != FQ_STATUS_OK){
        fqfile_close(&(f->file));
        return result;
    }
    f->trigger = b_size;
    return FQ_STATUS_OK;
}

fqstatus fqbfile_writechar(fqbfile *f, char c){
    fqstatus result;
    result = fqbuffer_appendchar(&(f->buffer), c);
    if(result != FQ_STATUS_OK) return result;
    if(f->buffer.offset >= f->trigger) fqbfile_flush(f);
    return FQ_STATUS_OK;
}

fqstatus fqbfile_write(fqbfile *f, char *buffer, fqbytecount buffer_n){
    fqstatus result;
    result = fqbuffer_append(&(f->buffer), buffer, buffer_n);
    if(result != FQ_STATUS_OK) return result;
    if(f->buffer.offset >= f->trigger) fqbfile_flush(f);
    return FQ_STATUS_OK;
}

void fqbfile_flush(fqbfile *f){
    fqfile_write(&(f->file), f->buffer.data, f->buffer.offset);
    fqbuffer_reset(&(f->buffer));
}

void fqbfile_close(fqbfile *f){
    fqbfile_flush(f);
    fqfile_close(&(f->file));
    fqbuffer_free(&(f->buffer));
}

fqstatus fqfsout_prepare(fqfsout *f, char input_files, char specify_filename, fqglobal opt, fqflag default_format_1, fqflag default_format_2){
    fqstatus result;
    char *filename_1 = NULL;
    char *filename_2 = NULL;
    fqflag format_1 = opt.output_format;
    fqflag format_2 = opt.output_format;
    char n_filenames = 2;
    if((input_files == 1) || (opt.output_interleaving == FQ_INTERLEAVED)) n_filenames = 1;
    if(format_1 == FQ_FORMAT_UNKNOWN) format_1 = default_format_1;
    if(format_2 == FQ_FORMAT_UNKNOWN) format_2 = default_format_2;    
    if((n_filenames == 1) && (specify_filename == 0)) return fqfsout_open_single(f, NULL, format_1, opt.output_bufsize);
    if((n_filenames == 1) && (specify_filename == 1)){
        filename_1 = generate_filename(opt.file_output_stem, opt.file_pair_replacement, 0, format_1);
        if(filename_1 == NULL) return FQ_STATUS_FAIL;
        result = fqfsout_open_single(f, filename_1, format_1, opt.output_bufsize);
        free(filename_1);
        return result;
    }
    filename_1 = generate_filename(opt.file_output_stem, opt.file_pair_replacement, 1, format_1);
    if(filename_1 == NULL) return FQ_STATUS_FAIL;
    filename_2 = generate_filename(opt.file_output_stem, opt.file_pair_replacement, 2, format_2);
    if(filename_2 == NULL){
        free(filename_1);
        return FQ_STATUS_FAIL;  
    }
    result = fqfsout_open_paired(f, filename_1, filename_2, format_1, format_2, opt.output_bufsize);
    free(filename_1);
    free(filename_2);
    return result;
}

fqstatus fqfsout_open_single(fqfsout *f, const char *filename, fqflag format, fqbytecount b_size){
    fqstatus result;
    f->files[0] = malloc(sizeof(fqbfile));
    if(f->files[0] == NULL) return FQ_STATUS_FAIL;
    f->files[1] = malloc(sizeof(fqbfile));
    if(f->files[1] == NULL){
        free(f->files[0]);
        return FQ_STATUS_FAIL;  
    } 
    result = fqbfile_open(f->files[0], filename, format, b_size);
    if(result != FQ_STATUS_OK){
        free(f->files[0]);
        free(f->files[1]);
        return result;
    }
    result = fqbfile_open(f->files[1], filename, format, b_size);
    if(result != FQ_STATUS_OK){
        fqbfile_close(f->files[0]);
        free(f->files[0]);
        free(f->files[1]);
        return result;
    }
    f->n_files = 2;
    return FQ_STATUS_OK;
}

// fqstatus fqfsout_open_single(fqfsout *f, const char *filename, fqflag format, fqbytecount b_size){
//     fqstatus result;
//     f->files[0] = malloc(sizeof(fqbfile));
//     if(f->files[0] == NULL) return FQ_STATUS_FAIL;
//     result = fqbfile_open(f->files[0], filename, format, b_size);
//     if(result != FQ_STATUS_OK){
//         free(f->files[0]);
//         return result;
//     }
//     f->files[1] = f->files[0];
//     f->n_files = 1;
//     return FQ_STATUS_OK;
// }

fqstatus fqfsout_open_paired(fqfsout *f, const char *filename_1, const char *filename_2, fqflag format_1, fqflag format_2, fqbytecount b_size){
    fqstatus result;
    f->files[0] = malloc(sizeof(fqbfile));
    if(f->files[0] == NULL) return FQ_STATUS_FAIL;
    f->files[1] = malloc(sizeof(fqbfile));
    if(f->files[1] == NULL){
        free(f->files[0]);
        return FQ_STATUS_FAIL;  
    } 
    result = fqbfile_open(f->files[0], filename_1, format_1, b_size);
    if(result != FQ_STATUS_OK){
        free(f->files[0]);
        free(f->files[1]);
        return result;
    }
    result = fqbfile_open(f->files[1], filename_2, format_2, b_size);
    if(result != FQ_STATUS_OK){
        fqbfile_close(f->files[0]);
        free(f->files[0]);
        free(f->files[1]);
        return result;
    }
    f->n_files = 2;
    return FQ_STATUS_OK;
}

fqstatus fqfsout_writechar(fqfsout *f, int file, char c){
    return fqbfile_writechar(f->files[file], c);
}

fqstatus fqfsout_write(fqfsout *f, int file, char *buffer, fqbytecount buffer_n){
    return fqbfile_write(f->files[file], buffer, buffer_n);
}

void fqfsout_flush(fqfsout *f, int file){
    fqbfile_flush(f->files[file]);
}

void fqfsout_close(fqfsout *f){
    fqbfile_close(f->files[0]);
    free(f->files[0]);
    if(f->n_files == 2){
        fqbfile_close(f->files[1]);
        free(f->files[1]);
    }
}
