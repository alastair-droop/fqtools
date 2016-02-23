// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "fqheader.h"

// fqstatus fqbfile_open(fqbfile *f, const char *filename, fqflag format, fqbytecount b_size){
//     fqstatus result;
//     result = fqfile_open(&(f->file), (char*)filename, FQ_MODE_WRITE, format);
//     if(result != FQ_STATUS_OK) return result;
//     result = fqbuffer_init(&(f->buffer), b_size);
//     if(result != FQ_STATUS_OK){
//         fqfile_close(&(f->file));
//         return result;
//     }
//     f->trigger = b_size;
//     return FQ_STATUS_OK;
// }
//
// fqstatus fqbfile_writechar(fqbfile *f, char c){
//     fqstatus result;
//     result = fqbuffer_appendchar(&(f->buffer), c);
//     if(result != FQ_STATUS_OK) return result;
//     if(f->buffer.offset >= f->trigger) fqbfile_flush(f);
//     return FQ_STATUS_OK;
// }
//
// fqstatus fqbfile_write(fqbfile *f, char *buffer, fqbytecount buffer_n){
//     fqstatus result;
//     result = fqbuffer_append(&(f->buffer), buffer, buffer_n);
//     if(result != FQ_STATUS_OK) return result;
//     if(f->buffer.offset >= f->trigger) fqbfile_flush(f);
//     return FQ_STATUS_OK;
// }
//
// void fqbfile_flush(fqbfile *f){
//     fqfile_write(&(f->file), f->buffer.data, f->buffer.offset);
//     fqbuffer_reset(&(f->buffer));
// }
//
// void fqbfile_close(fqbfile *f){
//     fqbfile_flush(f);
//     fqfile_close(&(f->file));
//     fqbuffer_free(&(f->buffer));
// }

// typedef struct{
//     char n_files;
//     fqflag interleaved;
//     fqfile *files[2];
//     fqbuffer *buffers[2];
// } fqfsout;

fqstatus fqfsout_open_single(fqfsout *f, const char *filename, fqflag interleaved, fqflag format, fqbytecount b_size){
    fqstatus result;
    result = fqfile_open(&(f->file[FQ_PAIR_1]), (char*)filename, FQ_MODE_WRITE, format);
    if(result != FQ_STATUS_OK) return result;
    result = fqbuffer_init(&(f->buffer[FQ_PAIR_1]), b_size);
    if(result != FQ_STATUS_OK){
        fqfile_close(&(f->file[FQ_PAIR_1]));
        return result;
    }
    f->trigger[FQ_PAIR_1] = b_size;
    if(interleaved == FQ_INTERLEAVED){
        result = fqbuffer_init(&(f->buffer[FQ_PAIR_2]), b_size);
        if(result != FQ_STATUS_OK){
            fqfile_close(&(f->file[FQ_PAIR_1]));
            fqbuffer_free(&(f->buffer[FQ_PAIR_1]));
            return result;
        }
        f->trigger[FQ_PAIR_2] = b_size;
    }
    f->n_files = 1;
    f->interleaved = interleaved;
    return FQ_STATUS_OK;
}

fqstatus fqfsout_open_paired(fqfsout *f, const char *filename_1, const char *filename_2, fqflag format_1, fqflag format_2, fqbytecount b_size){
    fqstatus result;
    result = fqfile_open(&(f->file[FQ_PAIR_1]), (char*)filename_1, FQ_MODE_WRITE, format_1);
    if(result != FQ_STATUS_OK) return result;
    result = fqfile_open(&(f->file[FQ_PAIR_2]), (char*)filename_2, FQ_MODE_WRITE, format_2);
    if(result != FQ_STATUS_OK){
        fqfile_close(&(f->file[FQ_PAIR_1]));
        return result;
    }
    result = fqbuffer_init(&(f->buffer[FQ_PAIR_1]), b_size);
    if(result != FQ_STATUS_OK){
        fqfile_close(&(f->file[FQ_PAIR_1]));
        fqfile_close(&(f->file[FQ_PAIR_2]));
        return result;
    }
    f->trigger[FQ_PAIR_1] = b_size;
    result = fqbuffer_init(&(f->buffer[FQ_PAIR_2]), b_size);
    if(result != FQ_STATUS_OK){
        fqfile_close(&(f->file[FQ_PAIR_1]));
        fqfile_close(&(f->file[FQ_PAIR_2]));
        fqbuffer_free(&(f->buffer[FQ_PAIR_1]));
        return result;
    }
    f->trigger[FQ_PAIR_2] = b_size;        
    f->n_files = 2;
    f->interleaved = FQ_NONINTERLEAVED;
    return FQ_STATUS_OK;
    
    // fqstatus result;
    // //Space for file0:
    // f->files[0] = malloc(sizeof(fsfile));
    // if(f->files[0] == NULL) return FQ_STATUS_FAIL;
    // //Space for file1:
    // f->files[0] = malloc(sizeof(fsfile));
    // if(f->files[0] == NULL){
    //     free(f->files[0]);
    //     return FQ_STATUS_FAIL;
    // }
    // //Open file0:
    // result = fqfile_open(&(f->file[0]), (char*)filename_1, FQ_MODE_WRITE, format_1);
    // if(result != FQ_STATUS_OK){
    //     free(f->files[0]);
    //     free(f->files[1]);
    // }
    // //Open file1:
    // result = fqfile_open(&(f->file[1]), (char*)filename_2, FQ_MODE_WRITE, format_2);
    // if(result != FQ_STATUS_OK){
    //     fqfile_close(f->files[0]);
    //     free(f->files[0]);
    //     free(f->files[1]);
    // }
    // //Init buffer 0:
    // result = fqbuffer_init(&(f->buffers[0]), b_size);
    // if(result != FQ_STATUS_OK){
    //     fqfile_close(f->files[0]);
    //     fqfile_close(f->files[1]);
    //     free(f->files[0]);
    //     free(f->files[1]);
    //     return result;
    // }
    // f->triggers[0] = b_size;
    // // Init buffer 1:
    // result = fqbuffer_init(&(f->buffers[1]), b_size);
    // if(result != FQ_STATUS_OK){
    //     fqfile_close(f->files[0]);
    //     fqfile_close(f->files[1]);
    //     free(f->files[0]);
    //     free(f->files[1]);
    //     fqbuffer_free(f->buffers[0]);
    //     return result;
    // }
    // f->triggers[1] = b_size;
    // f->n_files = 2;
    // f->interleaved = FQ_NONINTERLEAVED;
    // return FQ_STATUS_OK;
}

fqstatus fqfsout_writechar(fqfsout *f, int file, char c){
    fqstatus result;
    fqflag file_n;
    result = fqbuffer_appendchar(&(f->buffer[file]), c);
    if(f->buffer[file].offset >= f->trigger[file]){
        if((file == FQ_PAIR_2) && (f->interleaved == FQ_INTERLEAVED)) file_n = FQ_PAIR_1;
        else file_n = file;
        fqfile_write(&(f->file[file_n]), f->buffer[file].data, f->buffer[file].offset);
        fqbuffer_reset(&(f->buffer[file]));
    }
    return FQ_STATUS_OK;
}

fqstatus fqfsout_write(fqfsout *f, int file, char *buffer, fqbytecount buffer_n){
    fqstatus result;
    fqflag file_n;
    result = fqbuffer_append(&(f->buffer[file]), buffer, buffer_n);
    if(f->buffer[file].offset >= f->trigger[file]){
        if((file == FQ_PAIR_2) && (f->interleaved == FQ_INTERLEAVED)) file_n = FQ_PAIR_1;
        else file_n = file;
        fqfile_write(&(f->file[file_n]), f->buffer[file].data, f->buffer[file].offset);
        fqbuffer_reset(&(f->buffer[file]));
    }
    return FQ_STATUS_OK;
}

void fqfsout_flush(fqfsout *f, int file){
    fqflag file_n;
    if((file == FQ_PAIR_2) && (f->interleaved == FQ_INTERLEAVED)) file_n = FQ_PAIR_1;
    else file_n = file;
    fqfile_write(&(f->file[file_n]), f->buffer[file].data, f->buffer[file].offset);
    fqbuffer_reset(&(f->buffer[file]));
    fqfile_flush(&(f->file[file_n]));
}

void fqfsout_close(fqfsout *f){
    fqfsout_flush(f, FQ_PAIR_1);
    if((f->n_files == 2) || (f->interleaved == FQ_INTERLEAVED)) fqfsout_flush(f, FQ_PAIR_2);
    fqfile_close(&(f->file[FQ_PAIR_1]));
    if(f->n_files == 2) fqfile_close(&(f->file[FQ_PAIR_2]));
    fqbuffer_free(&(f->buffer[FQ_PAIR_1]));
    if((f->n_files == 2) || (f->interleaved == FQ_INTERLEAVED)) fqbuffer_free(&(f->buffer[FQ_PAIR_2]));
}
