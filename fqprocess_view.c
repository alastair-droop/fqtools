#include "fqheader.h"

// Define the global variables:
fqfsin f_in;
fqfsout f_out;
fqparser_callbacks callbacks;
char interleaving;

fqbytecount readBuffer(void *user, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[(int)user]->file), b, b_size);
}

void startRead(void *user){
    fqfsout_writechar(&f_out, (int)user, '@');
}

void endRead(void *user){
    if(interleaving == 1)fqfsout_flush(&f_out, (int)user);
}

void header1Block(void *user, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, (int)user, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, (int)user, '\n');
}

void header2Block_keep(void *user, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, (int)user, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, (int)user, '\n');
}

void header2Block_discard(void *user, char *block, fqbytecount block_n, char final){
    if(final == 1) fqfsout_writechar(&f_out, (int)user, '\n');
}

void sequenceBlock(void *user, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, (int)user, block, block_n);
    if(final == 1){
        fqfsout_writechar(&f_out, (int)user, '\n');
        fqfsout_writechar(&f_out, (int)user, '+');
    }
}

void qualityBlock(void *user, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, (int)user, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, (int)user, '\n');
}

int fqprocess_view(int argc, const char *argv[], fqglobal options){
    interleaving = options.output_interleaving;
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
    
    // Prepare the input file set:
    result = fqfsin_prepare(&f_in, argc - optind, &(argv[optind]), &callbacks, options);
    if(result != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: failed to initialize input\n");
        return FQ_STATUS_FAIL;
    }
    
    // Prepare the output file set:
    result = fqfsout_prepare(&f_out, f_in.n_files, output_specified, options, f_in.files[0]->file.format, f_in.files[1]->file.format);
    if(result != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: failed to initialize output\n");
        fqfsin_close(&f_in);
        return FQ_STATUS_FAIL;
    }

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
    do finished = fqfsin_step(&f_in);
    while(finished != 1);

    // Clean up:
    fqfsin_close(&f_in);
    fqfsout_close(&f_out);
    return FQ_STATUS_OK;
}
