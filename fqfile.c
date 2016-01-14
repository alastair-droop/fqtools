// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "fqheader.h"

// The general file manipulations. These delegate to specific callbacks:
fqstatus fqfile_open(fqfile *f, char *filename, fqflag mode, fqflag format){
    if(format == FQ_FORMAT_UNKNOWN) format = guess_file_format(filename);
    if((mode == FQ_MODE_READ) && (format == FQ_FORMAT_FASTQ)) return fqfile_open_read_file_fastq_uncompressed(f, filename);
    if((mode == FQ_MODE_READ) && (format == FQ_FORMAT_FASTQ_GZ)) return fqfile_open_read_file_fastq_compressed(f, filename);
    if((mode == FQ_MODE_READ) && (format == FQ_FORMAT_BAM)) return fqfile_open_read_file_bam(f, filename);
    if((mode == FQ_MODE_WRITE) && (format == FQ_FORMAT_FASTQ)) return fqfile_open_write_file_fastq_uncompressed(f, filename);
    if((mode == FQ_MODE_WRITE) && (format == FQ_FORMAT_FASTQ_GZ)) return fqfile_open_write_file_fastq_compressed(f, filename);
    if((mode == FQ_MODE_WRITE) && (format == FQ_FORMAT_FASTA)) return fqfile_open_write_file_fastq_uncompressed(f, filename);
    if((mode == FQ_MODE_WRITE) && (format == FQ_FORMAT_FASTA_GZ)) return fqfile_open_write_file_fastq_compressed(f, filename);
    return FQ_STATUS_FAIL;
}

void fqfile_close(fqfile *f){
    if(f->close != NULL) f->close(f);
}

fqbytecount fqfile_read(fqfile *f, char *buffer, fqbytecount buffer_n){
    if(f->read != NULL) return f->read(f, buffer, buffer_n);
    return 0;
}

fqbytecount fqfile_write(fqfile *f, char *buffer, fqbytecount buffer_n){
    if(f->write != NULL) return f->write(f, buffer, buffer_n);
    return 0;
}

fqbytecount fqfile_writechar(fqfile *f, char c){
    if(f->write != NULL) return f->writechar(f, c);
    return 0;
}

char fqfile_eof(fqfile *f){
    if(f->eof != NULL) return f->eof(f);
    return 1;
}

void fqfile_flush(fqfile *f){
    if(f->flush != NULL) f->flush(f);
}

// Callbacks for opening files:
fqstatus fqfile_open_read_file_fastq_uncompressed(fqfile *f, char *filename){
    if(filename == NULL){
        // Reading from a stream, (i.e. stdin):
        f->handle = stdin;
        f->type = FQ_TYPE_PIPE;
        f->eof = fqfile_eof_pipe;
        f->flush = fqfile_flush_pipe;
        f->close = fqfile_close_pipe;
    } else {
        // Reading from a named file:
        f->handle = (void*)fopen(filename, "r");
        if(f->handle == NULL) return FQ_STATUS_FAIL;
        f->type = FQ_TYPE_FILE;
        f->eof = fqfile_eof_file_fastq_uncompressed;
        f->flush = fqfile_flush_file_fastq_uncompressed;
        f->close = fqfile_close_file_fastq_uncompressed;
    }
    f->mode = FQ_MODE_READ;
    f->format = FQ_FORMAT_FASTQ;
    f->read = fqfile_read_fastq_uncompressed;
    f->write = NULL;
    f->writechar = NULL;
    return FQ_STATUS_OK;
}

fqstatus fqfile_open_read_file_fastq_compressed(fqfile *f, char *filename){
    if(filename == NULL){
        // Reading from a stream, (i.e. stdin):
        f->handle = gzdopen(dup(fileno(stdin)), "r");
        f->type = FQ_TYPE_PIPE;
        f->eof = fqfile_eof_pipe;
        f->flush = fqfile_flush_pipe;
        f->close = fqfile_close_file_fastq_compressed;
    } else {
        f->handle = (void*)gzopen(filename, "r");
        if(f->handle == NULL) return FQ_STATUS_FAIL;
        f->type = FQ_TYPE_FILE;
        f->eof = fqfile_eof_file_fastq_compressed;
        f->flush = fqfile_flush_file_fastq_compressed;
        f->close = fqfile_close_file_fastq_compressed;
    }
    f->mode = FQ_MODE_READ;
    f->format = FQ_FORMAT_FASTQ_GZ;
    f->read = fqfile_read_fastq_compressed;
    f->write = NULL;
    f->writechar = NULL;
    return FQ_STATUS_OK;
}

fqstatus fqfile_open_read_file_bam(fqfile *f, char *filename){
    if(filename == NULL){
        // Reading from a stream, (i.e. stdin):
        f->handle = (samFile*)sam_open("-", "r");
        if(f->handle == NULL) return FQ_STATUS_FAIL;
        f->type = FQ_TYPE_PIPE;
        f->eof = fqfile_eof_file_bam;
        f->flush = fqfile_flush_file_bam;
        f->close = fqfile_close_file_bam;
    } else {
        f->handle = (samFile*)sam_open(filename, "r");
        if(f->handle == NULL) return FQ_STATUS_FAIL;
        f->type = FQ_TYPE_FILE;
        f->eof = fqfile_eof_file_bam;
        f->flush = fqfile_flush_file_bam;
        f->close = fqfile_close_file_bam;
    }
    f->sam_header = sam_hdr_read((samFile*)(f->handle));
    if(f->sam_header == NULL){
        sam_close((samFile*)(f->handle));
        return FQ_STATUS_FAIL;
    }
    f->bam_data = bam_init1();
    f->mode = FQ_MODE_READ;
    f->format = FQ_FORMAT_BAM;
    f->read = fqfile_read_bam;
    f->write = NULL;
    f->writechar = NULL;
    return FQ_STATUS_OK;
}

fqstatus fqfile_open_write_file_fastq_uncompressed(fqfile *f, char *filename){
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
    f->writechar = fqfile_writechar_fastq_uncompressed;
    return FQ_STATUS_OK;
}

fqstatus fqfile_open_write_file_fastq_compressed(fqfile *f, char *filename){
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
    f->writechar = fqfile_writechar_fastq_compressed;
    return FQ_STATUS_OK;
}

// Callbacks for closing files:
void fqfile_close_file_fastq_uncompressed(fqfile *f){
    if((FILE*)(f->handle) != NULL) fclose((FILE*)(f->handle));
}

void fqfile_close_file_fastq_compressed(fqfile *f){
    if((gzFile*)(f->handle) != NULL) gzclose((gzFile*)(f->handle));
}

void fqfile_close_file_bam(fqfile *f){
    if((samFile*)(f->handle) != NULL) sam_close((samFile*)(f->handle));
    if(f->sam_header != NULL) bam_hdr_destroy(f->sam_header);
}

void fqfile_close_pipe(fqfile *f){
    return;
}

// Callbacks for reading file data into a buffer:
// These functions read data directly into the beginning of the buffer, reading a maximum of buffer.size
// bytes. The resulting buffer is always null-terminated so it can be read like a string later on.
fqbytecount fqfile_read_fastq_uncompressed(fqfile *f, char *buffer, fqbytecount buffer_n){
    if(((fqfile*)f)->mode != FQ_MODE_READ) return 0; // Check that we're allowed to read from this file
    return (fqbytecount)fread(buffer, sizeof(char), buffer_n, (FILE*)(((fqfile*)f)->handle)); // Read into the buffer
}

fqbytecount fqfile_read_fastq_compressed(fqfile *f, char *buffer, fqbytecount buffer_n){
    if(((fqfile*)f)->mode != FQ_MODE_READ) return 0; // Check that we're allowed to read from this file
    return (fqbytecount)gzread((gzFile*)(((fqfile*)f)->handle), buffer, (int)buffer_n); // Read into the buffer
}

fqbytecount fqfile_read_bam(fqfile *f, char *buffer, fqbytecount buffer_n){
    char read_reversed;
    char *read_pair;
    fqbytecount n_total = 0;
    int bytes_read = sam_read1((samFile*)(f->handle), f->sam_header, f->bam_data);
    if(bytes_read >= 0){
        //Find out how long out output buffer will be:
        n_total = f->bam_data->core.l_qname + (2 * f->bam_data->core.l_qseq) + 6;
        if(n_total > buffer_n){
            fprintf(stderr, "ERROR: input buffer not long enough for a full BAM read (%lu bytes)\n", n_total);
            return 0;
        }
        n_total = 0;
        read_reversed = f->bam_data->core.flag & BAM_FREVERSE;
        read_pair = "0";
        if((f->bam_data->core.flag & BAM_FREAD1) && !(f->bam_data->core.flag & BAM_FREAD2)) read_pair = "1";
        if((f->bam_data->core.flag & BAM_FREAD2) && !(f->bam_data->core.flag & BAM_FREAD1)) read_pair = "2";
        //Put the header into the buffer:
        memcpy(buffer, "@", 1);
        n_total++;
        memcpy(buffer+n_total, bam_get_qname(f->bam_data), f->bam_data->core.l_qname - 1);
        n_total += f->bam_data->core.l_qname - 1;
        memcpy(buffer+n_total, ":", 1);
        n_total++;
        memcpy(buffer+n_total, read_pair, 1);
        n_total++;
        memcpy(buffer+n_total, "\n", 1);
        n_total++;
        // Put the sequence into the buffer:
        convert_sequence((char*)bam_get_seq(f->bam_data), buffer+n_total, f->bam_data->core.l_qseq, read_reversed);
        n_total += f->bam_data->core.l_qseq;
        memcpy(buffer+n_total, "\n+\n", 3);
        n_total += 3;
        // Put the quality in:
        convert_quality((char*)bam_get_qual(f->bam_data), buffer+n_total, f->bam_data->core.l_qseq, read_reversed, 33);
        n_total += f->bam_data->core.l_qseq;        
        memcpy(buffer+n_total, "\n", 1);
    }
    return n_total;
}

// Callbacks to write buffer data to file:
// These functions write from the start of the buffer to the position marked by b.offset.
fqbytecount fqfile_write_fastq_uncompressed(fqfile *f, char *buffer, fqbytecount buffer_n){
    if(((fqfile*)f)->mode != FQ_MODE_WRITE) return 0; // Check that we're allowed to write to this file
    return (fqbytecount)fwrite(buffer, sizeof(char), buffer_n, (FILE*)(f->handle));
}

fqbytecount fqfile_write_fastq_compressed(fqfile *f, char *buffer, fqbytecount buffer_n){
    if(((fqfile*)f)->mode != FQ_MODE_WRITE) return 0; // Check that we're allowed to write to this file
    return (fqbytecount)gzwrite((gzFile*)(f->handle), buffer, (unsigned int)buffer_n);
}

fqbytecount fqfile_writechar_fastq_uncompressed(fqfile *f, char c){
    if(((fqfile*)f)->mode != FQ_MODE_WRITE) return 0; // Check that we're allowed to write to this file
    return (fqbytecount)fwrite(&c, sizeof(char), 1, (FILE*)(f->handle));
}

fqbytecount fqfile_writechar_fastq_compressed(fqfile *f, char c){
    if(((fqfile*)f)->mode != FQ_MODE_WRITE) return 0; // Check that we're allowed to write to this file
    return (fqbytecount)gzwrite((gzFile*)(f->handle), &c, 1);
}


// Callbacks to determine end of file:
char fqfile_eof_file_fastq_uncompressed(fqfile *f){
    return feof((FILE*)(((fqfile*)f)->handle));
}

char fqfile_eof_file_fastq_compressed(fqfile *f){
    return gzeof((gzFile*)(((fqfile*)f)->handle));
}

char fqfile_eof_file_bam(fqfile *f){
    return 0; // Placeholder for now.
}

char fqfile_eof_pipe(fqfile *f){
    return fqfile_eof_file_fastq_uncompressed(f);
}

//Callbacks to flush the file:
void fqfile_flush_file_fastq_uncompressed(fqfile *f){
    fflush((FILE*)(((fqfile*)f)->handle));
}

void fqfile_flush_file_fastq_compressed(fqfile *f){
    gzflush((gzFile*)(((fqfile*)f)->handle), 0);
}

void fqfile_flush_file_bam(fqfile *f){
    // Placeholder for now.
}

void fqfile_flush_pipe(fqfile *f){
    fflush((FILE*)(((fqfile*)f)->handle));
}

// Generate a filename from a file stem:
char* generate_filename(char *stem, char rep_chr, char pair, fqflag format){
    if(stem == NULL) return NULL;
    char *replacement, *extension, *output;
    size_t i, output_length;
    //Get the replacement string:
    switch(pair){
        case 1: {replacement = "1"; break;}
        case 2: {replacement = "2"; break;}
        case 0: default: {replacement = ""; break;}
    }
    //Get the extension string:
    switch(format){
        case FQ_FORMAT_FASTQ: {extension = ".fastq"; break;}
        case FQ_FORMAT_FASTQ_GZ: {extension = ".fastq.gz"; break;}
        case FQ_FORMAT_FASTA: {extension = ".fasta"; break;}
        case FQ_FORMAT_FASTA_GZ: {extension = ".fasta.gz"; break;}
        case FQ_FORMAT_BAM: {extension = ".bam"; break;}
        case FQ_FORMAT_UNKNOWN: default: {extension = "";}
    }
    //Iterate through the original string counting the times we see the replacement string:
    output_length = strlen(stem) + 1;
    for(i=0; i < strlen(stem); i++) if(stem[i] == rep_chr) output_length += strlen(replacement) - 1;
    output_length += strlen(extension);
    // Malloc the output string:
    output = (char*)malloc(output_length * sizeof(char));
    if(output == NULL){
        fprintf(stderr, "ERROR: failed to allocate filename\n");
        return NULL;
    }
    memset(output, 0, output_length);
    //Copy the stem into the output:
    char *ptr_stem = stem;
    char *ptr_output = output;
    for(i=0; i < strlen(stem); i++){
        if(ptr_stem[0] == rep_chr){
            memcpy(ptr_output, replacement, strlen(replacement));
            ptr_stem++;
            ptr_output += strlen(replacement);
        } else {
            memcpy(ptr_output, ptr_stem, 1);
            ptr_stem++;
            ptr_output++;
        }
    }
    memcpy(ptr_output, extension, strlen(extension));
    return(output);
}

// Guess the file format by its name:
fqflag guess_filename_format(char *filename){
    fqflag result = FQ_FORMAT_UNKNOWN;
    int i;
    if(filename == NULL) return guess_stdin_format();
    char *f = malloc(sizeof(char) * (strlen(filename) + 1));
    if(f == NULL) return FQ_FORMAT_UNKNOWN;
    f[strlen(filename)] = '\0';
    for(i=0; i < strlen(filename); i++) f[i] = tolower(filename[i]);
    if(strstr(f, ".fastq") != NULL) result = FQ_FORMAT_FASTQ;
    if(strstr(f, ".fastq.gz") != NULL) result = FQ_FORMAT_FASTQ_GZ;
    if(strstr(f, ".fasta") != NULL) result = FQ_FORMAT_FASTA;
    if(strstr(f, ".fasta.gz") != NULL) result = FQ_FORMAT_FASTA_GZ;
    if(strstr(f, ".bam") != NULL) result = FQ_FORMAT_BAM;
    free(f);
    return result;
}

// Guess the file format by peeking into the file, and looking at the magic number:
fqflag guess_file_format(char *filename){
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

void convert_sequence(char *in_seq, char *out_seq, int n, char reversed){
    int8_t seq_comp_table[16] = { 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15 };
    char *p = out_seq;
    int i;
    if(reversed == 1){
        for(i = n - 1; i > -1; --i){
            *p = seq_nt16_str[seq_comp_table[bam_seqi(in_seq, i)]];
            p++;
        }
    } else {
        for(i = 0; i < n; ++i){
            *p = seq_nt16_str[bam_seqi(in_seq, i)];
            p++;
        }        
    }
}

void convert_quality(char *in_qual, char *out_qual, int n, char reversed, char offset){
    char *p = out_qual;
    int i;
    if(reversed == 1){
        for(i = n - 1; i > -1; --i){
            *p = in_qual[i] + offset;
            p++;
        }
    } else {
        for(i = 0; i < n; ++i){
            *p = (char)(in_qual[i]) + offset;
            p++;
        }
    }
}
