#include "fqheader.h"

int main(int argc, const char *argv[]){
    fqfile f;
    int res;
    
    // Initialize the input file:
    res = fqfile_init(f, argv[1], FQFILE_TYPE_FASTQ_UNCOMPRESSED, FQFILE_MODE_READ, 100);
    if(res != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: Failed to load input file\n");
        fqfile_close(f);
    }
    
    // //Do stuff!
    // printf("Buffer size is %zd\n", f.buffer.size);
    // printf("Buffer offset is %zd\n", f.buffer.offset);
    //
    // //Read in a load of buffer data:
    // res = fqfile_readbuf(&f);
    // printf("Read %d bytes\n", res);
    // f.buffer.data[99] = '\0';
    // printf("Data: >>%s<<\n", f.buffer.data);
    //
    // //Close the input file:
    // fqfile_close(&f);
    
    //All done!
    return 0;
}
