#include "fqheader.h"

int main(int argc, const char *argv[]){
    fqfile f_in, f_out;
    fqbuffer buffer;
    int res, bytes_read;
    const char *filename_in = NULL;
    const char *filename_out = NULL;
    fqflag format;
    
    // Initialize the input file:    
    if(argc > 1){
        // Read from command line:
        filename_in = argv[1];
        format = guess_filename_format(filename_in);
        if(format == FQ_FORMAT_UNKNOWN) format = guess_file_format(filename_in);
    } else {
        // Read from stdin:
        format = guess_stdin_format();
        if(format == FQ_FORMAT_UNKNOWN) format = FQ_FORMAT_FASTQ;
    }
    res = fqfile_open(&f_in, filename_in, FQ_MODE_READ, format);
    if(res != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: Failed to load input file\n");
        fqfile_close(&f_in);
        return 1;
    }
    
    //Initialize the output file:
    res = fqfile_open(&f_out, filename_out, FQ_MODE_WRITE, FQ_FORMAT_FASTQ_GZ);
    if(res != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: Failed to load output file\n");
        fqfile_close(&f_in);
        fqfile_close(&f_out);
        return 1;
    }
    fprintf(stderr, "output file mode: %d\n", f_out.mode);
    fprintf(stderr, "output file type: %d\n", f_out.type);
    fprintf(stderr, "output file format: %d\n", f_out.format);
    
    //Initialize the buffer:
    res = fqbuffer_init(&buffer, 10);
    if(res != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: Failed to initialize buffer\n");
        fqbuffer_free(&buffer);
    }
    
    while(1){
        // Read some bytes:
        bytes_read = fqfile_read(&f_in, &buffer);
        fqfile_write(&f_out, &buffer);
        if(fqfile_eof(&f_in)) break;
    }

    //All done!
    fqbuffer_free(&buffer);
    fqfile_close(&f_in);
    fqfile_close(&f_out);
    fprintf(stderr, "done\n");
    return 0;
}
