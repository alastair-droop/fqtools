#include "fqheader.h"

fqstatus fqpfile_open(fqpfile *f, const char *filename, fqflag format, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding){
    fqstatus result;
    result = fqfile_open(&(f->file), (char*)filename, FQ_MODE_READ, format);
    if(result != FQ_STATUS_OK) return result;
    result = fqparser_init(&(f->parser), callbacks, in_bufsize, out_bufsize, seq_flags, encoding, 0);
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

fqstatus fqfsin_prepare(fqfsin *f, char n_filenames, const char *filenames[], fqparser_callbacks *callbacks, fqglobal opt){
    if(n_filenames == 0) return fqfsin_open_single(f, NULL, opt.input_format, callbacks, opt.input_bufsize, opt.output_bufsize, opt.sequence_flags, opt.quality);
    if(n_filenames == 1) return fqfsin_open_single(f, (char*)filenames[0], opt.input_format, callbacks, opt.input_bufsize, opt.output_bufsize, opt.sequence_flags, opt.quality);
    return fqfsin_open_paired(f, (char*)filenames[0], (char*)filenames[1], opt.input_format, opt.input_format, callbacks, opt.input_bufsize, opt.output_bufsize, opt.sequence_flags, opt.quality);
}

fqstatus fqfsin_open_single(fqfsin *f, const char *filename, fqflag format, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding){
    fqstatus result;
    f->files[0] = malloc(sizeof(fqpfile));
    if(f->files[0] == NULL) return FQ_STATUS_FAIL;
    result = fqpfile_open(f->files[0], filename, format, callbacks, in_bufsize, out_bufsize, seq_flags, encoding);
    if(result != FQ_STATUS_OK){
        free(f->files[0]);
        return result;
    }
    f->files[1] = f->files[0];
    f->n_files = 1;
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
    result = fqpfile_open(f->files[0], filename_1, format_1, callbacks, in_bufsize, out_bufsize, seq_flags, encoding);
    if(result != FQ_STATUS_OK){
        free(f->files[0]);
        free(f->files[1]);
        return result;
    }
    result = fqpfile_open(f->files[1], filename_2, format_2, callbacks, in_bufsize, out_bufsize, seq_flags, encoding);
    if(result != FQ_STATUS_OK){
        fqpfile_close(f->files[0]);
        free(f->files[0]);
        free(f->files[1]);
        return result;
    }
    f->files[1]->parser.user = (void*)1;
    f->n_files = 2;
    return FQ_STATUS_OK;
}

char fqfsin_step(fqfsin *f){
    char finished_1, finished_2;
    if(f->n_files == 1) {
        return fqparser_step(&(f->files[0]->parser));
    } else {
        finished_1 = fqparser_step(&(f->files[0]->parser));
        finished_2 = fqparser_step(&(f->files[1]->parser));
        if(finished_1 == finished_2) return finished_1;
        if(f->files[0]->parser.current_state == FQ_PARSER_STATE_INIT){
            f->files[1]->parser.callbacks->error(f->files[1]->parser.user, FQ_ERROR_PAIR_MISMATCH, f->files[1]->parser.line_number, f->files[1]->parser.current_character);
            f->files[1]->parser.error = 1;
        } else {
            f->files[0]->parser.callbacks->error(f->files[0]->parser.user, FQ_ERROR_PAIR_MISMATCH, f->files[0]->parser.line_number, f->files[0]->parser.current_character);
            f->files[0]->parser.error = 1;
        }
        return FQ_PARSER_COMPLETE;
    }
}


void fqfsin_close(fqfsin *f){
    fqpfile_close(f->files[0]);
    free(f->files[0]);
    if(f->n_files == 2){
        fqpfile_close(f->files[1]);
        free(f->files[1]);
    }
}
