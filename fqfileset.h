#include "fqheader.h"

#define FQ_FILESET_UNPAIRED 0
#define FQ_FILESET_PAIRED 1

#define FQ_FILESET_PAIR_1 0
#define FQ_FILESET_PAIR_2 1

#define FQ_FILESET_NONINTERLEAVED 0
#define FQ_FILESET_INTERLEAVED 1

typedef struct {
    fqflag paired;
    fqflag format;
    fqflag interleaved;
    fqfile file_1;
    fqfile file_2;
} fqoutset;

fqstatus fqoutset_prepare(fqoutset *f, fqflag paired, char specify_name, fqglobal opt);

fqstatus fqoutset_open_single(fqoutset *fs, char *filename, fqflag format, fqflag interleaved);
fqstatus fqoutset_open_paired(fqoutset *fs, char *filename_1, char *filename_2, fqflag format);

fqbytecount fqoutset_write(fqoutset *fs, char file, char *buffer, fqbytecount buffer_n);
fqbytecount fqoutset_writechar(fqoutset *fs, char file, char c);

void fqoutset_flush(fqoutset *fs, char file);
void fqoutset_close(fqoutset *fs);

typedef struct {
    fqflag paired;
    fqflag format;
    fqflag interleaved;
    fqfile file_1;
    fqfile file_2;
    fqparser parser_1;
    fqparser parser_2;
} fqinset;

fqstatus fqinset_prepare(fqinset *f, fqflag paired, const char *names[], char names_n, fqparser_callbacks *callbacks, fqglobal opt);
fqstatus fqinset_open_single(fqinset *fs, char *filename, fqflag format, fqflag interleaved, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding);
fqstatus fqinset_open_paired(fqinset *fs, char *filename_1, char *filename_2, fqflag format, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding);

fqbytecount fqinset_read(fqinset *fs, char file, char *buffer, fqbytecount buffer_n);

char fqinset_step(fqinset *fs);

// fqbytecount fqinset_read(fqinset *fs, char file, char *buffer, fqbytecount buffer_n);
// fqbytecount fqinset_writechar(fqinset *fs, char file, char c);

void fqinset_close(fqinset *fs);



// typedef struct {
//     fqflag mode;
//     fqflag paired;
//     fqflag format;
//     fqflag interleaved;
//     fqfile file_1;
//     fqfile file_2;
//     fqparser parser_1;
//     fqparser parser_2;
// } fqfileset;
//
// fqstatus fqfileset_open_single_write(fqfileset *fs, char *filename, fqflag format, fqflag interleaved);
// fqstatus fqfileset_open_pair_write(fqfileset *fs, char *filename_1, char *filename_2, fqflag format);
//
// fqbytecount fqfileset_read(fqfileset *fs, char file, char *buffer, fqbytecount buffer_n);
//
// void fqfileset_close(fqfileset *fs);
// //
// // fqstatus fqfileset_read(fqfileset *fs, fqbytecount *bytes_read_1, fqbytecount *bytes_read_2);
// // void fqfileset_write(fqfileset *fs);
// //
// // fqbuffer* fqfileset_getBuffer(fqfileset *fs, char pair);
// //
// // fqstatus fqfileset_appendchar(fqfileset *fs, char pair, char c);
// // fqstatus fqfileset_append(fqfileset *fs, char pair, char *data, fqbytecount size);
