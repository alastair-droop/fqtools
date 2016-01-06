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
    void (*startRead)(void *user);
    void (*endRead)(void *user);
    void (*header1Block)(void *user, fqbuffer *block, char final);
    void (*header2Block)(void *user, fqbuffer *block, char final);
    void (*sequenceBlock)(void *user, fqbuffer *block, char final);
    void (*qualityBlock)(void *user, fqbuffer *block, char final);
    void (*error)(void *user, char error_type, size_t line, char character);
    fqbytecount (*readBuffer)(char *b, fqbytecount b_size);
} fqparser_callbacks;

typedef struct {
    //IO:
    char *input_buffer;
    fqbytecount input_buffer_size;
    fqbytecount input_buffer_max_size;    
    fqbytecount input_buffer_offset;
    char *output_buffer;
    fqbytecount output_buffer_size;
    fqbytecount output_buffer_max_size;
    fqbytecount output_buffer_offset;
    //Callbacks:
    fqparser_callbacks *callbacks;
    // // IO:
    // fqbuffer *buffer_in;
    // fqbuffer *buffer_out;
    // // Permanent state:
    void *user;
    // fqparser_callbacks *callbacks;
    // // fqfileset *output_fileset;
    // // char *valid_chars;
    // char entry_point;
    // char error;

    // Temporary state:
    // fqbytecount sequence_length;
    // fqbytecount quality_length;
    char current_character;
    // char current_state;
    // fqbytecount index_buffer_in;
    // fqbytecount length_buffer_in;
    // size_t line_number;
} fqparser;

// fqstatus fqparser_init(fqparser *p, fqbuffer *buffer_in, fqbuffer *buffer_out, fqparser_callbacks *callbacks, void *user);
// fqstatus fqparser_init(fqparser *p, fqparser_callbacks *callbacks, char *input_buffer, char *output_buffer, fqbytecount in_bufsize, fqbytecount out_bufsize, void *user);
fqstatus fqparser_init(fqparser *p, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, void *user);
void fqparser_free(fqparser *p);

char fqparser_step(fqparser *p);
