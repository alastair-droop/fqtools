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
        case 's': return FQ_FORMAT_BAM;
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
    fqglobal options;
    char *command;

    //Set the default global options:
    options.input_bufsize = 1048576;
    options.output_bufsize = 1048576;
    options.sequence_flags = 0;
    options.quality = FQ_QTYPE_UNKNOWN;
    options.single_input = 0;
    options.input_format = FQ_FORMAT_UNKNOWN;
    options.output_format = FQ_FORMAT_UNKNOWN;
    options.output_fasta = 0;
    options.default_input_format = FQ_FORMAT_FASTQ_GZ;
    options.default_output_format = FQ_FORMAT_FASTQ_GZ;
    options.input_interleaving = FQ_NONINTERLEAVED;
    options.output_interleaving = FQ_NONINTERLEAVED;
    options.file_output_stem = "output%";
    options.file_pair_replacement = '%';
    options.keep_headers = 0;
    options.output_filename_specified = 0;
    
    //Parse the global options:
    while((option = getopt(argc, (char* const*)argv, "+hvdramulp:b:B:q:F:f:iI")) != -1){
        switch(option){
            case 'h':{global_help(); return FQ_STATUS_OK;}
            case 'v':{global_version(); return FQ_STATUS_OK;}
            case 'd':{sequence_specified = 1; options.sequence_flags = options.sequence_flags | SEQ_DNA; break;}
            case 'r':{sequence_specified = 1; options.sequence_flags = options.sequence_flags | SEQ_RNA; break; }
            case 'a':{sequence_specified = 1; options.sequence_flags = options.sequence_flags | SEQ_AMBIGUOUS; break;}
            case 'm':{sequence_specified = 1; options.sequence_flags = options.sequence_flags | SEQ_MASK; break;}
            case 'u':{sequence_specified = 1; options.sequence_flags = options.sequence_flags | SEQ_UPPERCASE; break;}
            case 'l':{sequence_specified = 1; options.sequence_flags = options.sequence_flags | SEQ_LOWERCASE; break;}
            case 'p':{options.file_pair_replacement = optarg[0]; break;}
            case 'b':{options.input_bufsize = buffer_size(optarg, 1073741824L); break;}
            case 'B':{options.output_bufsize = buffer_size(optarg, 1073741824L); break;}
            case 'f':{options.input_format = format_type(*optarg); break;}
            case 'F':{options.output_format = format_type(*optarg); break;}
            case 'q':{options.quality = quality_type(*optarg); break;}
            case 'i':{options.input_interleaving = FQ_INTERLEAVED; break;}
            case 'I':{options.output_interleaving = FQ_INTERLEAVED; break;}
            default:{global_usage(); exit(FQ_STATUS_FAIL);}
        }
    }
    
    //Set default sequence flags:
    if(sequence_specified == 0) options.sequence_flags = SEQ_DNA | SEQ_UPPERCASE;
    if(((options.sequence_flags & SEQ_UPPERCASE) == 0) && ((options.sequence_flags & SEQ_LOWERCASE) == 0)) options.sequence_flags = options.sequence_flags | SEQ_UPPERCASE; // If no case if given, add UPPERCASE
    
    //Get the requested command:
    if(optind >= argc){
        global_usage();
        return FQ_STATUS_FAIL;
    }
    command = (char*)argv[optind];
    
    //Farm out the individual commands to their processor functions:
    if(strcmp(command, "view") == 0) return fqprocess_view(argc, argv, options);
    if(strcmp(command, "head") == 0) return fqprocess_head(argc, argv, options);
    if(strcmp(command, "count") == 0) return fqprocess_count(argc, argv, options);
	if(strcmp(command, "header") == 0) return fqprocess_blockview(argc, argv, options, 0);
	if(strcmp(command, "sequence") == 0) return fqprocess_blockview(argc, argv, options, 1);
	if(strcmp(command, "quality") == 0) return fqprocess_blockview(argc, argv, options, 2);
	if(strcmp(command, "header2") == 0) return fqprocess_blockview(argc, argv, options, 3);
    if(strcmp(command, "fasta") == 0) return fqprocess_fasta(argc, argv, options);
    if(strcmp(command, "basetab") == 0) return fqprocess_basetab(argc, argv, options);
    if(strcmp(command, "qualtab") == 0) return fqprocess_qualtab(argc, argv, options);
    if(strcmp(command, "lengthtab") == 0) return fqprocess_lengthtab(argc, argv, options);
    if(strcmp(command, "type") == 0) return fqprocess_type(argc, argv, options);
    if(strcmp(command, "validate") == 0) return fqprocess_validate(argc, argv, options);
    if(strcmp(command, "find") == 0) return fqprocess_find(argc, argv, options);
    if(strcmp(command, "trim") == 0) return fqprocess_trim(argc, argv, options);
    if(strcmp(command, "qualmap") == 0) return fqprocess_qualmap(argc, argv, options);
    if(strcmp(command, "crisprtab") == 0) return fqprocess_crisprtab(argc, argv, options);
    
    //If we get to here, the given command string was invalid:
    fprintf(stderr, "ERROR: unknown command: \"%s\"\n", command);
    global_usage();
    return FQ_STATUS_FAIL;
}
