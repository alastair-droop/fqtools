#include "fqheader.h"

int main(int argc, const char *argv[]){
    fqfile f;
    fqbuffer buffer;
    int res, bytes_read;
    const char *filename = NULL;
    fqflag format;

    //Initialize the buffer:
    res = fqbuffer_init(&buffer, 25000);
    if(res != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: Failed to initialize buffer\n");
        fqbuffer_free(&buffer);
    }
    
    // Initialize the input file:    
    if(argc > 1){
        // Read from command line:
        filename = argv[1];
        format = guess_filename_format(filename);
        if(format == FQ_FORMAT_UNKNOWN) format = guess_file_format(filename);
    } else {
        // Read from stdin:
        format = guess_stdin_format();
        if(format == FQ_FORMAT_UNKNOWN) format = FQ_FORMAT_FASTQ;
    }
    res = fqfile_open(&f, filename, FQ_MODE_READ, format);
    if(res != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: Failed to load input file\n");
        fqfile_close(&f);
        return 1;
    }
    
    while(1){
        // Read some bytes:
        bytes_read = fqfile_read(&f, &buffer);
        // printf("%d bytes read\n", bytes_read);
        printf("%s", buffer.data);
        if(fqfile_eof(&f)){
            printf("\n");
            break;
        }
    }

    //All done!
    fqbuffer_free(&buffer);
    fqfile_close(&f);
    // printf("done\n");
    return 0;
}
