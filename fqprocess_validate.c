// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "fqheader.h"

// Define the global variables:
fqfsin f_in;
fqparser_callbacks callbacks;
fqbuffer header1_1, header1_2, header2_1, header2_2;

fqbytecount fqprocess_validate_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqprocess_validate_endRead(fqflag pair){
    if(pair == 0){
		if(header2_1.data[0] != '\0'){
			if(strcmp(header1_1.data, header2_1.data) != 0){
				fprintf(stderr, "ERROR [line %ld]: read header mismatch\n", f_in.files[0]->parser.line_number - 1);
				f_in.files[0]->parser.error = 1;
                callbacks.interrupt = 1;
			}
		}
    	fqbuffer_reset(&header1_1);
    	fqbuffer_reset(&header2_1);        
    } else {
		if(header2_2.data[0] != '\0'){
			if(strcmp(header1_2.data, header2_2.data) != 0){
				fprintf(stderr, "ERROR [line %ld]: read header mismatch\n", f_in.files[1]->parser.line_number - 1);
				f_in.files[1]->parser.error = 1;
                callbacks.interrupt = 1;
			}
		}
    	fqbuffer_reset(&header1_2);
    	fqbuffer_reset(&header2_2);
    }
}

void fqprocess_validate_header1Block(fqflag pair, char *block, fqbytecount block_n, char final){
    if (pair == 0) fqbuffer_append(&header1_1, block, block_n);
    else fqbuffer_append(&header1_2, block, block_n);
}

void fqprocess_validate_header2Block(fqflag pair, char *block, fqbytecount block_n, char final){
    if (pair == 0) fqbuffer_append(&header2_1, block, block_n);
    else fqbuffer_append(&header2_2, block, block_n);
}

fqstatus fqprocess_validate(int argc, const char *argv[], fqglobal options){
    int option;
    fqstatus result;
    char finished = 0;

    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+h")) != -1){
        switch(option){
            case 'h':{fqprocess_validate_help(); return FQ_STATUS_OK;}
            default:{fqprocess_validate_usage(); return FQ_STATUS_FAIL;}
        }
    }

    //Prepare the IO file sets:
    result = prepare_filesets(&f_in, NULL, argc - optind, &(argv[optind]), &callbacks, options);
    if(result != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: failed to initialize IO\n");
        return FQ_STATUS_FAIL;
    }

    //Set the callbacks:
    set_generic_callbacks(&callbacks);
    callbacks.readBuffer = fqprocess_validate_readBuffer;
    callbacks.endRead = fqprocess_validate_endRead;
    callbacks.header1Block = fqprocess_validate_header1Block;
    callbacks.header2Block = fqprocess_validate_header2Block;

	// Create the header buffers:
	fqbuffer_init(&header1_1, 1024);
	fqbuffer_init(&header2_1, 1024);
	fqbuffer_init(&header1_2, 1024);
	fqbuffer_init(&header2_2, 1024);

    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;
    
    if(result == FQ_STATUS_OK) printf("OK\n");
    
    // Clean up:
	fqbuffer_free(&header1_1);
	fqbuffer_free(&header2_1);
	fqbuffer_free(&header1_2);
	fqbuffer_free(&header2_2);
    fqfsin_close(&f_in);
    return result;
}
