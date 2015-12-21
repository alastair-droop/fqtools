#include "fqheader.h"

int main(int argc, const char *argv[]){
    fqfile f;
    fqbuffer buffer;
    int res;
    
    //Initialize the buffer:
    res = fqbuffer_init(&buffer, 1000000);
    if(res != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: Failed to initialize buffer\n");
        fqbuffer_free(&buffer);
    }
        
    // Initialize the input file:
    res = fqfile_open(&f, argv[1], FQFILE_FORMAT_FASTQ_UNCOMPRESSED, FQFILE_TYPE_FILE, FQFILE_MODE_READ);
    if(res != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: Failed to load input file\n");
        fqfile_close(&f);
    }
    
    // Read some bytes:
    int bytes_read = fqfile_readbuf(&f, &buffer);
    printf("Read %d bytes from file\n", bytes_read);
    printf("Buffer is: >>%s<<\n", buffer.data);
    printf("End of file?: >>%d<<\n", fqfile_eof(&f));
    
    
    //All done!
    fqbuffer_free(&buffer);
    fqfile_close(&f);
    return 0;
}
