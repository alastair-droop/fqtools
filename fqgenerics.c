#include "fqheader.h"

void fq_generic_read(void *user){}

void fq_generic_block(void *user, char *block, fqbytecount block_n, char final){}

void fq_generic_error(void *user, char error_type, size_t line, char character){
    switch(error_type){
        case FQ_ERROR_MISSING_HEADER:
            fprintf(stderr, "ERROR [line %lu]: expected header sequence\n", line);
            break;
        case FQ_ERROR_INVALID_SEQUENCE_CHARACTER:
            fprintf(stderr, "ERROR [line %lu]: invalid sequence character (%c)\n", line, character);
            break;
        case FQ_ERROR_INVALID_QUALITY_CHARACTER:
            fprintf(stderr, "ERROR [line %lu]: invalid quality character (%c)\n", line, character);
            break;
        case FQ_ERROR_INCOMPLETE_FINAL_READ:
            fprintf(stderr, "ERROR [line %lu]: incomplete final read\n", line);
            break;
        case FQ_ERROR_PAIR_MISMATCH:
            fprintf(stderr, "ERROR: file pair read count mismatch\n");
            break;
        case FQ_ERROR_FILE:
            fprintf(stderr, "ERROR: file read error\n");
            break;
        default:
            fprintf(stderr, "ERROR: general error\n");
    }
}

void set_generic_callbacks(fqparser_callbacks *callbacks){
    callbacks->interrupt = 0;
    callbacks->startRead = fq_generic_read;
    callbacks->endRead = fq_generic_read;
    callbacks->error = fq_generic_error;
    callbacks->header1Block = fq_generic_block;
    callbacks->header2Block = fq_generic_block;
    callbacks->sequenceBlock = fq_generic_block;
    callbacks->qualityBlock = fq_generic_block;
}
