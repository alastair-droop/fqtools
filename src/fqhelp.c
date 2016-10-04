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

// Global messages:
void global_version(){
    printf("%s %s (zlib %s; htslib %s)\n", PROG_NAME, FQTOOLS_VERSION, ZLIB_VERSION, hts_version());
}

void global_usage(){
    printf("usage: %s [-hvdramuli] [-b BUFSIZE] [-B BUFSIZE] [-q QUALTYPE] [-f FORMAT] [-F FORMAT] COMMAND [...] [FILE] [FILE]\n", PROG_NAME);
}

void global_help(){
    global_usage();
    printf("\n");
    printf("global options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  -v               Show the program version and exit.\n");
    printf("  -d               Allow DNA sequence bases       (ACGTN)\n");
    printf("  -r               Allow RNA sequence bases       (ACGUN)\n");
    printf("  -a               Allow ambiguous sequence bases (RYKMSWBDHV)\n");
    printf("  -m               Allow mask sequence base       (X)\n");
    printf("  -u               Allow uppercase sequence bases\n");
    printf("  -l               Allow lowercase sequence bases\n");
    printf("  -p CHR           Set the pair replacement character (default \"%%\")\n");    
    printf("  -b BUFSIZE       Set the input buffer size\n");
    printf("  -B BUFSIZE       Set the output buffer size\n");
    printf("  -q QUALTYPE      Set the quality score encoding\n");
    printf("  -f FORMAT        Set the input file format\n");
    printf("  -F FORMAT        Set the output file format\n");
    printf("  -i               Read interleaved input file pairs\n");
    printf("  -I               Write interleaved output file pairs\n");
    printf("\n");
    printf("CHR:\n");
    printf("    This character will be replaced by the pair value when writing paired files.\n");
    printf("\n");
    printf("BUFSIZE:\n");
    printf("    Possible suffixes are [bkMG]. If no suffix is given, value is in bytes.\n");
    printf("\n");
    printf("QUALTYPE:\n");
    printf("    u  Do not assume specifc quality score encoding\n");
    printf("    s  Interpret quality scores as Sanger encoded\n");
    printf("    o  Interpret quality scores as Solexa encoded\n");
    printf("    i  Interpret quality scores as Illumina encoded\n");
    printf("\n");
    printf("FORMAT:\n");
    printf("    F  uncompressed FASTQ format (.fastq)\n");
    printf("    f  compressed FASTQ format (.fastq.gz)\n");
    printf("    b  BAM format (.bam)\n");
    printf("    s  SAM format (.sam)\n");
    printf("    u  attempt to infer format from file extension, (default .fastq.gz)\n");
    printf("\n");
    printf("COMMAND:\n");
    printf("view      View FASTQ files\n");
    printf("head      View the first reads in FASTQ files\n");
    printf("count     Count FASTQ file reads\n");
    printf("header    View FASTQ file header data\n");
    printf("sequence  View FASTQ file sequence data\n");
    printf("quality   View FASTQ file quality data\n");
    printf("header2   View FASTQ file secondary header data\n");
    printf("fasta     Convert FASTQ files to FASTA format\n");
    printf("basetab   Tabulate FASTQ base frequencies\n");
    printf("qualtab   Tabulate FASTQ quality character frequencies\n");
    printf("lengthtab Tabulate FASTQ read lengths\n");
    printf("type      Attempt to guess the FASTQ quality encoding type\n");
    printf("validate  Validate FASTQ files\n");
    printf("find      Find FASTQ reads containing specific sequences\n");
    printf("trim      Trim reads in a FASTQ file\n");
    printf("qualmap   Translate quality values using a mapping file\n");
}

void fqprocess_view_usage(){
    printf("usage: %s [...] view [-hk] [-o OUTFILE] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_head_usage(){
    printf("usage: %s [...] head [-hk] [-n NUMBER] [-o OUTFILE] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_count_usage(){
    printf("usage: %s [...] count [-h] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_blockview_usage(char *command_str){
    printf("usage: %s [...] %s [-h] [FILE] [FILE]\n", PROG_NAME, command_str);
}

void fqprocess_fasta_usage(){
    printf("usage: %s [...] fasta [-h] [-l LENGTH] [-o OUTFILE] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_basetab_usage(){
    printf("usage: %s [...] basetab [-has] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_qualtab_usage(){
    printf("usage: %s [...] qualtab [-h] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_lengthtab_usage(){
    printf("usage: %s [...] lengthtab [-h] [FILE]\n", PROG_NAME);
}

void fqprocess_type_usage(){
    printf("usage: %s [...] type [-h] [FILE]\n", PROG_NAME);
}

void fqprocess_validate_usage(){
    printf("usage: %s [...] validate [-h] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_find_usage(){
    printf("usage: %s [...] find [-hka] [-o OUTFILE]  [-s SEQUENCE ...] [FILE]\n", PROG_NAME);
}

void fqprocess_trim_usage(){
    printf("usage: %s [...] trim [-hk] [-o OUTFILE] [-s LENGTH] [-l LENGTH] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_qualmap_usage(){
    printf("usage: %s [...] qualmap [-hk] [-o OUTFILE] MAP [FILE] [FILE]\n", PROG_NAME);
}


void fqprocess_view_help(){
    printf("View FASTQ files.\n");
    printf("\n");
    printf("view options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  -k               Preserve secondary headers (if present).\n");
    printf("  -o STEM          Output file stem (default \"output%%\").\n");
    printf("  FILE             The fastq file(s) to view.\n");
    printf("\n");
    printf("STEM:\n");
    printf("    The file stem to use for output files (without file extension). Any\n");
    printf("    instances of the single character specified using the -p global argument\n");
    printf("    will be replaced with the pair number, or removed for single output\n");
    printf("    files. If the -o option is not specified, single file output will\n");
    printf("    be written to stdout and paired file output to the default stem (output%%).\n");
    printf("\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
}

void fqprocess_head_help(){
    printf("View the first reads in FASTQ files.\n");
    printf("\n");
    printf("view options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  -k               Preserve secondary headers (if present).\n");
    printf("  -n NUMBER        The number of reads to display (default 10).\n");
    printf("  -o STEM          Output file stem (default \"output%%\").\n");
    printf("  FILE             The fastq file(s) to view.\n");
    printf("\n");
    printf("NUMBER:\n");
    printf("    The number of reads to return from the top of the file(s).\n");
    printf("\n");
    printf("STEM:\n");
    printf("    The file stem to use for output files (without file extension). Any\n");
    printf("    instances of the single character specified using the -p global argument\n");
    printf("    will be replaced with the pair number, or removed for single output\n");
    printf("    files. If the -o option is not specified, single file output will\n");
    printf("    be written to stdout and paired file output to the default stem (output%%).\n");
    printf("\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
}

void fqprocess_count_help(){
    printf("Count FASTQ file reads.\n");
    printf("\n");
    printf("count options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  FILE             The fastq file(s) to count.\n");
    printf("\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
}

void fqprocess_blockview_help(char *command_str){
    printf("View FASTQ file %s data.\n", command_str);
    printf("\n");
    printf("%s options:\n", command_str);
    printf("  -h               Show this help message and exit.\n");
    printf("  FILE             The fastq file(s) to view.\n");
    printf("\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
}

void fqprocess_fasta_help(){
    printf("Convert FASTQ files to FASTA format.\n");
    printf("\n");
    printf("fasta options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  -l LENGTH        Maximum number of sequence characters per line.\n");
    printf("  -o STEM          Output file stem (default \"output%%\").\n");
    printf("  FILE             The fastq file(s) to view.\n");
    printf("\n");
	printf("LENGTH:\n");
	printf("The maximum number of sequence characters per line. If not specified,\n");
	printf("no line wrapping is performed.\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
    printf("\n");
    printf("STEM:\n");
    printf("    The file stem to use for output files (without file extension). Any\n");
    printf("    instances of the single character specified using the -p global argument\n");
    printf("    will be replaced with the pair number, or removed for single output\n");
    printf("    files. If the -o option is not specified, single file output will\n");
    printf("    be written to stdout and paired file output to the default stem (output%%).\n");
    printf("\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
}

void fqprocess_basetab_help(){
    printf("Tabulate FASTQ base frequencies.\n");
    printf("\n");
    printf("basetab options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  -a               Show all valid base frequencies, even if zero.\n");
    printf("  -s               Sort returned base frequencies (otherwise alphabetical).\n");
    printf("  FILE             The fastq file(s) to count.\n");
    printf("\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
}

void fqprocess_qualtab_help(){
    printf("Tabulate FASTQ quality character frequencies.\n");
    printf("\n");
    printf("basetab options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  FILE             The fastq file(s) to count.\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
	printf("\n");
	printf("The output depends on the specified quality type (-q). If no quality\n");
	printf("type is specified, then the ASCII characters in the quality strings\n");
	printf("are tabulated for all possible characters (33-127). If a quality type\n");
	printf("is specified, the quality score and approximate probability of error\n");
	printf("are returned for only the valid characters.\n");
}

void fqprocess_lengthtab_help(){
    printf("Tabulate FASTQ read lengths.\n");
    printf("\n");
    printf("lengthtab options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  FILE             The fastq file(s) to count.\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
	printf("\n");
}

void fqprocess_type_help(){
    printf("Attempt to guess the FASTQ quality encoding type.\n");
    printf("\n");
    printf("type options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  FILE             The fastq file(s) to count.\n");
    printf("\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
    printf("\n");
    printf("The file type is guessed from the quality data as follows:\n");
    printf("  1) if MIN(offset) < 59 then the file is fastq-sanger encoded;\n");
    printf("  2) if 59 <= MIN(offset) < 64 then the file is fastq-solexa encoded;\n");
    printf("  3) otherwise, the file is fastq-illumina encoded.\n");
}

void fqprocess_validate_help(){
    printf("Validate FASTQ file.\n");
    printf("\n");
    printf("validate options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  FILE             The fastq file(s) to count.\n");
    printf("\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
}

void fqprocess_find_help(){
    printf("Find FASTQ reads containing specific sequences.\n");
    printf("\n");
    printf("view options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  -k               Preserve secondary headers (if present).\n");
    printf("  -a               Require all sequences for a match.\n");
    printf("  -s SEQUENCE      Sequence to match against.\n");
    printf("  -f SEQ_FILE      Read match sequences from file.\n");
    printf("  -o STEM          Output file stem (default \"output%%\").\n");
    printf("  FILE             The fastq file(s) to view.\n");
    printf("\n");
    printf("SEQUENCE:\n");
    printf("    An exact sequence to look for in the read. Multiple sequences are\n");
    printf("    permitted. If -a is specified, all specified sequences must be present\n");
    printf("    for a read to match, otherwise the presence of any one is sufficient.\n");
    printf("    If no sequences are specified, all reads are returned.\n");
    printf("\n");
    printf("SEQ_FILE:\n");
    printf("    If specified with -f, sequences are read one per line from file.\n");
    printf("    Empty lines are ignored.\n");
    printf("STEM:\n");
    printf("    The file stem to use for output files (without file extension). Any\n");
    printf("    instances of the single character specified using the -p global argument\n");
    printf("    will be replaced with the pair number, or removed for single output\n");
    printf("    files. If the -o option is not specified, single file output will\n");
    printf("    be written to stdout and paired file output to the default stem (output%%).\n");
    printf("\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
}

void fqprocess_trim_help(){
    printf("View FASTQ files.\n");
    printf("\n");
    printf("view options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  -k               Preserve secondary headers (if present).\n");
    printf("  -o STEM          Output file stem (default \"output%%\").\n");
    printf("  -s LENGTH        Trim LENGTH bases from the read start.\n");
    printf("  -l LENGTH        Trim the read to a maximum length of LENGTH.\n");
    printf("  FILE             The fastq file(s) to view.\n");
    printf("\n");
    printf("STEM:\n");
    printf("    The file stem to use for output files (without file extension). Any\n");
    printf("    instances of the single character specified using the -p global argument\n");
    printf("    will be replaced with the pair number, or removed for single output\n");
    printf("    files. If the -o option is not specified, single file output will\n");
    printf("    be written to stdout and paired file output to the default stem (output%%).\n");
    printf("\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
    printf("\n");
    printf("LENGTH:\n");
    printf("    If LENGTH is not specified, no trimming is performed.\n");
}

void fqprocess_qualmap_help(){
    printf("Translate quality values using a mapping file.\n");
    printf("\n");
    printf("qualmap options:\n");
    printf("  -h               Show this help message and exit.\n");
    printf("  -k               Preserve secondary headers (if present).\n");
    printf("  -o STEM          Output file stem (default \"output%%\").\n");
    printf("  MAP              The mapping file to use (see below)\n");
    printf("  FILE             The fastq file(s) to view.\n");
    printf("\n");
    printf("STEM:\n");
    printf("    The file stem to use for output files (without file extension). Any\n");
    printf("    instances of the single character specified using the -p global argument\n");
    printf("    will be replaced with the pair number, or removed for single output\n");
    printf("    files. If the -o option is not specified, single file output will\n");
    printf("    be written to stdout and paired file output to the default stem (output%%).\n");
    printf("\n");
    printf("MAP:\n");
    printf("    The base quality translation table. This should be a text file with a single\n");
    printf("    line containing 94 characters. These will correspond to the valid quality\n");
    printf("    characters from '!' to '~'. Any characters outside that range will be clipped\n");
    printf("\n");
    printf("FILE:\n");
    printf("    If no input file is specified, input will be read from stdin.\n");
}
