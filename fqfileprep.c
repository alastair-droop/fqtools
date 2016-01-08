#include "fqheader.h"

fqstatus prepare_filesets(fqfsin *in, fqfsout *out, int n_infiles, const char *infiles[], fqparser_callbacks *callbacks, fqglobal options){
    fqstatus result;
    char *outfile_1 = NULL;
    char *outfile_2 = NULL;
    fqflag informat_1, informat_2;    
    fqflag outformat_1, outformat_2;
    int n_outfiles;
    //INPUT:
    if((n_infiles == 2) && (options.input_interleaving == FQ_INTERLEAVED)) options.input_interleaving = FQ_NONINTERLEAVED; // Ignore interleaving for 2 input files
    if(n_infiles == 0){
        informat_1 = options.input_format;
        if(informat_1 == FQ_FORMAT_UNKNOWN) informat_1 = options.default_input_format;
        result = fqfsin_open_single(in, NULL, informat_1, callbacks, options.input_bufsize, options.output_bufsize, options.sequence_flags, options.quality, options.input_interleaving);
        if(result != FQ_STATUS_OK) goto finish;
        n_outfiles = 1;
    } else{
        if(n_infiles == 1){
            informat_1 = options.input_format;
            if(informat_1 == FQ_FORMAT_UNKNOWN) informat_1 = guess_file_format((char*)infiles[0]);
            if(informat_1 == FQ_FORMAT_UNKNOWN) informat_1 = options.default_input_format;
            result = fqfsin_open_single(in, (char*)infiles[0], informat_1, callbacks, options.input_bufsize, options.output_bufsize, options.sequence_flags, options.quality, options.input_interleaving);
            if(result != FQ_STATUS_OK) goto finish;            
            n_outfiles = 1;
        } else {
            informat_1 = options.input_format;
            if(informat_1 == FQ_FORMAT_UNKNOWN) informat_1 = guess_file_format((char*)infiles[0]);
            if(informat_1 == FQ_FORMAT_UNKNOWN) informat_1 = options.default_input_format;
            informat_2 = options.input_format;
            if(informat_2 == FQ_FORMAT_UNKNOWN) informat_2 = guess_file_format((char*)infiles[1]);
            if(informat_2 == FQ_FORMAT_UNKNOWN) informat_2 = options.default_input_format;
            result = fqfsin_open_paired(in, (char*)infiles[0], (char*)infiles[1], informat_1, informat_2, callbacks, options.input_bufsize, options.output_bufsize, options.sequence_flags, options.quality);
            if(result != FQ_STATUS_OK) goto finish;
            n_outfiles = 2;
        }
    }
    if((n_outfiles == 1) && (options.input_interleaving == FQ_INTERLEAVED)) n_outfiles = 2;
    //OUTPUT:
    if((n_outfiles == 1) && (options.output_interleaving == FQ_INTERLEAVED)) options.output_interleaving = FQ_NONINTERLEAVED;
    if((n_outfiles == 2) && (options.output_interleaving == FQ_INTERLEAVED)) n_outfiles = 1;
    // 
    if((n_outfiles == 1) && (options.output_filename_specified == 0)){
        outformat_1 = options.output_format;
        if(outformat_1 == FQ_FORMAT_UNKNOWN) outformat_1 = informat_1;
        result = fqfsout_open_single(out, NULL, outformat_1, options.output_bufsize);
        if(result != FQ_STATUS_OK) {fqfsin_close(in); goto finish;}        
    }
    if((n_outfiles == 1) && (options.output_filename_specified == 1)){
        outformat_1 = options.output_format;
        if(outformat_1 == FQ_FORMAT_UNKNOWN) outformat_1 = informat_1;
        outfile_1 = generate_filename(options.file_output_stem, options.file_pair_replacement, 0, outformat_1);
        result = fqfsout_open_single(out, outfile_1, outformat_1, options.output_bufsize);
        if(result != FQ_STATUS_OK) {fqfsin_close(in); goto finish;}
    }
    if(n_outfiles == 2){
        outformat_1 = options.output_format;
        if(outformat_1 == FQ_FORMAT_UNKNOWN) outformat_1 = informat_1;
        outformat_2 = options.output_format;
        if(outformat_2 == FQ_FORMAT_UNKNOWN) outformat_2 = informat_2;
        outfile_1 = generate_filename(options.file_output_stem, options.file_pair_replacement, 1, outformat_1);
        outfile_2 = generate_filename(options.file_output_stem, options.file_pair_replacement, 2, outformat_2);
        result = fqfsout_open_paired(out, outfile_1, outfile_2, outformat_1, outformat_2, options.output_bufsize);
        if(result != FQ_STATUS_OK) {fqfsin_close(in); goto finish;}
    }
finish:
    if(outfile_1 != NULL) free(outfile_1);
    if(outfile_2 != NULL) free(outfile_2);
    return result;
}
