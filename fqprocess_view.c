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

fqbytecount fqprocess_view_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqprocess_view_startRead(fqflag pair){
    fqfsout_writechar(&f_out, pair, '@');
}

void fqprocess_view_endRead(fqflag pair){
    if(interleaving_out == FQ_INTERLEAVED) fqfsout_flush(&f_out, pair);
}

void fqprocess_view_header1Block(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void fqprocess_view_header2Block_keep(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void fqprocess_view_header2Block_discard(fqflag pair, char *block, fqbytecount block_n, char final){
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

void fqprocess_view_sequenceBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1){
        fqfsout_writechar(&f_out, pair, '\n');
        fqfsout_writechar(&f_out, pair, '+');
    }
}

void fqprocess_view_qualityBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    fqfsout_write(&f_out, pair, block, block_n);
    if(final == 1) fqfsout_writechar(&f_out, pair, '\n');
}

fqstatus fqprocess_view(int argc, const char *argv[], fqglobal options){
    interleaving_out = options.output_interleaving;
    int option;
    fqstatus result;
    char finished = 0;

    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+hko:")) != -1){
        switch(option){
            case 'h':{fqprocess_view_help(); return FQ_STATUS_OK;}
            case 'k':{options.keep_headers = 1; break;}
            case 'o':{options.output_filename_specified = 1; options.file_output_stem = optarg; break;}
            default:{fqprocess_view_usage(); return FQ_STATUS_FAIL;}
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
    callbacks.readBuffer = fqprocess_view_readBuffer;
    callbacks.startRead = fqprocess_view_startRead;
    callbacks.endRead = fqprocess_view_endRead;
    callbacks.header1Block = fqprocess_view_header1Block;
    callbacks.sequenceBlock = fqprocess_view_sequenceBlock;
    if(options.keep_headers == 1) callbacks.header2Block = fqprocess_view_header2Block_keep;
    else callbacks.header2Block = fqprocess_view_header2Block_discard;
    callbacks.qualityBlock = fqprocess_view_qualityBlock;

    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;
    
    // Clean up:
    fqfsin_close(&f_in);
    fqfsout_close(&f_out);
    return result;
}
