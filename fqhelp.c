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
    printf("    b  unaligned BAM format (.bam)\n");
    printf("    u  attempt to infer format from file extension, (default .fastq.gz)\n");
    printf("\n");
    printf("COMMAND:\n");
    printf("view      View FASTQ files\n");
    printf("count     Count FASTQ file reads\n");
    printf("fasta     Convert FASTQ files to FASTA format\n");
    printf("basetab   Tabulate FASTQ base frequencies\n");
    printf("type      Attempt to guess the FASTQ quality encoding type\n");    
}

void fqprocess_view_usage(){
    printf("usage: %s [...] view [-hk] [-o OUTFILE] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_count_usage(){
    printf("usage: %s [...] count [-h] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_fasta_usage(){
    printf("usage: %s [...] fasta [-h] [-l LENGTH] [-o OUTFILE] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_basetab_usage(){
    printf("usage: %s [...] basetab [-has] [FILE] [FILE]\n", PROG_NAME);
}

void fqprocess_type_usage(){
    printf("usage: %s [...] type [-h] [FILE]\n", PROG_NAME);
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
