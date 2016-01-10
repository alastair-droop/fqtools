#include "fqheader.h"
#include <math.h>

// Define the global variables:
fqfsin f_in;
fqfsout f_out;
fqparser_callbacks callbacks;
unsigned long int offsets[94];

fqbytecount fqprocess_type_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqprocess_type_qualityBlock(fqflag pair, char *block, fqbytecount block_n, char final){
	fqbytecount i;
	int offset;
	for(i = 0; i < block_n; i++){
		offset = block[i] - 33;
		if(offset < 59) callbacks.interrupt = 1;
		offsets[offset] ++;
	}
}

fqstatus fqprocess_type(int argc, const char *argv[], fqglobal options){
    int i, option;
    fqstatus result;
    char finished = 0;
    char min_offset;
    options.single_input = 1;
    
    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+hko:")) != -1){
        switch(option){
            case 'h':{fqprocess_type_help(); return FQ_STATUS_OK;}
            case 'k':{options.keep_headers = 1; break;}
            case 'o':{options.output_filename_specified = 1; options.file_output_stem = optarg; break;}
            default:{fqprocess_type_usage(); return FQ_STATUS_FAIL;}
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
    callbacks.readBuffer = fqprocess_type_readBuffer;
    callbacks.qualityBlock = fqprocess_type_qualityBlock;

    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;
    
    //Call the file type based on the lowest offset:
    if(result == FQ_STATUS_OK){
    	//Find the lowest offset:
    	min_offset = 126;
    	for(i = 0; i < 94; i ++){
    		if(offsets[i] != 0){
    			min_offset = i + 33;
    			break;
    		}
    	}
        
    	//Based upon minimum offset, call the file type:
    	if(min_offset < 59) printf("fastq-sanger\n");
    	else if(min_offset < 64) printf("fastq-solexa\n");
    	else printf("fastq-illumina\n");
    }
    
    // Clean up:
    fqfsin_close(&f_in);
    fqfsout_close(&f_out);
    return result;
}
