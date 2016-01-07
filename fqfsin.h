#include "fqheader.h"

typedef struct {
    fqfile file;
    fqparser parser;
} fqpfile;

fqstatus fqpfile_open(fqpfile *f, const char *filename, fqflag format, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding);
void fqpfile_close(fqpfile *f);

typedef struct{
    char n_files;
    fqpfile *files[2];
} fqfsin;

fqstatus fqfsin_prepare(fqfsin *f, char n_filenames, const char *filenames[], fqparser_callbacks *callbacks, fqglobal opt);
fqstatus fqfsin_open_single(fqfsin *f, const char *filename, fqflag format, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding);
fqstatus fqfsin_open_paired(fqfsin *f, const char *filename_1, const char *filename_2, fqflag format_1, fqflag format_2, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding);

char fqfsin_step(fqfsin *f);
void fqfsin_close(fqfsin *f);
