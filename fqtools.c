#include "fqheader.h"

fqbytecount buffer_size(char *str, fqbytecount max){
    fqbytecount size, multiplier, output;
    char *error;
    int n = strlen(str) - 1;
    switch(str[n]){
        case 'b': case 'B': {multiplier = 1; str[n] = '\0'; break;}
        case 'k': case 'K': {multiplier = 1024; str[n] = '\0'; break;}
        case 'm': case 'M': {multiplier = 1048576; str[n] = '\0'; break;}
        case 'g': case 'G': {multiplier = 1073741824L; str[n] = '\0'; break;}
        default: multiplier = 1;
    }
    size = (fqbytecount)strtoul(optarg, &error, 10);
    if(error[0] != '\0'){
        fprintf(stderr, "ERROR: invalid buffer size: \"%s\"\n", str);
        exit(FQ_STATUS_FAIL);
    }
    output = size * multiplier;
    if(output > max){
        fprintf(stderr, "ERROR: requested buffer size (%lu bytes) exceeds the maximum buffer size\n", output);
        exit(FQ_STATUS_FAIL);
    }
    return output;
}

fqflag format_type(char f){
    switch(f){
        case 'F': return FQ_FORMAT_FASTQ;
        case 'f': return FQ_FORMAT_FASTQ_GZ;
        case 'b': return FQ_FORMAT_BAM;
        case 'u': default: return FQ_FORMAT_UNKNOWN;
    }
}

fqflag quality_type(char q){
    switch(q){
        case 's': case 'S': return FQ_QTYPE_SANGER;
        case 'o': case 'O': return FQ_QTYPE_SOLEXA;
        case 'i': case 'I': return FQ_QTYPE_ILLUMINA;
        case 'u': case 'U': default: return FQ_QTYPE_UNKNOWN;
    }
}

int main(int argc, const char *argv[]){
    int option;
    char sequence_specified = 0;
    fqbytecount input_bufsize = 1048576;
    fqbytecount output_bufsize = 1048576;
    fqflag sequence_flags = 0;
    fqflag quality = FQ_QTYPE_UNKNOWN;
    fqflag input_format = FQ_FORMAT_FASTQ_GZ;
    fqflag output_format = FQ_FORMAT_FASTQ_GZ;
    fqflag interleaving = FQ_FILESET_NONINTERLEAVED;
    
    //Parse the global options:
    while((option = getopt(argc, (char* const*)argv, "+hvdramulb:B:q:f:i")) != -1){
        switch(option){
            case 'h':{global_help(); return FQ_STATUS_OK;}
            case 'v':{global_version(); return FQ_STATUS_OK;}
            case 'd':{sequence_specified = 1; sequence_flags = sequence_flags | SEQ_DNA; break;}
            case 'r':{sequence_specified = 1; sequence_flags = sequence_flags | SEQ_RNA; break; }
            case 'a':{sequence_specified = 1; sequence_flags = sequence_flags | SEQ_AMBIGUOUS; break;}
            case 'm':{sequence_specified = 1; sequence_flags = sequence_flags | SEQ_MASK; break;}
            case 'u':{sequence_specified = 1; sequence_flags = sequence_flags | SEQ_UPPERCASE; break;}
            case 'l':{sequence_specified = 1; sequence_flags = sequence_flags | SEQ_LOWERCASE; break;}
            case 'b':{input_bufsize = buffer_size(optarg, 1073741824L); break;}
            case 'B':{output_bufsize = buffer_size(optarg, 1073741824L); break;}
            case 'f':{input_format = format_type(*optarg); break;}
            case 'F':{output_format = format_type(*optarg); break;}
            case 'q':{quality = quality_type(*optarg); break;}
            case 'i':{interleaving = FQ_FILESET_INTERLEAVED; break;}
            default:{global_usage(); exit(FQ_STATUS_FAIL);}
        }
    }
    
    //Set default sequence flags:
    if(sequence_specified == 0) sequence_flags = SEQ_DNA | SEQ_UPPERCASE;
    if(((sequence_flags & SEQ_UPPERCASE) == 0) && ((sequence_flags & SEQ_LOWERCASE) == 0)) sequence_flags = sequence_flags | SEQ_UPPERCASE; // If no case if given, add UPPERCASE
    
    printf("input buffer size is %lu bytes\n", input_bufsize);
    printf("output buffer size is %lu bytes\n", output_bufsize);
    printf("quality type is %d\n", quality);
    printf("input format is %d\n", input_format);
    printf("output format is %d\n", output_format);
    printf("interleaving is %d\n", interleaving);
    
    return FQ_STATUS_OK;
    
    
//     fqstatus result;
//     char finished;
//     fqbuffer_init(&(read_buffer[0]), 10);
//     fqbuffer_init(&(read_buffer[1]), 10);
//     // Set up the callbacks:
//     set_generic_callbacks(&callbacks);
//     callbacks.readBuffer = readBuffer;
//     callbacks.startRead = startRead;
//     callbacks.endRead = endRead;
//     callbacks.header1Block = header1Block;
//     callbacks.sequenceBlock = sequenceBlock;
//     callbacks.header2Block = header2Block;
//     callbacks.qualityBlock = qualityBlock;
//
//     result = fqinset_open_paired(&fs_in, "./fastq/D0000XXXX-01-1.fastq.gz", "./fastq/D0000XXXX-01-2.fastq.gz", FQ_FORMAT_FASTQ_GZ, &callbacks, 10, 10, SEQ_UPPERCASE|SEQ_DNA, FQ_QTYPE_UNKNOWN);
//     // result = fqinset_open_single(&fs_in, "./fastq/D0000XXXX-01-1.fastq.gz", FQ_FORMAT_FASTQ_GZ, FQ_FILESET_NONINTERLEAVED, &callbacks, 10, 10, SEQ_UPPERCASE|SEQ_DNA, FQ_QTYPE_UNKNOWN);
//     if(result != FQ_STATUS_OK){
//         fprintf(stderr, "ERROR: Failed to open input file.\n");
//         return 1;
//     }
//
//     result = fqoutset_open_paired(&fs_out, "./out_1.fastq", "./out_2.fastq", FQ_FORMAT_FASTQ);
//     // result = fqoutset_open_single(&fs_out, "./out_1.fastq", FQ_FORMAT_FASTQ, FQ_FILESET_INTERLEAVED);
//     if(result != FQ_STATUS_OK){
//         fprintf(stderr, "ERROR: Failed to open output file.\n");
//         fqinset_close(&fs_in);
//         return 1;
//     }
//
//     // Step through the input fileset:
//     do finished = fqinset_step(&fs_in);
//     while(finished != 1);
//
//     //Done:
//     fqinset_close(&fs_in);
//     fqoutset_close(&fs_out);
//     fqbuffer_free(&(read_buffer[0]));
//     fqbuffer_free(&(read_buffer[1]));
//     return 0;
}
