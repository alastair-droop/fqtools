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
