#include "fqheader.h"

void fq_generic_read(fqflag pair);
void fq_generic_block(fqflag pair, char *block, fqbytecount block_n, char final);
void fq_generic_error(fqflag pair, char error_type, size_t line, char character);
void set_generic_callbacks(fqparser_callbacks *callbacks);
