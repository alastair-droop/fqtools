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
void global_version();
void global_usage();
void global_help();

// Subcommand messages:
void fqprocess_view_usage();
void fqprocess_head_usage();
void fqprocess_count_usage();
void fqprocess_blockview_usage(char *command_str);
void fqprocess_fasta_usage();
void fqprocess_basetab_usage();
void fqprocess_qualtab_usage();
void fqprocess_lengthtab_usage();
void fqprocess_type_usage();
void fqprocess_validate_usage();
void fqprocess_find_usage();
void fqprocess_trim_usage();
void fqprocess_qualmap_usage();
void fqprocess_tabseq_usage();

void fqprocess_view_help();
void fqprocess_head_help();
void fqprocess_count_help();
void fqprocess_blockview_help(char *command_str);
void fqprocess_fasta_help();
void fqprocess_basetab_help();
void fqprocess_qualtab_help();
void fqprocess_lengthtab_help();
void fqprocess_type_help();
void fqprocess_validate_help();
void fqprocess_find_help();
void fqprocess_trim_help();
void fqprocess_qualmap_help();
void fqprocess_tabseq_help();
