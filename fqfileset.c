#include "fqheader.h"

fqstatus fqfileset_open_single(fqfileset *fs, const char *filename, fqflag mode, fqflag format, fqflag interleaved){
    fqstatus result;
    // Open the input file:
    result = fqfile_open(&(fs->file_1), filename, mode, format);
    if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
    fs->mode = mode;
    fs->paired = FQ_FILESET_UNPAIRED;
    fs->format = format;
    fs->interleaved = interleaved;
    return FQ_STATUS_OK;
}

fqstatus fqfileset_open_pair(fqfileset *fs, const char *filename_1, const char *filename_2, fqflag mode, fqflag format){
    fqstatus result;
    // Open the first input file:
    result = fqfile_open(&(fs->file_1), filename_1, mode, format);
    if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
    result = fqfile_open(&(fs->file_2), filename_2, mode, format);
    if(result != FQ_STATUS_OK){
        fqfile_close(&(fs->file_1));
        return FQ_STATUS_FAIL;
    }
    fs->mode = mode;
    fs->paired = FQ_FILESET_PAIRED;
    fs->format = format;
    fs->interleaved = FQ_FILESET_NONINTERLEAVED;
    return FQ_STATUS_OK;
}


fqbytecount fqfileset_read(fqfileset *fs, char file, char *buffer, fqbytecount buffer_n){
    if((fs->interleaved == FQ_FILESET_INTERLEAVED) && (file == FQ_FILESET_PAIR_2)) file = FQ_FILESET_PAIR_1;
    if((file == FQ_FILESET_PAIR_2) && (fs->paired == FQ_FILESET_UNPAIRED)) return 0;
    if(file == FQ_FILESET_PAIR_1) return fqfile_read(&(fs->file_1), buffer, buffer_n);
    if(file == FQ_FILESET_PAIR_2) return fqfile_read(&(fs->file_2), buffer, buffer_n);
    return 0;
}

fqbytecount fqfileset_write(fqfileset *fs, char file, char *buffer, fqbytecount buffer_n){
    if((fs->interleaved == FQ_FILESET_INTERLEAVED) && (file == FQ_FILESET_PAIR_2)) file = FQ_FILESET_PAIR_1;
    if((file == FQ_FILESET_PAIR_2) && (fs->paired == FQ_FILESET_UNPAIRED)) return 0;
    if(file == FQ_FILESET_PAIR_1) return fqfile_write(&(fs->file_1), buffer, buffer_n);
    if(file == FQ_FILESET_PAIR_2) return fqfile_write(&(fs->file_2), buffer, buffer_n);
    return 0;
}

fqbytecount fqfileset_writechar(fqfileset *fs, char file, char c){
    if((fs->interleaved == FQ_FILESET_INTERLEAVED) && (file == FQ_FILESET_PAIR_2)) file = FQ_FILESET_PAIR_1;
    if((file == FQ_FILESET_PAIR_2) && (fs->paired == FQ_FILESET_UNPAIRED)) return 0;
    if(file == FQ_FILESET_PAIR_1) return fqfile_writechar(&(fs->file_1), c);
    if(file == FQ_FILESET_PAIR_2) return fqfile_writechar(&(fs->file_2), c);
    return 0;
}

void fqfileset_close(fqfileset *fs){
    fqfile_close(&(fs->file_1));
    if(fs->paired == FQ_FILESET_PAIRED) fqfile_close(&(fs->file_2));
}

// fqstatus fqfileset_open(fqfileset *fs, const char *filename_1, const char *filename_2, fqflag mode, fqflag format, size_t buffer_size){
//     fqstatus result;
//     //Open file 1:
//     result = fqfilebuffer_open(&(fs->file_1), filename_1, mode, format, buffer_size);
//     if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
//     if(filename_2 != NULL){
//         //Open file 2:
//         result = fqfilebuffer_open(&(fs->file_2), filename_2, mode, format, buffer_size);
//         if(result != FQ_STATUS_OK){
//             fqfilebuffer_close(&(fs->file_1));
//             return FQ_STATUS_FAIL;
//         }
//        fs->paired = FQ_FILESET_PAIRED;
//     } else {
//         fs->paired = FQ_FILESET_UNPAIRED;
//     }
//     fs->mode = mode;
//     fs->format = format;
//     return FQ_STATUS_OK;
// }
//
// void fqfileset_close(fqfileset *fs){
//     fqfilebuffer_close(&(fs->file_1));
//     if(fs->paired == FQ_FILESET_PAIRED) fqfilebuffer_close(&(fs->file_2));
//     return;
// }
//
// fqstatus fqfileset_read(fqfileset *fs, fqbytecount *bytes_read_1, fqbytecount *bytes_read_2){
//     *bytes_read_1 = fqfilebuffer_read(&(fs->file_1));
//     if(fs->paired == FQ_FILESET_PAIRED) *bytes_read_2 = fqfilebuffer_read(&(fs->file_2));
//     else *bytes_read_2 = 0;
//     return FQ_STATUS_OK;
// }
//
// fqbuffer* fqfileset_getBuffer(fqfileset *fs, char pair){
//     if(pair == FQ_FILESET_PAIR_1) return &(fs->file_1.buffer);
//     return &(fs->file_2.buffer);
// }
//
// void fqfileset_write(fqfileset *fs){
//     fqfilebuffer_write(&(fs->file_1));
//     if(fs->paired == FQ_FILESET_PAIRED) fqfilebuffer_write(&(fs->file_2));
// }
//
// fqstatus fqfileset_appendchar(fqfileset *fs, char pair, char c){
//     if(pair == FQ_FILESET_PAIR_1) return fqbuffer_appendchar(&(fs->file_1.buffer), c);
//     if(pair == FQ_FILESET_PAIR_2) return fqbuffer_appendchar(&(fs->file_2.buffer), c);
//     return FQ_STATUS_FAIL;
// }
//
// fqstatus fqfileset_append(fqfileset *fs, char pair, char *data, fqbytecount size){
//     if(pair == FQ_FILESET_PAIR_1) return fqbuffer_append(&(fs->file_1.buffer), data, size);
//     if(pair == FQ_FILESET_PAIR_2) return fqbuffer_append(&(fs->file_2.buffer), data, size);
//     return FQ_STATUS_FAIL;
// }
