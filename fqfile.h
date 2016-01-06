#include "fqheader.h"

// Define the file mode flag bits:
typedef unsigned char fqflag;
#define FQ_MODE_READ 0
#define FQ_MODE_WRITE 1

#define FQ_TYPE_PIPE 0
#define FQ_TYPE_FILE 1

#define FQ_FORMAT_UNKNOWN 0
#define FQ_FORMAT_FASTQ 1
#define FQ_FORMAT_FASTQ_GZ 2
#define FQ_FORMAT_BAM 3

// Define the structure that will describe a single file:
typedef struct {
    fqflag mode; // The mode (read or write)
    fqflag type; // The type (file or pipe)
    fqflag format; // The file format
    void *handle; // The file handle.
    void (*close)(void *f);
    fqbytecount (*read)(void *f, char *buffer, fqbytecount buffer_n);
    // fqbytecount (*write)(void *f, fqbuffer *b);
    char (*eof)(void *f);
} fqfile;

// Main file handle functions:
fqstatus fqfile_open(fqfile *f, const char *filename, fqflag mode, fqflag format);
void fqfile_close(fqfile *f);
fqbytecount fqfile_read(void *f, char *buffer, fqbytecount buffer_n);
// fqbytecount fqfile_write(fqfile *f, fqbuffer *b);
char fqfile_eof(fqfile *f);

// Callbacks to open different file types:
fqstatus fqfile_open_read_file_fastq_uncompressed(fqfile *f, const char *filename);
fqstatus fqfile_open_read_file_fastq_compressed(fqfile *f, const char *filename);
// fqstatus fqfile_open_write_file_fastq_uncompressed(fqfile *f, const char *filename);
// fqstatus fqfile_open_write_file_fastq_compressed(fqfile *f, const char *filename);

// Callbacks to close different file types:
void fqfile_close_file_fastq_uncompressed(void *f);
void fqfile_close_file_fastq_compressed(void *f);
void fqfile_close_pipe(void *f);

// Callbacks to read & write different file types:
fqbytecount fqfile_read_fastq_uncompressed(void *f, char *buffer, fqbytecount buffer_n);
fqbytecount fqfile_read_fastq_compressed(void *f, char *buffer, fqbytecount buffer_n);
// fqbytecount fqfile_write_fastq_uncompressed(void *f, fqbuffer *b);
// fqbytecount fqfile_write_fastq_compressed(void *f, fqbuffer *b);

// Callbacks to determine end of file:
char fqfile_eof_file_fastq_uncompressed(void *f);
char fqfile_eof_file_fastq_compressed(void *f);
char fqfile_eof_pipe(void *f);

// Functions to guess the file type by name or magic number:
fqflag guess_filename_format(const char *filename);
fqflag guess_file_format(const char *filename);
fqflag guess_stdin_format();
