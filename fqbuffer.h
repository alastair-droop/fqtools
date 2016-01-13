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
    char *data;
    fqbytecount size;
    fqbytecount offset;
} fqbuffer;

fqstatus fqbuffer_init(fqbuffer *b, fqbytecount size);
void fqbuffer_free(fqbuffer *b);

void fqbuffer_reset(fqbuffer *b);
fqbytecount fqbuffer_remaining(fqbuffer *b);
fqstatus fqbuffer_extend(fqbuffer *b, fqbytecount extra);

fqstatus fqbuffer_append(fqbuffer *b, char *data, fqbytecount size);
fqstatus fqbuffer_appendchar(fqbuffer *b, char c);

fqstatus fqbuffer_appendchar_noextend(fqbuffer *b, char c);
