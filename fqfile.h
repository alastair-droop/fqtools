#include "fqheader.h"

// Define the file modes:
#define FQFILE_MODE_READ 0x01
#define FQFILE_MODE_WRITE 0x02

// Define the possible file types:
#define FQFILE_TYPE_FASTQ_UNCOMPRESSED 0x01
#define FQFILE_TYPE_FASTQ_COMPRESSED 0x02

// Define the struct that will hold a single file:
typedef struct{
    char mode;
    char type;
    //The file handle (type depends of file type):
    void *handle;
    //Callbacks:
    void (*close_file)(void *f);
    int (*read_buf)(void *f, fqbuffer *b);
    char (*eof)(void *f);
} fqfile;

// Main file handle functions:
int fqfile_open(fqfile *f, const char *filename, char type, char mode);
void fqfile_close(fqfile *f);
int fqfile_readbuf(fqfile *f, fqbuffer *b);
char fqfile_eof(fqfile *f);

// Callbacks to open different file types:
int fqfile_open_file(fqfile *f, const char *filename, char mode);
int fqfile_open_gzfile(fqfile *f, const char *filename, char mode);

// Callbacks to close different file types:
void fqfile_close_file(void *f);
void fqfile_close_gzfile(void *f);

// Callbacks to read different file types:
int fqfile_readbuf_file(void *f, fqbuffer *b);
int fqfile_readbuf_gzfile(void *f, fqbuffer *b);

// Callbacks to determine end of file:
char fqfile_eof_file(void *f);
char fqfile_eof_gzfile(void *f);
