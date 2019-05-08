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
char trim_header;
fqbuffer p1header, p1seq, p2seq;

fqbytecount fqprocess_tabseq_readBuffer(fqflag pair, char *b, fqbytecount b_size){
    return fqfile_read(&(f_in.files[pair]->file), b, b_size);
}

void fqprocess_tabseq_headerBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    if(pair == FQ_PAIR_1) fqbuffer_append(&p1header, block, block_n);
}

void fqprocess_tabseq_sequenceBlock(fqflag pair, char *block, fqbytecount block_n, char final){
    if(pair == FQ_PAIR_1) fqbuffer_append(&p1seq, block, block_n);
    else fqbuffer_append(&p2seq, block, block_n);
}

void fqprocess_tabseq_endRead(fqflag pair){
    int header_offset = p1header.offset;
    if((paired == 0) || (pair == FQ_PAIR_2)){
        if(trim_header == 1){
            if(p1header.offset > 2){
                if((strncmp(p1header.data + p1header.offset - 2, "/1", 2) == 0) || (strncmp(p1header.data + p1header.offset - 2, "/2", 2) == 0)) header_offset = header_offset - 2;
            }            
        }
        fwrite(p1header.data, sizeof(char), header_offset, stdout);
        fwrite("\t", sizeof(char), 1, stdout);
        fwrite(p1seq.data, sizeof(char), p1seq.offset, stdout);
        if(paired == 1){
            fwrite("\t", sizeof(char), 1, stdout);
            fwrite(p2seq.data, sizeof(char), p2seq.offset, stdout);            
        }
        fwrite("\n", sizeof(char), 1, stdout);
        fflush(stdout);
        fqbuffer_reset(&p1header);
        fqbuffer_reset(&p1seq);
        fqbuffer_reset(&p2seq);        
    }
}

fqstatus fqprocess_tabseq(int argc, const char *argv[], fqglobal options){
    int option;
    fqstatus result;
    char finished = 0;
    trim_header = 0;
    
    //Initialise the output buffers:
    result = fqbuffer_init(&p1header, options.output_bufsize);
    if(result != FQ_STATUS_OK) return FQ_STATUS_FAIL;
    result = fqbuffer_init(&p1seq, options.output_bufsize);
    if(result != FQ_STATUS_OK){
        fqbuffer_free(&p1header);
        return FQ_STATUS_FAIL;
    }
    result = fqbuffer_init(&p2seq, options.output_bufsize);
    if(result != FQ_STATUS_OK){
        fqbuffer_free(&p1header);
        fqbuffer_free(&p1seq);
        return FQ_STATUS_FAIL;
    }
    
    //Parse the subcommand options:
    optind++; // Skip the subcommand argument
    while((option = getopt(argc, (char* const*)argv, "+ht")) != -1){
        switch(option){
            case 'h':{fqprocess_tabseq_help(); return FQ_STATUS_OK;}
            case 't':{trim_header=1; break;}
            default:{fqprocess_tabseq_usage(); return FQ_STATUS_FAIL;}
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
    callbacks.readBuffer = fqprocess_tabseq_readBuffer;
    callbacks.endRead = fqprocess_tabseq_endRead;
	callbacks.header1Block = fqprocess_tabseq_headerBlock;
	callbacks.header2Block = fqprocess_tabseq_headerBlock;
	callbacks.sequenceBlock = fqprocess_tabseq_sequenceBlock;
        
    // Step through the input fileset:
    do finished = fqfsin_step(&f_in);
    while(finished != 1);
    result = f_in.status;
    
    // Clean up:
    fqfsin_close(&f_in);
    fqfsout_close(&f_out);
    fqbuffer_free(&p1header);
    fqbuffer_free(&p1seq);
    fqbuffer_free(&p2seq);
    return result;
}
