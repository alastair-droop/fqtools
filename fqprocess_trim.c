#include "fqheader.h"

// Define the global variables:
fqfsin f_in;
fqfsout f_out;
fqparser_callbacks callbacks;
char interleaving_out;

fqbytecount trim_start = 0;
fqbytecount length_out = 0;
fqbytecount line_pos = 0;


fqbytecount fqprocess_trim_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqprocess_trim_startRead(fqflag pair){
    fqfsout_writechar(&f_out, pair, '@');
}

void fqprocess_trim_endRead(fqflag pair){
    if(interleaving_out == FQ_INTERLEAVED) fqfsout_flush(&f_out, pair);
}

void fqprocess_trim_header1Block(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void fqprocess_trim_header2Block_keep(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void fqprocess_trim_header2Block_discard(fqflag pair, char *block, fqbytecount block_n, char final){
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void fqprocess_trim_sequenceBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    fqbytecount trim_from_front = 0;
    fqbytecount trim_from_end = 0;
    if(!(((line_pos + block_n) <= trim_start) || ((length_out != 0) && (line_pos >= (trim_start + length_out))))){
        if(line_pos < trim_start) trim_from_front = trim_start - line_pos;
        if((length_out != 0) && ((line_pos + block_n) > (trim_start + length_out))) trim_from_end = (line_pos + block_n) - (trim_start + length_out);
        fqfsout_write(&f_out, pair, block + trim_from_front, block_n - (trim_from_front + trim_from_end));
    }
    line_pos += block_n;
    if(final == 1){
        fqfsout_writechar(&f_out, pair, '\n');
        fqfsout_writechar(&f_out, pair, '+');
        line_pos = 0;
    }
}

void fqprocess_trim_qualityBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    fqbytecount trim_from_front = 0;
    fqbytecount trim_from_end = 0;
    if(!(((line_pos + block_n) <= trim_start) || ((length_out != 0) && (line_pos >= (trim_start + length_out))))){
        if(line_pos < trim_start) trim_from_front = trim_start - line_pos;
        if((length_out != 0) && ((line_pos + block_n) > (trim_start + length_out))) trim_from_end = (line_pos + block_n) - (trim_start + length_out);
        fqfsout_write(&f_out, pair, block + trim_from_front, block_n - (trim_from_front + trim_from_end));
    }
    line_pos += block_n;
    if(final == 1){
        fqfsout_writechar(&f_out, pair, '\n');
        line_pos = 0;
    }
}

fqstatus fqprocess_trim(int argc, const char *argv[], fqglobal options){
    interleaving_out = options.output_interleaving;
    int option;
    fqstatus result;
    char finished = 0;
	char *convert_err = NULL;

    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+hko:s:l:")) != -1){
        switch(option){
            case 'h':{fqprocess_trim_help(); return FQ_STATUS_OK;}
            case 'k':{options.keep_headers = 1; break;}
			case 's':{
				trim_start = strtoul(optarg, &convert_err, 10);
				if(convert_err[0] != '\0'){
					fprintf(stderr, "ERROR: invalid start trimming value\n");
					return FQ_STATUS_FAIL;
				}
				break;
			} // end trim start switch
			case 'l':{
				length_out = strtoul(optarg, &convert_err, 10);
				if(convert_err[0] != '\0'){
					fprintf(stderr, "ERROR: invalid read length value\n");
					return FQ_STATUS_FAIL;
				}
				break;
			} // end length out switch
            case 'o':{options.output_filename_specified = 1; options.file_output_stem = optarg; break;}
            default:{fqprocess_trim_usage(); return FQ_STATUS_FAIL;}
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
    callbacks.readBuffer = fqprocess_trim_readBuffer;
    callbacks.startRead = fqprocess_trim_startRead;
    callbacks.endRead = fqprocess_trim_endRead;
    callbacks.header1Block = fqprocess_trim_header1Block;
    callbacks.sequenceBlock = fqprocess_trim_sequenceBlock;
    if(options.keep_headers == 1) callbacks.header2Block = fqprocess_trim_header2Block_keep;
    else callbacks.header2Block = fqprocess_trim_header2Block_discard;
    callbacks.qualityBlock = fqprocess_trim_qualityBlock;

    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;
    
    // Clean up:
    fqfsin_close(&f_in);
    fqfsout_close(&f_out);
    return result;
}
