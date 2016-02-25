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
char paired;

fqbytecount fqprocess_blockview_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqcommand_blockview_block(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1){
        if((pair == 0) && (paired == 1)) fqfsout_writechar(&f_out, pair, '\t');
        else fqfsout_writechar(&f_out, pair, '\n');
    }
}

void fqprocess_blockview_endRead(fqflag pair){
    fqfsout_flush(&f_out, pair);
}

fqstatus fqprocess_blockview(int argc, const char *argv[], fqglobal options, char blockview){
    int option;
    fqstatus result;
    char finished = 0;
    char *command_str;

    // Sort out the command string based on the blockview value:
	switch(blockview){
		default: case 0:{command_str = "header"; break;}
		case 1:{command_str = "sequence"; break;}
		case 2:{command_str = "quality"; break;}
        case 3:{command_str = "secondary header"; break;}
	}
    
    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+ho:")) != -1){
        switch(option){
            case 'h':{fqprocess_blockview_help(command_str); return FQ_STATUS_OK;}
            case 'o':{options.output_filename_specified = 1; options.file_output_stem = optarg; break;}
            default:{fqprocess_blockview_usage(command_str); return FQ_STATUS_FAIL;}
        }
    }
    
    //Prepare the IO file sets:
    result = prepare_filesets(&f_in, NULL, argc - optind, &(argv[optind]), &callbacks, options);
    if(result != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: failed to initialize IO\n");
        return FQ_STATUS_FAIL;
    }
    result = fqfsout_open_single(&f_out, NULL, FQ_NONINTERLEAVED, FQ_FORMAT_FASTQ, options.output_bufsize);
    if(result != FQ_STATUS_OK){
        fprintf(stderr, "ERROR: failed to initialize IO\n");
        return FQ_STATUS_FAIL;
    }
    if((f_in.n_files == 2) || (f_in.interleaved == FQ_INTERLEAVED)) paired = 1;
    else paired = 0;

    //Set the callbacks:
    set_generic_callbacks(&callbacks);
    callbacks.readBuffer = fqprocess_blockview_readBuffer;
    callbacks.endRead = fqprocess_blockview_endRead;
	switch(blockview){
		case 0:{callbacks.header1Block = fqcommand_blockview_block; break;}
		case 1:{callbacks.sequenceBlock = fqcommand_blockview_block; break;}
		case 2:{callbacks.qualityBlock = fqcommand_blockview_block; break;}
		case 3:{callbacks.header2Block = fqcommand_blockview_block; break;}
	}
        
    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;
    
    // Clean up:
    fqfsin_close(&f_in);
    fqfsout_close(&f_out);
    return result;
}
