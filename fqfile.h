#include <stdio.h>

// Define the file modes:
#define FQFILE_MODE_READ 0x01
#define FQFILE_MODE_WRITE 0x02

// Define the possible file types:
#define FQFILE_TYPE_FASTQ_UNCOMPRESSED 0x01

// Define the struct that will hold a single file:
typedef struct{
    FILE *handle;
    char mode;
    char type;
    void (*read_block)(char *buffer, size_t n);
} fq_file;

// Define the callback functions that read a buffer from a file:
void fqfile_readblock_fastq_uncompressed(char *buffer, size_t n);

// Initialise a file:
int fq_file_init(fq_file *f, const char *filename, char type, char mode);

// Close a file:
void fq_file_close(fq_file *f);
