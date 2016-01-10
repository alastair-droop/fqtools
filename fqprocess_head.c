#include "fqheader.h"

// Define the global variables:
fqfsin f_in;
fqfsout f_out;
fqparser_callbacks callbacks;
char interleaving_out;
fqbytecount read_n;
fqbytecount max_reads;

fqbytecount fqprocess_head_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqprocess_head_startRead(fqflag pair){
    fqfsout_writechar(&f_out, pair, '@');
}

void fqprocess_head_endRead(fqflag pair){
    if(interleaving_out == FQ_INTERLEAVED) fqfsout_flush(&f_out, pair);
	read_n++;
	if(read_n >= max_reads) callbacks.interrupt = 1;
}

void fqprocess_head_header1Block(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void fqprocess_head_header2Block_keep(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void fqprocess_head_header2Block_discard(fqflag pair, char *block, fqbytecount block_n, char final){
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void fqprocess_head_sequenceBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1){
        fqfsout_writechar(&f_out, pair, '\n');
        fqfsout_writechar(&f_out, pair, '+');
    }
}

void fqprocess_head_qualityBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

fqstatus fqprocess_head(int argc, const char *argv[], fqglobal options){
    interleaving_out = options.output_interleaving;
    int option;
	char *convert_err;
    fqstatus result;
    char finished = 0;
	max_reads = 10;
	read_n = 0;
    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+hkn:o:")) != -1){
        switch(option){
            case 'h':{fqprocess_head_help(); return FQ_STATUS_OK;}
            case 'k':{options.keep_headers = 1; break;}
			case 'n':{
				max_reads = strtoul(optarg, &convert_err, 10);
				if(convert_err[0] != '\0'){
					fprintf(stderr, "ERROR: invalid maximum read count\n");
					return 1;
				}
				break;
            }
            case 'o':{options.output_filename_specified = 1; options.file_output_stem = optarg; break;}
            default:{fqprocess_head_usage(); return FQ_STATUS_FAIL;}
        }
    }

    //Prepare the IO file sets:
    result = prepare_filesets(&f_in, &f_out, argc - optind, &(argv[optind]), &callbacks, options);
    if(result != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: failed to initialize IO\n");
        return FQ_STATUS_FAIL;
    }

    //Set the callbacks:
    set_generic_callbacks(&callbacks);
    callbacks.readBuffer = fqprocess_head_readBuffer;
    callbacks.startRead = fqprocess_head_startRead;
    callbacks.endRead = fqprocess_head_endRead;
    callbacks.header1Block = fqprocess_head_header1Block;
    callbacks.sequenceBlock = fqprocess_head_sequenceBlock;
    if(options.keep_headers == 1) callbacks.header2Block = fqprocess_head_header2Block_keep;
    else callbacks.header2Block = fqprocess_head_header2Block_discard;
    callbacks.qualityBlock = fqprocess_head_qualityBlock;

    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;
    
    // Clean up:
    fqfsin_close(&f_in);
    fqfsout_close(&f_out);
    return result;
}
