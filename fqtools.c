#include "fqheader.h"
//
// fqinset fs_in;
// fqoutset fs_out;
// fqparser_callbacks callbacks;
// fqbuffer read_buffer[2];
//
// fqbytecount readBuffer(void *user, char *b, fqbytecount b_size){
//     return fqinset_read(&fs_in, (char)user, b, b_size);
// }
//
// void startRead(void *user){
//     fqbuffer_appendchar(&(read_buffer[(int)user]), '@');
// }
//
// void endRead(void *user){
//     read_buffer[(int)user].data[read_buffer[(int)user].offset] = '\0';
//     fqoutset_write(&fs_out, (char)user, read_buffer[(int)user].data, read_buffer[(int)user].offset);
//     fqbuffer_reset(&(read_buffer[(int)user]));
// }
//
// void header1Block(void *user, char *block, fqbytecount block_n, char final){
//     fqbuffer_append(&(read_buffer[(int)user]), block, block_n);
//     if(final == 1) fqbuffer_appendchar(&(read_buffer[(int)user]), '\n');
// }
//
// void header2Block(void *user, char *block, fqbytecount block_n, char final){
//     fqbuffer_append(&(read_buffer[(int)user]), block, block_n);
//     if(final == 1) fqbuffer_appendchar(&(read_buffer[(int)user]), '\n');
// }
//
// void sequenceBlock(void *user, char *block, fqbytecount block_n, char final){
//     fqbuffer_append(&(read_buffer[(int)user]), block, block_n);
//     if(final == 1){
//         fqbuffer_appendchar(&(read_buffer[(int)user]), '\n');
//         fqbuffer_appendchar(&(read_buffer[(int)user]), '+');
//     }
// }
//
// void qualityBlock(void *user, char *block, fqbytecount block_n, char final){
//     fqbuffer_append(&(read_buffer[(int)user]), block, block_n);
//     if(final == 1) fqbuffer_appendchar(&(read_buffer[(int)user]), '\n');
// }
//

int main(int argc, const char *argv[]){
    int i, option;
    
    //Parse the global options:
    while((option = getopt(argc, (char* const*)argv, "+hv")) != -1){
        switch(option){
            case 'h':{
                global_help();
                return 0;
            } // End of option case for 'h'
            case 'v':{
                global_version();
                return 0;
            } // End of option case for 'v'
            // case 'v':{} // End of option case for 'v'
        }
    }
    
//     fqstatus result;
//     char finished;
//     fqbuffer_init(&(read_buffer[0]), 10);
//     fqbuffer_init(&(read_buffer[1]), 10);
//     // Set up the callbacks:
//     set_generic_callbacks(&callbacks);
//     callbacks.readBuffer = readBuffer;
//     callbacks.startRead = startRead;
//     callbacks.endRead = endRead;
//     callbacks.header1Block = header1Block;
//     callbacks.sequenceBlock = sequenceBlock;
//     callbacks.header2Block = header2Block;
//     callbacks.qualityBlock = qualityBlock;
//
//     result = fqinset_open_paired(&fs_in, "./fastq/D0000XXXX-01-1.fastq.gz", "./fastq/D0000XXXX-01-2.fastq.gz", FQ_FORMAT_FASTQ_GZ, &callbacks, 10, 10, SEQ_UPPERCASE|SEQ_DNA, FQ_QTYPE_UNKNOWN);
//     // result = fqinset_open_single(&fs_in, "./fastq/D0000XXXX-01-1.fastq.gz", FQ_FORMAT_FASTQ_GZ, FQ_FILESET_NONINTERLEAVED, &callbacks, 10, 10, SEQ_UPPERCASE|SEQ_DNA, FQ_QTYPE_UNKNOWN);
//     if(result != FQ_STATUS_OK){
//         fprintf(stderr, "ERROR: Failed to open input file.\n");
//         return 1;
//     }
//
//     result = fqoutset_open_paired(&fs_out, "./out_1.fastq", "./out_2.fastq", FQ_FORMAT_FASTQ);
//     // result = fqoutset_open_single(&fs_out, "./out_1.fastq", FQ_FORMAT_FASTQ, FQ_FILESET_INTERLEAVED);
//     if(result != FQ_STATUS_OK){
//         fprintf(stderr, "ERROR: Failed to open output file.\n");
//         fqinset_close(&fs_in);
//         return 1;
//     }
//
//     // Step through the input fileset:
//     do finished = fqinset_step(&fs_in);
//     while(finished != 1);
//
//     //Done:
//     fqinset_close(&fs_in);
//     fqoutset_close(&fs_out);
//     fqbuffer_free(&(read_buffer[0]));
//     fqbuffer_free(&(read_buffer[1]));
//     return 0;
}
