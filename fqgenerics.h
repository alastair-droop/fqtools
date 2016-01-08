#include "fqheader.h"

void fq_generic_read(int user);
void fq_generic_block(int user, char *block, fqbytecount block_n, char final);
void fq_generic_error(int user, char error_type, size_t line, char character);
void set_generic_callbacks(fqparser_callbacks *callbacks);
