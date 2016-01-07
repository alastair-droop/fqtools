#include "fqheader.h"

fqstatus fqoutset_open_single(fqoutset *fs, const char *filename, fqflag format, fqflag interleaved){
    fqstatus result;
    // Open the input file:
    result = fqfile_open(&(fs->file_1), filename, FQ_MODE_WRITE, format);
    if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
    fs->paired = FQ_FILESET_UNPAIRED;
    fs->format = format;
    fs->interleaved = interleaved;
    return FQ_STATUS_OK;
}

fqstatus fqoutset_open_paired(fqoutset *fs, const char *filename_1, const char *filename_2, fqflag format){
    fqstatus result;
    // Open the first input file:
    result = fqfile_open(&(fs->file_1), filename_1, FQ_MODE_WRITE, format);
    if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
    // Open the second input file:
    result = fqfile_open(&(fs->file_2), filename_2, FQ_MODE_WRITE, format);
    if(result != FQ_STATUS_OK){
        fqfile_close(&(fs->file_1));
        return FQ_STATUS_FAIL;
    }
    fs->paired = FQ_FILESET_PAIRED;
    fs->format = format;
    fs->interleaved = FQ_FILESET_NONINTERLEAVED;
    return FQ_STATUS_OK;
}

fqbytecount fqoutset_write(fqoutset *fs, char file, char *buffer, fqbytecount buffer_n){
    if((fs->interleaved == FQ_FILESET_INTERLEAVED) && (file == FQ_FILESET_PAIR_2)) file = FQ_FILESET_PAIR_1;
    if((file == FQ_FILESET_PAIR_2) && (fs->paired == FQ_FILESET_UNPAIRED)) return 0;
    if(file == FQ_FILESET_PAIR_1) return fqfile_write(&(fs->file_1), buffer, buffer_n);
    if(file == FQ_FILESET_PAIR_2) return fqfile_write(&(fs->file_2), buffer, buffer_n);
    return 0;
}

fqbytecount fqoutset_writechar(fqoutset *fs, char file, char c){
    if((fs->interleaved == FQ_FILESET_INTERLEAVED) && (file == FQ_FILESET_PAIR_2)) file = FQ_FILESET_PAIR_1;
    if((file == FQ_FILESET_PAIR_2) && (fs->paired == FQ_FILESET_UNPAIRED)) return 0;
    if(file == FQ_FILESET_PAIR_1) return fqfile_writechar(&(fs->file_1), c);
    if(file == FQ_FILESET_PAIR_2) return fqfile_writechar(&(fs->file_2), c);
    return 0;
}

void fqoutset_flush(fqoutset *fs, char file){
    if(file == FQ_FILESET_PAIR_1) return fqfile_flush(&(fs->file_1));
    if(file == FQ_FILESET_PAIR_2) return fqfile_flush(&(fs->file_2));
}


void fqoutset_close(fqoutset *fs){
    fqfile_close(&(fs->file_1));
    if(fs->paired == FQ_FILESET_PAIRED) fqfile_close(&(fs->file_2));
}


fqstatus fqinset_open_single(fqinset *fs, const char *filename, fqflag format, fqflag interleaved, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding){
    fqstatus result;
    // Open the input file:
    result = fqfile_open(&(fs->file_1), filename, FQ_MODE_READ, format);
    if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
    // Initalise the input parser:
    result = fqparser_init(&(fs->parser_1), callbacks, in_bufsize, out_bufsize, seq_flags, encoding, (void *)FQ_FILESET_PAIR_1);
    if(result != FQ_STATUS_OK){
        fqfile_close(&(fs->file_1));
        return FQ_STATUS_FAIL;
    }
    fs->paired = FQ_FILESET_UNPAIRED;
    fs->format = format;
    fs->interleaved = interleaved;
    return FQ_STATUS_OK;
}

fqstatus fqinset_open_paired(fqinset *fs, const char *filename_1, const char *filename_2, fqflag format, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding){
    fqstatus result;
    // Open the first input file:
    result = fqfile_open(&(fs->file_1), filename_1, FQ_MODE_READ, format);
    if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
    // Open the second input file:
    result = fqfile_open(&(fs->file_2), filename_2, FQ_MODE_READ, format);
    if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
    // Initalise the first input parser:
    result = fqparser_init(&(fs->parser_1), callbacks, in_bufsize, out_bufsize, seq_flags, encoding, (void *)FQ_FILESET_PAIR_1);
    if(result != FQ_STATUS_OK){
        fqfile_close(&(fs->file_1));
        fqfile_close(&(fs->file_2));
        return FQ_STATUS_FAIL;
    }
    // Initalise the second input parser:
    result = fqparser_init(&(fs->parser_2), callbacks, in_bufsize, out_bufsize, seq_flags, encoding, (void *)FQ_FILESET_PAIR_2);
    if(result != FQ_STATUS_OK){
        fqfile_close(&(fs->file_1));
        fqfile_close(&(fs->file_2));
        fqparser_free(&(fs->parser_1));
        return FQ_STATUS_FAIL;
    }
    fs->paired = FQ_FILESET_PAIRED;
    fs->format = format;
    fs->interleaved = FQ_FILESET_NONINTERLEAVED;
    return FQ_STATUS_OK;
}

fqbytecount fqinset_read(fqinset *fs, char file, char *buffer, fqbytecount buffer_n){
    if((fs->interleaved == FQ_FILESET_INTERLEAVED) && (file == FQ_FILESET_PAIR_2)) file = FQ_FILESET_PAIR_1;
    if((file == FQ_FILESET_PAIR_2) && (fs->paired == FQ_FILESET_UNPAIRED)) return 0;
    if(file == FQ_FILESET_PAIR_1) return fqfile_read(&(fs->file_1), buffer, buffer_n);
    if(file == FQ_FILESET_PAIR_2) return fqfile_read(&(fs->file_2), buffer, buffer_n);
    return 0;
}

char fqinset_step(fqinset *fs){
    char finished_1, finished_2;
    if(fs->paired == FQ_FILESET_UNPAIRED){
        return fqparser_step(&(fs->parser_1));
    } else {
        finished_1 = fqparser_step(&(fs->parser_1));
        finished_2 = fqparser_step(&(fs->parser_2));
        if(finished_1 == finished_2) return finished_1;
        if(fs->parser_1.current_state == FQ_PARSER_STATE_INIT){
            fs->parser_2.callbacks->error(fs->parser_2.user, FQ_ERROR_PAIR_MISMATCH, fs->parser_2.line_number, fs->parser_2.current_character);
            fs->parser_2.error = 1;
        } else {
            fs->parser_1.callbacks->error(fs->parser_1.user, FQ_ERROR_PAIR_MISMATCH, fs->parser_1.line_number, fs->parser_1.current_character);
            fs->parser_1.error = 1;
        }
        return FQ_PARSER_COMPLETE;
    }
}

void fqinset_close(fqinset *fs){
    fqfile_close(&(fs->file_1));
    fqfile_close(&(fs->file_2));
    fqparser_free(&(fs->parser_1));
    fqparser_free(&(fs->parser_2));
}
