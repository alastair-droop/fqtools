#include "fqheader.h"

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
    // fqbuffer buffer;
    // int (*read_buf)(void *f);
} fqfile;

// // Define the callback functions that read a buffer from a file:
// int fqfile_readblock_fastq_uncompressed(void *f);
//
// // Initialise a file:
int fqfile_init(fqfile f, const char *filename, char type, char mode, size_t buffer_size);

// Close a file:
void fqfile_close(fqfile f);
//
// // Read the file into data buffer:
// int fqfile_readbuf(fqfile *f);
