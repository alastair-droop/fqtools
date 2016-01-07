#include "fqheader.h"

// Define the global variables:
fqinset f_in;
fqoutset f_out;
fqparser_callbacks callbacks;
fqbuffer read_buffer[2];

fqbytecount readBuffer(void *user, char *b, fqbytecount b_size){
    return fqinset_read(&f_in, (char)user, b, b_size);
}

void startRead(void *user){
    fqbuffer_appendchar(&(read_buffer[(int)user]), '@');
}

void endRead(void *user){
    read_buffer[(int)user].data[read_buffer[(int)user].offset] = '\0';
    fqoutset_write(&f_out, (char)user, read_buffer[(int)user].data, read_buffer[(int)user].offset);
    fqbuffer_reset(&(read_buffer[(int)user]));
}

void header1Block(void *user, char *block, fqbytecount block_n, char final){
    fqbuffer_append(&(read_buffer[(int)user]), block, block_n);
    if(final == 1) fqbuffer_appendchar(&(read_buffer[(int)user]), '\n');
}

void header2Block_keep(void *user, char *block, fqbytecount block_n, char final){
    fqbuffer_append(&(read_buffer[(int)user]), block, block_n);
    if(final == 1) fqbuffer_appendchar(&(read_buffer[(int)user]), '\n');
}

void header2Block_discard(void *user, char *block, fqbytecount block_n, char final){
    if(final == 1) fqbuffer_appendchar(&(read_buffer[(int)user]), '\n');
}

void sequenceBlock(void *user, char *block, fqbytecount block_n, char final){
    fqbuffer_append(&(read_buffer[(int)user]), block, block_n);
    if(final == 1){
        fqbuffer_appendchar(&(read_buffer[(int)user]), '\n');
        fqbuffer_appendchar(&(read_buffer[(int)user]), '+');
    } 
}

void qualityBlock(void *user, char *block, fqbytecount block_n, char final){
    fqbuffer_append(&(read_buffer[(int)user]), block, block_n);
    if(final == 1) fqbuffer_appendchar(&(read_buffer[(int)user]), '\n');
}

int fqprocess_view(int argc, const char *argv[], fqglobal options){
    int option;
    char keep_headers = 0;
    char output_specified = 0;
    fqstatus result;
    char finished = 0;
    
    //Parse the subcommand options:
    options.file_output_stem = "output%";
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+hko:")) != -1){
        switch(option){
            case 'h':{fqprocess_view_help(); return FQ_STATUS_OK;}
            case 'k':{keep_headers = 1; break;}
            case 'o':{output_specified = 1; options.file_output_stem = optarg; break;}
            default:{fqprocess_view_usage(); return FQ_STATUS_FAIL;}
        }
    }
    
    // Work out how many input and output files we'll be writing:
    char input_n = (argc - optind);
    char input_paired, output_paired;
    if ((input_n != 2) && (options.input_interleaving == FQ_FILESET_NONINTERLEAVED)) input_paired = FQ_FILESET_UNPAIRED;
    else input_paired = FQ_FILESET_PAIRED;
    if ((input_paired == FQ_FILESET_UNPAIRED) || (options.output_interleaving == FQ_FILESET_INTERLEAVED)) output_paired = FQ_FILESET_UNPAIRED;
    else output_paired = FQ_FILESET_PAIRED;
    
    //Prepare the input file set:
    result = fqinset_prepare(&f_in, input_paired, argv, argc, &callbacks, options);
    if(result != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: failed to initialize input\n");
        return FQ_STATUS_FAIL;
    }
    
    //Set default output format based on calculated input format:
    if(options.output_format == FQ_FORMAT_UNKNOWN) options.output_format = f_in.format; // Guess by input format
    if(options.output_format == FQ_FORMAT_UNKNOWN) options.output_format = FQ_FORMAT_FASTQ_GZ; // Default to FASTQ.GZ

    //Prepare the output file set:
    result = fqoutset_prepare(&f_out, output_paired, output_specified, options);
    if(result != FQ_STATUS_OK){
        fqinset_close(&f_in);
        fprintf(stderr, "ERROR: failed to initialize output\n");
        return FQ_STATUS_FAIL;
    }

    //Initialize the output read buffers:
    fqbuffer_init(&(read_buffer[0]), 250);
    if(input_paired == FQ_FILESET_PAIRED) fqbuffer_init(&(read_buffer[1]), 250);
    
    //Set the callbacks:
    set_generic_callbacks(&callbacks);
    callbacks.readBuffer = readBuffer;
    callbacks.startRead = startRead;
    callbacks.endRead = endRead;
    callbacks.header1Block = header1Block;
    callbacks.sequenceBlock = sequenceBlock;
    if(keep_headers == 1) callbacks.header2Block = header2Block_keep;
    else callbacks.header2Block = header2Block_discard;
    callbacks.qualityBlock = qualityBlock;

    // Step through the input fileset:
    do finished = fqinset_step(&f_in);
    while(finished != 1);

    // Clean up:
    fqinset_close(&f_in);
    fqoutset_close(&f_out);
    fqbuffer_free(&(read_buffer[0]));
    if(input_paired == FQ_FILESET_PAIRED) fqbuffer_free(&(read_buffer[1]));
    return FQ_STATUS_OK;
}
