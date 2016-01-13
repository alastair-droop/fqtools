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

// Define the base count structure:
typedef struct {
	char base;
	fqbytecount count;
} base_count;

// Define the global variables:
fqfsin f_in;
fqparser_callbacks callbacks;
base_count counts[57];

//Function to compare two bases by count:
int compare_bases(const void *lhs, const void  *rhs){
	if(((base_count*)lhs)->count > ((base_count*)rhs)->count) return -1;
	if(((base_count*)lhs)->count < ((base_count*)rhs)->count) return 1;
	return 0;
}

fqbytecount fqprocess_basetab_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqprocess_basetab_sequenceBlock(fqflag pair, char *block, fqbytecount block_n, char final){
	fqbytecount i;
	for(i = 0; i < block_n; i++) counts[block[i] - 'A'].count ++;
}

fqstatus fqprocess_basetab(int argc, const char *argv[], fqglobal options){
    int option;
    fqstatus result;
    int i;
	char include_all = 0;
    char sort = 0;
    char finished = 0;

    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+has")) != -1){
        switch(option){
            case 'h':{fqprocess_basetab_help(); return FQ_STATUS_OK;}
            case 'a':{include_all = 1; break;}
            case 's':{sort = 1; break;}
            default: {fqprocess_basetab_usage(); return FQ_STATUS_FAIL;}
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
    callbacks.readBuffer = fqprocess_basetab_readBuffer;
    callbacks.sequenceBlock = fqprocess_basetab_sequenceBlock;
    
    //Initialize the count list:
    for(i = 0; i < 57; i++){
        if(f_in.files[0]->parser.valid_sequence_characters[i + 'A'] == 1) counts[i].base = '\0';
        else counts[i].base = i + 'A';
        counts[i].count = 0;
    }

    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;

    //Print the output:
    if(result == FQ_STATUS_OK){
		if(sort == 1) qsort(counts, 57, sizeof(base_count), compare_bases);
        //Print the base composition:
        for(i = 0; i < 57; i ++){
            if(counts[i].base == '\0') continue;
            if((include_all == 0) && (counts[i].count == 0)) continue;
            printf("%c\t%ld\n", counts[i].base, counts[i].count);
        }
    }
    
    // Clean up:
    fqfsin_close(&f_in);
    return result;
}
