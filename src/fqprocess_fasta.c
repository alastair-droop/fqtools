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
fqfsout f_out;
fqparser_callbacks callbacks;
char interleaving_out;
fqbytecount line_length, current_line;

fqbytecount fqprocess_fasta_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqprocess_fasta_startRead(fqflag pair){
    fqfsout_writechar(&f_out, pair, '>');
}

void fqprocess_fasta_endRead(fqflag pair){
    if(interleaving_out == FQ_INTERLEAVED) fqfsout_flush(&f_out, pair);
}

void fqprocess_fasta_header1Block(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void fqprocess_fasta_sequenceBlock(fqflag pair, char *block, fqbytecount block_n, char final){
	fqbytecount line_remaining = line_length - current_line;
	if((line_length == 0) || line_remaining >= block_n){
        fqfsout_write(&f_out, pair, block, block_n);
		current_line += block_n;
		if(final == 1){
            fqfsout_writechar(&f_out, pair, '\n');
			current_line = 0;
		}
	} else {
		long int block_remaining = block_n;
		while(block_remaining > 0){
			char *new_string = strndup(block + (block_n - block_remaining), line_remaining);
            fqfsout_write(&f_out, pair, new_string, strlen(new_string));
            fqfsout_writechar(&f_out, pair, '\n');
			block_remaining -= line_remaining;
			free(new_string);
		}
	}
}

fqstatus fqprocess_fasta(int argc, const char *argv[], fqglobal options){
    interleaving_out = options.output_interleaving;
    options.output_fasta = 1;
    options.default_output_format = FQ_FORMAT_FASTA;
    int option;
    fqstatus result;
	char *convert_err = NULL;
    char finished = 0;

    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+hko:l:")) != -1){
        switch(option){
            case 'h':{fqprocess_fasta_help(); return FQ_STATUS_OK;}
            case 'k':{options.keep_headers = 1; break;}
			case 'l':{
				line_length = strtoul(optarg, &convert_err, 10);
				if(convert_err[0] != '\0'){
					fprintf(stderr, "ERROR: invalid line length\n");
					fqprocess_fasta_usage();
					return FQ_STATUS_FAIL;
				}
				break;
			} // end max line switch
            case 'o':{options.output_filename_specified = 1; options.file_output_stem = optarg; break;}
            default:{fqprocess_fasta_usage(); return FQ_STATUS_FAIL;}
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
    callbacks.readBuffer = fqprocess_fasta_readBuffer;
    callbacks.startRead = fqprocess_fasta_startRead;
    callbacks.endRead = fqprocess_fasta_endRead;
    callbacks.header1Block = fqprocess_fasta_header1Block;
    callbacks.sequenceBlock = fqprocess_fasta_sequenceBlock;

    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;
    
    // Clean up:
    fqfsin_close(&f_in);
    fqfsout_close(&f_out);
    return result;
}
