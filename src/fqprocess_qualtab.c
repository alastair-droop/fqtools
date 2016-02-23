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
#include <math.h>

// Define the global variables:
fqfsin f_in;
fqparser_callbacks callbacks;
fqbytecount counts[94];

float fqcommand_qualtab_phred(char q){
	return(powf((float)10, -((float)q / (float)10)));
}

float fqcommand_qualtab_solexa(int q){
	return(powf((float)10, -(float)10 * log10f((powf((float)10, (float)q / (float)10) + (float)1)) / (float)(10)));
}

fqbytecount fqprocess_qualtab_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqprocess_qualtab_qualityBlock(fqflag pair, char *block, fqbytecount block_n, char final){
	fqbytecount i;
	for(i = 0; i < block_n; i++) counts[block[i] - 33] ++;
}

fqstatus fqprocess_qualtab(int argc, const char *argv[], fqglobal options){
    int i, option;
    fqstatus result;
    char finished = 0;

    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+hko:")) != -1){
        switch(option){
            case 'h':{fqprocess_qualtab_help(); return FQ_STATUS_OK;}
            case 'k':{options.keep_headers = 1; break;}
            case 'o':{options.output_filename_specified = 1; options.file_output_stem = optarg; break;}
            default:{fqprocess_qualtab_usage(); return FQ_STATUS_FAIL;}
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
    callbacks.readBuffer = fqprocess_qualtab_readBuffer;
    callbacks.qualityBlock = fqprocess_qualtab_qualityBlock;

    //Initialize the quality character list:
	for(i = 0; i < 94; i++) counts[i] = 0;
    
    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;
    
    //Print out the quality data:
    if(result == FQ_STATUS_OK){
		if(options.quality == FQ_QTYPE_SANGER) for(i = 0; i < 94; i ++) printf("%c\t%d\t%0.4f\t%zd\n", i + 33, i, fqcommand_qualtab_phred(i), counts[i]);
		else if(options.quality == FQ_QTYPE_SOLEXA) for(i = 26; i < 94; i ++) printf("%c\t%d\t%0.4f\t%zd\n", i + 33, i - 31, fqcommand_qualtab_solexa((int)i - 31), counts[i]);
		else if(options.quality == FQ_QTYPE_ILLUMINA) for(i = 31; i < 94; i ++) printf("%c\t%d\t%0.4f\t%zd\n", i + 33, i - 31, fqcommand_qualtab_phred((int)i - 31), counts[i]);
        else for(i = 0; i < 94; i ++) printf("%c\t%zd\n", i + 33, counts[i]); // FQ_QTYPE_UNKNOWN
	}
    
    // Clean up:
    fqfsin_close(&f_in);
    return result;
}
