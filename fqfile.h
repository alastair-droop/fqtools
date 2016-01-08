#include "fqheader.h"

// Define the structure that will describe a single file:
typedef struct fqfile fqfile;
struct fqfile {
    fqflag mode; // The mode (read or write)
    fqflag type; // The type (file or pipe)
    fqflag format; // The file format
    void *handle; // The file handle.
    void (*close)(fqfile *f);
    fqbytecount (*read)(fqfile *f, char *buffer, fqbytecount buffer_n);
    fqbytecount (*write)(fqfile *f, char *buffer, fqbytecount buffer_n);
    fqbytecount (*writechar)(fqfile *f, char c);
    char (*eof)(fqfile *f);
    void (*flush)(fqfile *f);
    //Specific BAM parsing parts:
    bam_hdr_t *sam_header;
    bam1_t *bam_data;
    char read_pair;
    char read_reversed;
};

// Main file handle functions:
fqstatus fqfile_open(fqfile *f, char *filename, fqflag mode, fqflag format);
void fqfile_close(fqfile *f);
fqbytecount fqfile_read(fqfile *f, char *buffer, fqbytecount buffer_n);
fqbytecount fqfile_write(fqfile *f, char *buffer, fqbytecount buffer_n);
fqbytecount fqfile_writechar(fqfile *f, char c);
char fqfile_eof(fqfile *f);
void fqfile_flush(fqfile *f);

// Callbacks to open different file types:
fqstatus fqfile_open_read_file_fastq_uncompressed(fqfile *f, char *filename);
fqstatus fqfile_open_read_file_fastq_compressed(fqfile *f, char *filename);
fqstatus fqfile_open_read_file_bam(fqfile *f, char *filename);
fqstatus fqfile_open_write_file_fastq_uncompressed(fqfile *f, char *filename);
fqstatus fqfile_open_write_file_fastq_compressed(fqfile *f, char *filename);

// Callbacks to close different file types:
void fqfile_close_file_fastq_uncompressed(fqfile *f);
void fqfile_close_file_fastq_compressed(fqfile *f);
void fqfile_close_file_bam(fqfile *f);
void fqfile_close_pipe(fqfile *f);

// Callbacks to read & write different file types:
fqbytecount fqfile_read_fastq_uncompressed(fqfile *f, char *buffer, fqbytecount buffer_n);
fqbytecount fqfile_read_fastq_compressed(fqfile *f, char *buffer, fqbytecount buffer_n);
fqbytecount fqfile_read_bam(fqfile *f, char *buffer, fqbytecount buffer_n);
fqbytecount fqfile_write_fastq_uncompressed(fqfile *f, char *buffer, fqbytecount buffer_n);
fqbytecount fqfile_write_fastq_compressed(fqfile *f, char *buffer, fqbytecount buffer_n);
fqbytecount fqfile_writechar_fastq_uncompressed(fqfile *f, char c);
fqbytecount fqfile_writechar_fastq_compressed(fqfile *f, char c);

// Callbacks to determine end of file:
char fqfile_eof_file_fastq_uncompressed(fqfile *f);
char fqfile_eof_file_fastq_compressed(fqfile *f);
char fqfile_eof_file_bam(fqfile *f);
char fqfile_eof_pipe(fqfile *f);

// Callbacks to flush a file:
void fqfile_flush_file_fastq_uncompressed(fqfile *f);
void fqfile_flush_file_fastq_compressed(fqfile *f);
void fqfile_flush_file_bam(fqfile *f);
void fqfile_flush_pipe(fqfile *f);

// Generate a filename from a file stem:
char* generate_filename(char *stem, char rep_chr, char pair, fqflag format);

// Functions to guess the file type by name or magic number:
fqflag guess_filename_format(char *filename);
fqflag guess_file_format(char *filename);
fqflag guess_stdin_format();

//BAM processing functions:
void convert_sequence(char *in_seq, char *out_seq, int n, char reversed);
void convert_quality(char *in_qual, char *out_qual, int n, char reversed, char offset);
