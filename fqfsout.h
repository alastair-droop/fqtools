#include "fqheader.h"

typedef struct{
    fqfile file;
    fqbuffer buffer;
    fqbytecount trigger;
} fqbfile;

fqstatus fqbfile_open(fqbfile *f, const char *filename, fqflag format, fqbytecount b_size);
fqstatus fqbfile_writechar(fqbfile *f, char c);
fqstatus fqbfile_write(fqbfile *f, char *buffer, fqbytecount buffer_n);

void fqbfile_flush(fqbfile *f);
void fqbfile_close(fqbfile *f);

typedef struct{
    char n_files;
    fqbfile *files[2];
} fqfsout;

fqstatus fqfsout_open_single(fqfsout *f, const char *filename, fqflag format, fqbytecount b_size);
fqstatus fqfsout_open_paired(fqfsout *f, const char *filename_1, const char *filename_2, fqflag format_1, fqflag format_2, fqbytecount b_size);
fqstatus fqfsout_writechar(fqfsout *f, int file, char c);
fqstatus fqfsout_write(fqfsout *f, int file, char *buffer, fqbytecount buffer_n);
void fqfsout_flush(fqfsout *f, int file);
void fqfsout_close(fqfsout *f);
