#include "fqheader.h"

// //
// //  fqparser.h
// //  readTest
// //
// //  Created by Alastair Droop on 06/11/2014.
// //  Copyright (c) 2014 Alastair Droop. All rights reserved.
// //
//
// #ifndef __readTest__fqparser__
// #define __readTest__fqparser__
//
//
// //Define parser version string:
// #define FASTQ_PARSER_VERSION "1.2 2015-02-27"
//
// //Define the parser states:
// #define STATE_INIT 0
// #define STATE_HEADER_1 1
// #define STATE_SEQUENCE 2
// #define STATE_SEQUENCE_NEWLINE 3
// #define STATE_HEADER_2 4
// #define STATE_QUALITY 5
// #define STATE_QUALITY_NEWLINE 6
// #define STATE_QUALITY_END 7
//
// //Define the entry points:
// #define ENTRY_START 0
// #define ENTRY_LOOP 1
// #define ENTRY_QUALITY 2
// #define ENTRY_DONE 3
//
// typedef struct {
//     // Permanent state:
//     void *user;
//     fqcallbacks *callbacks;
//     char *input_buffer;
//     char *output_buffer;
//     size_t input_buffer_size;
//     size_t output_buffer_size;
//     char *valid_chars;
//     char entry_point;
//     char error;
//     // Temporary state:
//     char current_state;
//     char current_character;
//     char file_type;
//     char file_offset;
//     char file_status;
//     size_t line_number;
//     size_t characters_read;
//     size_t sequence_length;
//     size_t quality_length;
//     size_t input_buffer_pos;
//     size_t output_buffer_pos;
// } fqparser;
//
// char fqparser_init(fqparser *p, fqcallbacks *callbacks, size_t in_bufsize, size_t out_bufsize, unsigned char seq_flags, char file_type, void *user);
// void fqparser_free(fqparser *p);
//
// char fqparser_step(fqparser *p, fqfile *f);
// char fqparser_pairstep(fqparser *p1, fqparser *p2, fqfile *f1, fqfile *f2);
//
// #endif /* defined(__readTest__fqparser__) */
