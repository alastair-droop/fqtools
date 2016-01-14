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

#define LENGTH_BUFFER_SIZE 4096

// Define the global variables:
fqfsin f_in;
fqparser_callbacks callbacks;
fqbytecount current_read_length, read_count, length_buffer_increment, length_buffer_size;
fqbytecount *length_buffer;

int compare_lengths(const void *lhs, const void  *rhs){
	if(*(const int *)lhs > *(const int *)rhs) return 1;
	if(*(const int *)lhs < *(const int *)rhs) return -1;
	return 0;
}

fqbytecount fqprocess_lengthtab_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqprocess_lengthtab_endRead(fqflag pair){
    length_buffer[read_count] = current_read_length;
    current_read_length = 0;
    read_count++;
    if(read_count >= length_buffer_size){
        length_buffer_size += LENGTH_BUFFER_SIZE;
        length_buffer = (size_t*)realloc(length_buffer, sizeof(fqbytecount) * length_buffer_size);
        if(length_buffer == NULL){
                fqfsin_close(&f_in);
            fprintf(stderr, "ERROR: Failed to re-allocate read length buffer.");
            exit(FQ_STATUS_FAIL);
        }
    }
}

void fqprocess_lengthtab_sequenceBlock(fqflag pair, char *block, fqbytecount block_n, char final){
	current_read_length += block_n;
}

fqstatus fqprocess_lengthtab(int argc, const char *argv[], fqglobal options){
    options.single_input = 1;
    int option;
    fqstatus result;
    char finished = 0;
    fqbytecount i, current_value, count;

    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+h")) != -1){
        switch(option){
            case 'h':{fqprocess_lengthtab_help(); return FQ_STATUS_OK;}
            default:{fqprocess_lengthtab_usage(); return FQ_STATUS_FAIL;}
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
    callbacks.readBuffer = fqprocess_lengthtab_readBuffer;
    callbacks.endRead = fqprocess_lengthtab_endRead;
    callbacks.sequenceBlock = fqprocess_lengthtab_sequenceBlock;

	//  Set up the length buffer:
	current_read_length = 0;
	read_count = 0;
	length_buffer_increment = LENGTH_BUFFER_SIZE;
	length_buffer_size = LENGTH_BUFFER_SIZE;
	length_buffer = (fqbytecount*)malloc(length_buffer_size * sizeof(fqbytecount));
	if(length_buffer == NULL){
		fprintf(stderr, "ERROR: Failed to allocate read length buffer.");
        fqfsin_close(&f_in);
		return FQ_STATUS_FAIL;
	}

    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;
    
    // Process the resulting length data:
	if(result == FQ_STATUS_OK){
		qsort(length_buffer, read_count, sizeof(fqbytecount), compare_lengths);
		current_value = length_buffer[0];
		count = 0;
		for(i = 0; i < read_count; i++){
			if(length_buffer[i] != current_value){
				printf("%zd\t%zd\n", current_value, count);
				current_value = length_buffer[i];
				count = 0;
			}
			count++;
		}
		printf("%zd\t%zd\n", current_value, count);
    }
    
    // Clean up:
    fqfsin_close(&f_in);
	free(length_buffer);
    return result;
}
