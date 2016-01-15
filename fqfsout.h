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

typedef struct{
    char n_files;
    fqflag interleaved;
    fqfile file[2];
    fqbuffer buffer[2];
    fqbytecount trigger[2];
} fqfsout;

fqstatus fqfsout_open_single(fqfsout *f, const char *filename, fqflag interleaved, fqflag format, fqbytecount b_size);
fqstatus fqfsout_open_paired(fqfsout *f, const char *filename_1, const char *filename_2, fqflag format_1, fqflag format_2, fqbytecount b_size);
fqstatus fqfsout_writechar(fqfsout *f, int file, char c);
fqstatus fqfsout_write(fqfsout *f, int file, char *buffer, fqbytecount buffer_n);
void fqfsout_flush(fqfsout *f, int file);
void fqfsout_close(fqfsout *f);
