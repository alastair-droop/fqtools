#include "fqheader.h"

// Define the global variables:
fqfsin f_in;
fqfsout f_out;
fqparser_callbacks callbacks;
char keep_headers;
char **subjects = NULL;
size_t subject_count = 0;
fqbuffer header1, sequence, header2, quality;

fqbytecount fqprocess_find_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqprocess_find_startRead(fqflag pair){
	fqbuffer_reset(&header1);
	fqbuffer_reset(&sequence);
	fqbuffer_reset(&header2);
    fqbuffer_reset(&quality);
}

void writeRead(fqflag pair){
    fqfsout_writechar(&f_out, pair, '@');
    fqfsout_write(&f_out, pair, header1.data, header1.offset);
    fqfsout_writechar(&f_out, pair, '\n');            
    fqfsout_write(&f_out, pair, sequence.data, sequence.offset);
    fqfsout_writechar(&f_out, pair, '\n');            
    fqfsout_writechar(&f_out, pair, '+');
    if(keep_headers == 1) fqfsout_write(&f_out, pair, header2.data, header2.offset);
    fqfsout_writechar(&f_out, pair, '\n');
    fqfsout_write(&f_out, pair, quality.data, quality.offset);
    fqfsout_writechar(&f_out, pair, '\n');
}

void fqprocess_find_endRead_none(fqflag pair){
    writeRead(pair);
}

void fqprocess_find_endRead_or(fqflag pair){
    fqbytecount i;
	for(i = 0; i < subject_count; i++){
		if(strstr(sequence.data, subjects[i]) != NULL) writeRead(pair);
	}    
}

void fqprocess_find_endRead_and(fqflag pair){
    fqbytecount i;
	for(i = 0; i < subject_count; i++){
		if(strstr(sequence.data, subjects[i]) == NULL) return;
	}
    writeRead(pair);
}

void fqprocess_find_header1Block(fqflag pair, char *block, fqbytecount block_n, char final){
    fqbuffer_append(&header1, block, block_n);
}

void fqprocess_find_header2Block_keep(fqflag pair, char *block, fqbytecount block_n, char final){
    fqbuffer_append(&header2, block, block_n);
}

void fqprocess_find_header2Block_discard(fqflag pair, char *block, fqbytecount block_n, char final){
}

void fqprocess_find_sequenceBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    fqbuffer_append(&sequence, block, block_n);
}

void fqprocess_find_qualityBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    fqbuffer_append(&quality, block, block_n);
}

fqstatus fqprocess_find(int argc, const char *argv[], fqglobal options){
    options.single_input = 1;
    int option;
    fqstatus result;
	char require_all = 0;
    char finished = 0;
    size_t i;

    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+hkas:o:")) != -1){
        switch(option){
            case 'h':{fqprocess_find_help(); return FQ_STATUS_OK;}
            case 'k':{options.keep_headers = 1; break;}
            case 'a':{require_all = 1; break;}
			case 's':{
                subjects = (char**)realloc(subjects, (subject_count + 1) * sizeof(char*));
                subjects[subject_count] = (char*)malloc(strlen(optarg) * sizeof(char));
                strcpy(subjects[subject_count], optarg);
                subject_count++;
				break;
			}
            case 'o':{options.output_filename_specified = 1; options.file_output_stem = optarg; break;}
            default:{fqprocess_find_usage(); return FQ_STATUS_FAIL;}
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
    callbacks.readBuffer = fqprocess_find_readBuffer;
    callbacks.startRead = fqprocess_find_startRead;
    if(subject_count == 0) callbacks.endRead = fqprocess_find_endRead_none;
    else{
        if(require_all == 1) callbacks.endRead = fqprocess_find_endRead_and;
        else callbacks.endRead = fqprocess_find_endRead_or;
    }
    callbacks.header1Block = fqprocess_find_header1Block;
    callbacks.sequenceBlock = fqprocess_find_sequenceBlock;
    if(options.keep_headers == 1) callbacks.header2Block = fqprocess_find_header2Block_keep;
    else callbacks.header2Block = fqprocess_find_header2Block_discard;
    callbacks.qualityBlock = fqprocess_find_qualityBlock;

    //Initialise the buffers:
	fqbuffer_init(&header1, 1024);
	fqbuffer_init(&sequence, 1024);
	fqbuffer_init(&header2, 1024);
    fqbuffer_init(&quality, 1024);
    
    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;
    
    // Clean up:
	for(i = 0; i < subject_count; i++) free(subjects[i]);
	free(subjects);
	fqbuffer_free(&header1);
	fqbuffer_free(&sequence);
	fqbuffer_free(&header2);
    fqbuffer_free(&quality);
    fqfsin_close(&f_in);
    fqfsout_close(&f_out);
    return result;
}
