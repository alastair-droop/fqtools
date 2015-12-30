#include "fqheader.h"

void fq_generic_read(void *user){}

void fq_generic_block(void *user, fqbuffer *block, char final){}

void fq_generic_error(void *user, char error_type, size_t line, char character){
	char *message;
	switch(error_type){
		case FQ_ERROR_MISSING_HEADER:
			message = "expected header sequence";
			break;
		case FQ_ERROR_INVALID_SEQUENCE_CHARACTER:
			message = "invalid sequence character";
			break;
		case FQ_ERROR_INVALID_QUALITY_CHARACTER:
			message = "invalid quality character";
			break;
		case FQ_ERROR_INCOMPLETE_FINAL_READ:
			message = "incomplete final read";
			break;
		case FQ_ERROR_PAIR_MISMATCH:
			message = "file pair read count mismatch";
			break;
		case FQ_ERROR_FILE:
			message = "file read error";
			break;
		default:
			message = "general error";
	}
	if(line == 0) fprintf(stderr, "ERROR: %s\n", message);
	else fprintf(stderr, "ERROR [line %ld]: %s\n", line, message);
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
