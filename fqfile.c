#include "fqheader.h"

// The general file manipulations. These delegate to specific callbacks:
fqstatus fqfile_open(fqfile *f, const char *filename, fqflag mode, fqflag format){
    if((mode == FQ_MODE_READ) && (format == FQ_FORMAT_FASTQ)) return fqfile_open_read_file_fastq_uncompressed(f, filename);
    if((mode == FQ_MODE_READ) && (format == FQ_FORMAT_FASTQ_GZ)) return fqfile_open_read_file_fastq_compressed(f, filename);
    return FQ_STATUS_FAIL;
}

void fqfile_close(fqfile *f){
    if(f->close != NULL) f->close(f);
}

int fqfile_read(fqfile *f, fqbuffer *b){
    if(f->read != NULL) return f->read(f, b);
    return 0;
}

char fqfile_eof(fqfile *f){
    if(f->eof != NULL) return f->eof(f);
    return 1;
}

// Callbacks for opening files:
fqstatus fqfile_open_read_file_fastq_uncompressed(fqfile *f, const char *filename){
    if(filename == NULL){
        // Reading from a stream, (i.e. stdin):
        f->handle = stdin;
        f->type = FQ_TYPE_PIPE;
        f->eof = fqfile_eof_pipe;
        f->close = fqfile_close_pipe;
    } else {
        // Reading from a named file:
        f->handle = (void*)fopen(filename, "r");
        if(f->handle == NULL) return FQ_STATUS_FAIL;
        f->type = FQ_TYPE_FILE;
        f->eof = fqfile_eof_file_fastq_uncompressed;
        f->close = fqfile_close_file_fastq_uncompressed;
    }
    f->format = FQ_FORMAT_FASTQ;
    f->read = fqfile_read_fastq_uncompressed;
    return FQ_STATUS_OK;
}

fqstatus fqfile_open_read_file_fastq_compressed(fqfile *f, const char *filename){
    if(filename == NULL){
        // Reading from a stream, (i.e. stdin):
        f->handle = gzdopen(dup(fileno(stdin)), "r");
        f->type = FQ_TYPE_PIPE;
        f->eof = fqfile_eof_pipe;
        f->close = fqfile_close_pipe;
    } else {
        f->handle = (void*)gzopen(filename, "r");
        if(f->handle == NULL) return FQ_STATUS_FAIL;
        f->type = FQ_TYPE_FILE;
        f->eof = fqfile_eof_file_fastq_compressed;
        f->close = fqfile_close_file_fastq_compressed;
    }
    f->format = FQ_FORMAT_FASTQ_GZ;
    f->read = fqfile_read_fastq_compressed;
    return FQ_STATUS_OK;
}

//
// // int fqfile_open_fastq_write(fqfile *f, const char *filename){
// //     if(filename == NULL){
// //         f->handle = stdout;
// //         f->type = FQFILE_TYPE_PIPE;
// //         f->close_file = fqfile_close_pipe;
// //     } else {
// //         f->handle = (void*)fopen(filename, "w");
// //         f->type = FQFILE_TYPE_FILE;
// //         f->close_file = fqfile_close_file;
// //     }
// //     f->read_buf = NULL;
// //     f->write_buf = fqfile_writebuf_file;
// //     f->eof = fqfile_eof_file;
// //     return FQ_STATUS_OK;
// // }
//
//
// // int fqfile_open_fastqgz_write(fqfile *f, const char *filename){
// //     return FQ_STATUS_FAIL;
// // }
//
// Callbacks for closing files:
void fqfile_close_file_fastq_uncompressed(void *f){
    if((FILE*)(((fqfile*)f)->handle) != NULL) fclose((FILE*)(((fqfile*)f)->handle));
}

void fqfile_close_file_fastq_compressed(void *f){
    if((gzFile*)(((fqfile*)f)->handle) != NULL) gzclose((gzFile*)(((fqfile*)f)->handle));
}

void fqfile_close_pipe(void *f){
    return;
}

// Callbacks for reading file data into a buffer:
// These functions read data directly into the beginning of the buffer, reading a maximum of buffer->size
// bytes. The resulting buffer is null-terminated so it can be read like a string later on.
int fqfile_read_fastq_uncompressed(void *f, fqbuffer *b){
    if(((fqfile*)f)->mode != FQ_MODE_READ) return 0; // Check that we're allowed to read from this file
    int bytes_read = (int)fread(b->data, sizeof(char), b->size, (FILE*)(((fqfile*)f)->handle)); // Read into the buffer
    b->data[bytes_read] = '\0'; // Mark the end of the read data with a NULL
    return bytes_read;
}

int fqfile_read_fastq_compressed(void *f, fqbuffer *b){
    if(((fqfile*)f)->mode != FQ_MODE_READ) return 0; // Check that we're allowed to read from this file
    int bytes_read = gzread((gzFile*)(((fqfile*)f)->handle), b->data, (int)b->size); // Read into the buffer
    b->data[bytes_read] = '\0'; // Mark the end of the read data with a NULL
    return bytes_read;
}

// // Callbacks to determine end of file:
char fqfile_eof_file_fastq_uncompressed(void *f){
    return feof((FILE*)(((fqfile*)f)->handle));
}

char fqfile_eof_file_fastq_compressed(void *f){
    return gzeof((gzFile*)(((fqfile*)f)->handle));
}

char fqfile_eof_pipe(void *f){
    return fqfile_eof_file_fastq_uncompressed(f);
}

// Guess the file format by its name:
fqflag guess_filename_format(const char *filename){
    if(filename == NULL) return guess_stdin_format();
    if(strlen(filename) < 3) return guess_file_format(filename);
    if(strncmp(filename + (strlen(filename) - 3), ".gz", 3) == 0) return FQ_FORMAT_FASTQ_GZ;
    return FQ_FORMAT_FASTQ;
}

// Guess the file format by peeking into the file, and looking at the magic number:
fqflag guess_file_format(const char *filename){
    FILE *f;
    unsigned char c1 = 0;
    unsigned char c2 = 0;
    f = fopen(filename, "r");
    if(f == NULL) return FQ_FORMAT_UNKNOWN;
    fread(&c1, sizeof(char), 1, f);
    fread(&c2, sizeof(char), 1, f);
    fclose(f);
    if((c1 == 31) && (c2 == 139)) return FQ_FORMAT_FASTQ_GZ;
    return FQ_FORMAT_FASTQ;
}

fqflag guess_stdin_format(){
    // Currently, I do not know how to safely pop the magic number from STDIN and
    // successfully pop it back. Untill I can do this without breaking the stream,
    // we simply pass out "No idea" and rely on a safe default in main.
    return FQ_FORMAT_UNKNOWN;
}
