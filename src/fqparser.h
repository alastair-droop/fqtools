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

// Define the parser states:
#define FQ_PARSER_STATE_INIT 0
#define FQ_PARSER_STATE_HEADER_1 1
#define FQ_PARSER_STATE_SEQUENCE 2
#define FQ_PARSER_STATE_SEQUENCE_NEWLINE 3
#define FQ_PARSER_STATE_HEADER_2 4
#define FQ_PARSER_STATE_QUALITY 5
#define FQ_PARSER_STATE_QUALITY_NEWLINE 6
#define FQ_PARSER_STATE_QUALITY_END 7

// Define the parser entry points:
#define FQ_PARSER_ENTRY_START 0
#define FQ_PARSER_ENTRY_LOOP 1
#define FQ_PARSER_ENTRY_QUALITY 2
#define FQ_PARSER_ENTRY_DONE 3

// Define the possible error types:
#define FQ_ERROR_UNKNOWN 0
#define FQ_ERROR_MISSING_HEADER 1
#define FQ_ERROR_INVALID_SEQUENCE_CHARACTER 2
#define FQ_ERROR_INVALID_QUALITY_CHARACTER 3
#define FQ_ERROR_INCOMPLETE_FINAL_READ 4
#define FQ_ERROR_PAIR_MISMATCH 5
#define FQ_ERROR_FILE 6

// Define the parser step values:
#define FQ_PARSER_INCOMPLETE 0
#define FQ_PARSER_COMPLETE 1

//Define the parser callback struct:
typedef struct {
    char interrupt;
    void (*startRead)(fqflag pair);
    void (*endRead)(fqflag pair);
    void (*header1Block)(fqflag pair, char *block, fqbytecount block_n, char final);
    void (*header2Block)(fqflag pair, char *block, fqbytecount block_n, char final);
    void (*sequenceBlock)(fqflag pair, char *block, fqbytecount block_n, char final);
    void (*qualityBlock)(fqflag pair, char *block, fqbytecount block_n, char final);
    void (*error)(fqflag pair, char error_type, size_t line, char character);
    fqbytecount (*readBuffer)(fqflag pair, char *b, fqbytecount b_size);
} fqparser_callbacks;

typedef struct {
    //IO:
    char *input_buffer;
    fqbytecount input_buffer_size;
    fqbytecount input_buffer_max;    
    fqbytecount input_buffer_offset;
    char *output_buffer;
    fqbytecount output_buffer_max;
    fqbytecount output_buffer_offset;
    //Callbacks:
    fqparser_callbacks *callbacks;
    // Permanent state:
    fqflag interleaved;
    char pair;
    char *valid_sequence_characters;
    char *valid_quality_characters;
    // Temporary state:
    fqbytecount n_sequences[2];
    fqbytecount sequence_length;
    fqbytecount quality_length;
    char entry_point;
    char current_character;
    char current_state;
    char error;
    fqbytecount line_number;
} fqparser;

fqstatus fqparser_init(fqparser *p, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding, fqflag interleaved, fqflag pair);
void fqparser_free(fqparser *p);

void fqparser_setValidSequenceCharacters(fqparser *p, fqflag flags);
void fqparser_setValidQualityCharacters(fqparser *p, fqflag encoding);
void fqparser_showValidSequenceCharacters(fqparser *p);
void fqparser_showValidQualityCharacters(fqparser *p);

char fqparser_step(fqparser *p);
