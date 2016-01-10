#include "fqheader.h"

fqstatus fqpfile_open(fqpfile *f, const char *filename, fqflag format, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding, fqflag interleaved){
    fqstatus result;
    result = fqfile_open(&(f->file), (char*)filename, FQ_MODE_READ, format);
    if(result != FQ_STATUS_OK) return result;
    result = fqparser_init(&(f->parser), callbacks, in_bufsize, out_bufsize, seq_flags, encoding, interleaved, 0);
    if(result != FQ_STATUS_OK){
        fqfile_close(&(f->file));
        return result;
    }
    return FQ_STATUS_OK;
}

void fqpfile_close(fqpfile *f){
    fqfile_close(&(f->file));
    fqparser_free(&(f->parser));
}

fqstatus fqfsin_open_single(fqfsin *f, const char *filename, fqflag format, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding, fqflag interleaved){
    fqstatus result;
    f->files[0] = malloc(sizeof(fqpfile));
    if(f->files[0] == NULL) return FQ_STATUS_FAIL;
    result = fqpfile_open(f->files[0], filename, format, callbacks, in_bufsize, out_bufsize, seq_flags, encoding, interleaved);
    if(result != FQ_STATUS_OK){
        free(f->files[0]);
        return result;
    }
    f->files[1] = f->files[0];
    f->n_files = 1;
    f->status = FQ_STATUS_OK;
    return FQ_STATUS_OK;
}

fqstatus fqfsin_open_paired(fqfsin *f, const char *filename_1, const char *filename_2, fqflag format_1, fqflag format_2, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding){
    fqstatus result;
    f->files[0] = malloc(sizeof(fqpfile));
    if(f->files[0] == NULL) return FQ_STATUS_FAIL;
    f->files[1] = malloc(sizeof(fqpfile));
    if(f->files[1] == NULL) {
        free(f->files[0]);
        return FQ_STATUS_FAIL;
    }
    result = fqpfile_open(f->files[0], filename_1, format_1, callbacks, in_bufsize, out_bufsize, seq_flags, encoding, FQ_NONINTERLEAVED);
    if(result != FQ_STATUS_OK){
        free(f->files[0]);
        free(f->files[1]);
        return result;
    }
    result = fqpfile_open(f->files[1], filename_2, format_2, callbacks, in_bufsize, out_bufsize, seq_flags, encoding, FQ_NONINTERLEAVED);
    if(result != FQ_STATUS_OK){
        fqpfile_close(f->files[0]);
        free(f->files[0]);
        free(f->files[1]);
        return result;
    }
    f->files[1]->parser.pair = 1;
    f->n_files = 2;
    f->status = FQ_STATUS_OK;
    return FQ_STATUS_OK;
}

char fqfsin_step(fqfsin *f){
    char finished_1, finished_2;
    int error_file, error_pair;
    fqbytecount n1, n2;
    f->status = FQ_STATUS_OK;
    finished_1 = fqparser_step(&(f->files[FQ_PAIR_1]->parser));
    if(f->n_files == 1){
        f->status = f->files[FQ_PAIR_1]->parser.error;
        // SINGLE PARSER
        if(finished_1 == FQ_PARSER_INCOMPLETE) return FQ_PARSER_INCOMPLETE;
        if(f->files[FQ_PAIR_1]->parser.interleaved == FQ_NONINTERLEAVED) return FQ_PARSER_COMPLETE;
        n1 = f->files[FQ_PAIR_1]->parser.n_sequences[FQ_PAIR_1];
        n2 = f->files[FQ_PAIR_1]->parser.n_sequences[FQ_PAIR_2];
        if(n1 == n2) return FQ_PARSER_COMPLETE;
        error_file = FQ_PAIR_1;
        if(n1 < n2) error_pair = FQ_PAIR_1;
        else error_pair = FQ_PAIR_2;
    } else {
        //Double parser
        finished_2 = fqparser_step(&(f->files[FQ_PAIR_2]->parser));
        f->status = f->files[FQ_PAIR_1]->parser.error | f->files[FQ_PAIR_2]->parser.error;
        if(finished_1 == finished_2) return finished_1;
        if(finished_1 == FQ_PARSER_COMPLETE) error_file = FQ_PAIR_2;
        else error_file = FQ_PAIR_1;
        error_pair = error_file;
    }
    f->files[error_file]->parser.callbacks->error(error_pair, FQ_ERROR_PAIR_MISMATCH, f->files[error_file]->parser.line_number, f->files[error_file]->parser.current_character);
    f->files[error_file]->parser.error = 1;
    f->status = FQ_STATUS_FAIL;
    return FQ_PARSER_COMPLETE;
}

void fqfsin_close(fqfsin *f){
    fqpfile_close(f->files[0]);
    free(f->files[0]);
    if(f->n_files == 2){
        fqpfile_close(f->files[1]);
        free(f->files[1]);
    }
}
