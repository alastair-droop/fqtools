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

//Define the parser callback struct:
typedef struct {
	char interrupt;
	void (*startRead)(void *user);
	void (*endRead)(void *user);
	void (*header1Block)(void *user, char *block, size_t n, char final);
	void (*header2Block)(void *user, char *block, size_t n, char final);
	void (*sequenceBlock)(void *user, char *block, size_t n, char final);
	void (*qualityBlock)(void *user, char *block, size_t n, char final);
	void (*error)(void *user, char error_type, size_t line, char character);
} fqparser_callbacks;

typedef struct {
    // IO:
    fqfileset *fs_in;
    fqfileset *fs_out;
    // Permanent state:
    void *user;
    fqparser_callbacks *callbacks;
    // fqfileset *output_fileset;
    // char *valid_chars;
    char entry_point;
    char error;
    // Temporary state:
    char current_state;
    char current_character;
    size_t line_number;
    size_t characters_read;
    size_t sequence_length;
    size_t quality_length;
} fqparser;

fqstatus fqparser_init(fqparser *p, fqfileset *fs_in, fqfileset *fs_out, fqparser_callbacks *callbacks, void *user);
void fqparser_free(fqparser *p);

fqstatus fqparser_step(fqparser *p);
//
// char fqparser_step(fqparser *p, fqfile *f);
// char fqparser_pairstep(fqparser *p1, fqparser *p2, fqfile *f1, fqfile *f2);
//
// #endif /* defined(__readTest__fqparser__) */
