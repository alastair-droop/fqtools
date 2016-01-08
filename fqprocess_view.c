#include "fqheader.h"

// Define the global variables:
fqfsin f_in;
fqfsout f_out;
fqparser_callbacks callbacks;
char interleaving_out;

fqbytecount readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void startRead(fqflag pair){
    fqfsout_writechar(&f_out, pair, '@');
}

void endRead(fqflag pair){
    if(interleaving_out == FQ_INTERLEAVED) fqfsout_flush(&f_out, pair);
}

void header1Block(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void header2Block_keep(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void header2Block_discard(fqflag pair, char *block, fqbytecount block_n, char final){
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void sequenceBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1){
        fqfsout_writechar(&f_out, pair, '\n');
        fqfsout_writechar(&f_out, pair, '+');
    }
}

void qualityBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

int fqprocess_view(int argc, const char *argv[], fqglobal options){
    interleaving_out = options.output_interleaving;
    int option;
    fqstatus result;
    char finished = 0;
    //
    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+hko:")) != -1){
        switch(option){
            case 'h':{fqprocess_view_help(); return FQ_STATUS_OK;}
            case 'k':{options.keep_headers = 1; break;}
            case 'o':{options.output_filename_specified = 1; options.file_output_stem = optarg; break;}
            default:{fqprocess_view_usage(); return FQ_STATUS_FAIL;}
        }
    }

    result = prepare_filesets(&f_in, &f_out, argc - optind, &(argv[optind]), &callbacks, options);
    if(result != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: failed to initialize IO\n");
        return FQ_STATUS_FAIL;
    }

    //Set the callbacks:
    set_generic_callbacks(&callbacks);
    callbacks.readBuffer = readBuffer;
    callbacks.startRead = startRead;
    callbacks.endRead = endRead;
    callbacks.header1Block = header1Block;
    callbacks.sequenceBlock = sequenceBlock;
    if(options.keep_headers == 1) callbacks.header2Block = header2Block_keep;
    else callbacks.header2Block = header2Block_discard;
    callbacks.qualityBlock = qualityBlock;

    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);

    // // Clean up:
    fqfsin_close(&f_in);
    fqfsout_close(&f_out);
    return FQ_STATUS_OK;
}
