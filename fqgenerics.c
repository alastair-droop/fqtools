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

void fq_generic_read(fqflag pair){}

void fq_generic_block(fqflag pair, char *block, fqbytecount block_n, char final){}

void fq_generic_error(fqflag pair, char error_type, size_t line, char character){
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
