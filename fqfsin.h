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

typedef struct {
    fqfile file;
    fqparser parser;
} fqpfile;

fqstatus fqpfile_open(fqpfile *f, const char *filename, fqflag format, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding, fqflag interleaved);
void fqpfile_close(fqpfile *f);

typedef struct{
    char n_files;
    fqflag interleaved;
    fqflag status;
    fqpfile *files[2];
} fqfsin;

fqstatus fqfsin_open_single(fqfsin *f, const char *filename, fqflag format, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding, fqflag interleaved);
fqstatus fqfsin_open_paired(fqfsin *f, const char *filename_1, const char *filename_2, fqflag format_1, fqflag format_2, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding);

char fqfsin_step(fqfsin *f);
void fqfsin_close(fqfsin *f);
