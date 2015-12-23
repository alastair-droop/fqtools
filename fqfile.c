#include "fqheader.h"

// The general file manipulations. These delegate to specific callbacks:
fqstatus fqfile_open(fqfile *f, const char *filename, fqflag mode, fqflag format){
    if((mode == FQ_MODE_READ) && (format == FQ_FORMAT_FASTQ)) return fqfile_open_read_file_fastq_uncompressed(f, filename);
    if((mode == FQ_MODE_READ) && (format == FQ_FORMAT_FASTQ_GZ)) return fqfile_open_read_file_fastq_compressed(f, filename);
    if((mode == FQ_MODE_WRITE) && (format == FQ_FORMAT_FASTQ)) return fqfile_open_write_file_fastq_uncompressed(f, filename);
    if((mode == FQ_MODE_WRITE) && (format == FQ_FORMAT_FASTQ_GZ)) return fqfile_open_write_file_fastq_compressed(f, filename);
    return FQ_STATUS_FAIL;
}

void fqfile_close(fqfile *f){
    if(f->close != NULL) f->close(f);
}

int fqfile_read(fqfile *f, fqbuffer *b){
    if(f->read != NULL) return f->read(f, b);
    return 0;
}

int fqfile_write(fqfile *f, fqbuffer *b){
    if(f->write != NULL) return f->write(f, b);
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
    f->mode = FQ_MODE_READ;
    f->format = FQ_FORMAT_FASTQ;
    f->read = fqfile_read_fastq_uncompressed;
    f->write = NULL;
    return FQ_STATUS_OK;
}

fqstatus fqfile_open_read_file_fastq_compressed(fqfile *f, const char *filename){
    if(filename == NULL){
        // Reading from a stream, (i.e. stdin):
        f->handle = gzdopen(dup(fileno(stdin)), "r");
        f->type = FQ_TYPE_PIPE;
        f->eof = fqfile_eof_pipe;
        f->close = fqfile_close_file_fastq_compressed;
    } else {
        f->handle = (void*)gzopen(filename, "r");
        if(f->handle == NULL) return FQ_STATUS_FAIL;
        f->type = FQ_TYPE_FILE;
        f->eof = fqfile_eof_file_fastq_compressed;
        f->close = fqfile_close_file_fastq_compressed;
    }
    f->mode = FQ_MODE_READ;
    f->format = FQ_FORMAT_FASTQ_GZ;
    f->read = fqfile_read_fastq_compressed;
    f->write = NULL;
    return FQ_STATUS_OK;
}

fqstatus fqfile_open_write_file_fastq_uncompressed(fqfile *f, const char *filename){
    if(filename == NULL){
        // Writing from a stream, (i.e. stdout):
        f->handle = stdout;
        f->type = FQ_TYPE_PIPE;
        f->eof = fqfile_eof_pipe;
        f->close = fqfile_close_pipe;
    } else {
        // Writing to a named file:
        f->handle = (void*)fopen(filename, "w");
        if(f->handle == NULL) return FQ_STATUS_FAIL;
        f->type = FQ_TYPE_FILE;
        f->eof = fqfile_eof_file_fastq_uncompressed;
        f->close = fqfile_close_file_fastq_uncompressed;
    }
    f->mode = FQ_MODE_WRITE;
    f->format = FQ_FORMAT_FASTQ;
    f->read = NULL;
    f->write = fqfile_write_fastq_uncompressed;
    return FQ_STATUS_OK;
}

fqstatus fqfile_open_write_file_fastq_compressed(fqfile *f, const char *filename){
    if(filename == NULL){
        // Writing from a stream, (i.e. stdout):
        f->handle = gzdopen(dup(fileno(stdout)), "w");
        f->type = FQ_TYPE_PIPE;
        f->eof = fqfile_eof_pipe;
        f->close = fqfile_close_file_fastq_compressed;
    } else {
        // Writing to a named file:
        f->handle = (void*)gzopen(filename, "w");
        if(f->handle == NULL) return FQ_STATUS_FAIL;
        f->type = FQ_TYPE_FILE;
        f->eof = fqfile_eof_file_fastq_compressed;
        f->close = fqfile_close_file_fastq_compressed;
    }
    f->mode = FQ_MODE_WRITE;
    f->format = FQ_FORMAT_FASTQ_GZ;
    f->read = NULL;
    f->write = fqfile_write_fastq_compressed;
    return FQ_STATUS_OK;
}


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
// These functions read data directly into the beginning of the buffer, reading a maximum of buffer.size
// bytes. The resulting buffer is always null-terminated so it can be read like a string later on.
int fqfile_read_fastq_uncompressed(void *f, fqbuffer *b){
    if(((fqfile*)f)->mode != FQ_MODE_READ) return 0; // Check that we're allowed to read from this file
    int bytes_read = (int)fread(b->data, sizeof(char), b->size, (FILE*)(((fqfile*)f)->handle)); // Read into the buffer
    b->data[bytes_read] = '\0'; // Mark the end of the read data with a NULL
    b->offset = bytes_read; // Set the offset
    return bytes_read;
}

int fqfile_read_fastq_compressed(void *f, fqbuffer *b){
    if(((fqfile*)f)->mode != FQ_MODE_READ) return 0; // Check that we're allowed to read from this file
    int bytes_read = gzread((gzFile*)(((fqfile*)f)->handle), b->data, (int)b->size); // Read into the buffer
    b->data[bytes_read] = '\0'; // Mark the end of the read data with a NULL
    b->offset = bytes_read; // Set the offset    
    return bytes_read;
}

// Callbacks to write buffer data to file:
// These functions write from the start of the buffer to the position marked by b.offset.
int fqfile_write_fastq_uncompressed(void *f, fqbuffer *b){
    if(((fqfile*)f)->mode != FQ_MODE_WRITE) return 0; // Check that we're allowed to write to this file
    if(b->offset == 0) return 0; // No point writing anything if there is nothing to write!
    int bytes_written = (int)fwrite(b->data, sizeof(char), b->offset, (FILE*)(((fqfile*)f)->handle));
    return bytes_written;
}

int fqfile_write_fastq_compressed(void *f, fqbuffer *b){
    if(((fqfile*)f)->mode != FQ_MODE_WRITE) return 0; // Check that we're allowed to write to this file
    if(b->offset == 0) return 0; // No point writing anything if there is nothing to write!
    int bytes_written = (int)gzwrite((gzFile*)(((fqfile*)f)->handle), b->data, (unsigned int)b->offset);
    return bytes_written;
}

// Callbacks to determine end of file:
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
