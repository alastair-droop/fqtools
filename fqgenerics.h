#include "fqheader.h"

void fq_generic_read(void *user);
void fq_generic_block(void *user, char *block, fqbytecount block_n, char final);
void fq_generic_error(void *user, char error_type, size_t line, char character);
void set_generic_callbacks(fqparser_callbacks *callbacks);
