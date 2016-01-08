#include "fqheader.h"

fqstatus fqparser_init(fqparser *p, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, fqflag seq_flags, fqflag encoding, fqflag interleaved, int user){
    p->input_buffer = (char*)malloc(in_bufsize * sizeof(char));
    if(p->input_buffer == NULL) return FQ_STATUS_FAIL;
    p->output_buffer = (char*)malloc((out_bufsize + 1) * sizeof(char));
    if(p->output_buffer == NULL){
        free(p->input_buffer);
        return FQ_STATUS_FAIL;
    }
    p->valid_sequence_characters = (char*)malloc(256 * sizeof(char));
    if(p->output_buffer == NULL){
        free(p->input_buffer);
        free(p->output_buffer);
        return FQ_STATUS_FAIL;
    }
    fqparser_setValidSequenceCharacters(p, seq_flags);
    p->valid_quality_characters = (char*)malloc(256 * sizeof(char));
    if(p->output_buffer == NULL){
        free(p->input_buffer);
        free(p->output_buffer);
        free(p->valid_sequence_characters);
        return FQ_STATUS_FAIL;
    }
    fqparser_setValidQualityCharacters(p, encoding);
    p->input_buffer_size = 0;
    p->input_buffer_max = in_bufsize;
    p->input_buffer_offset = 0;
    p->output_buffer_max = out_bufsize;
    p->output_buffer_offset = 0;
    p->output_buffer[p->output_buffer_max] = '\0';
    p->interleaved = interleaved;
    p->user = user;
    p->callbacks = callbacks;
    p->entry_point = FQ_PARSER_ENTRY_START;
    p->sequence_length = 0;
    p->quality_length = 0;
    p->line_number = 0;
    return FQ_STATUS_OK;
}

void fqparser_free(fqparser *p){
    free(p->input_buffer);
    free(p->output_buffer);
    free(p->valid_sequence_characters);
    free(p->valid_quality_characters);
    return;
}

void fqparser_setValidSequenceCharacters(fqparser *p, fqflag flags){
    memset(p->valid_sequence_characters, 1, 256);
	char dna = (flags & SEQ_DNA) != 0;
	char rna = (flags & SEQ_DNA) != 0;
	char ambiguous = (flags & SEQ_AMBIGUOUS) != 0;
	char mask = (flags & SEQ_MASK) != 0;
	char lowercase = (flags & SEQ_LOWERCASE) != 0;
	char uppercase = (flags & SEQ_UPPERCASE) != 0;
	if (dna == 1){
		if (uppercase == 1){
			p->valid_sequence_characters['A'] = 0;
			p->valid_sequence_characters['C'] = 0;
			p->valid_sequence_characters['G'] = 0;
			p->valid_sequence_characters['T'] = 0;
			p->valid_sequence_characters['N'] = 0;
		}
		if (lowercase == 1){
			p->valid_sequence_characters['a'] = 0;
			p->valid_sequence_characters['c'] = 0;
			p->valid_sequence_characters['g'] = 0;
			p->valid_sequence_characters['t'] = 0;
			p->valid_sequence_characters['n'] = 0;
		}
	}
	if (rna == 1){
		if (uppercase == 1){
			p->valid_sequence_characters['A'] = 0;
			p->valid_sequence_characters['C'] = 0;
			p->valid_sequence_characters['G'] = 0;
			p->valid_sequence_characters['U'] = 0;
			p->valid_sequence_characters['N'] = 0;
		}
		if (lowercase == 1){
			p->valid_sequence_characters['a'] = 0;
			p->valid_sequence_characters['c'] = 0;
			p->valid_sequence_characters['g'] = 0;
			p->valid_sequence_characters['u'] = 0;
			p->valid_sequence_characters['n'] = 0;
		}
	}
	if (ambiguous == 1){
		if (uppercase == 1){
			p->valid_sequence_characters['R'] = 0;
			p->valid_sequence_characters['Y'] = 0;
			p->valid_sequence_characters['K'] = 0;
			p->valid_sequence_characters['M'] = 0;
			p->valid_sequence_characters['S'] = 0;
			p->valid_sequence_characters['W'] = 0;
			p->valid_sequence_characters['B'] = 0;
			p->valid_sequence_characters['D'] = 0;
			p->valid_sequence_characters['H'] = 0;
			p->valid_sequence_characters['V'] = 0;
		}
		if (lowercase == 1){
			p->valid_sequence_characters['r'] = 0;
			p->valid_sequence_characters['y'] = 0;
			p->valid_sequence_characters['k'] = 0;
			p->valid_sequence_characters['m'] = 0;
			p->valid_sequence_characters['s'] = 0;
			p->valid_sequence_characters['w'] = 0;
			p->valid_sequence_characters['b'] = 0;
			p->valid_sequence_characters['d'] = 0;
			p->valid_sequence_characters['h'] = 0;
			p->valid_sequence_characters['v'] = 0;
		}
	}
	if (mask == 1){
		if (uppercase == 1) p->valid_sequence_characters['X'] = 0;
		if (lowercase == 1) p->valid_sequence_characters['x'] = 0;
	}
}

void fqparser_setValidQualityCharacters(fqparser *p, fqflag encoding){
    memset(p->valid_quality_characters, 1, 256);
    if(encoding == FQ_QTYPE_UNKNOWN) memset(p->valid_quality_characters + 33, 0, 94);
    if(encoding == FQ_QTYPE_SANGER) memset(p->valid_quality_characters + 33, 0, 94);
    if(encoding == FQ_QTYPE_SOLEXA) memset(p->valid_quality_characters + 59, 0, 68);
    if(encoding == FQ_QTYPE_ILLUMINA) memset(p->valid_quality_characters + 64, 0, 63);
}

void fqparser_showValidSequenceCharacters(fqparser *p){
    int i;
    for(i=0;i<250;i++){
        if((i>31) && (i < 127)) printf("%c", i);
        else printf("-");
    }
    printf("\n");
    for(i=0;i<250;i++){
        printf("%d", p->valid_sequence_characters[i]);
    }
    printf("\n");
}

void fqparser_showValidQualityCharacters(fqparser *p){
    int i;
    for(i=0;i<250;i++){
        if((i>31) && (i < 127)) printf("%c", i);
        else printf("-");
    }
    printf("\n");
    for(i=0;i<250;i++){
        printf("%d", p->valid_quality_characters[i]);
    }
    printf("\n");
}

char fqparser_step(fqparser *p){
    switch(p->entry_point){
        case FQ_PARSER_ENTRY_START: goto entry_start;
        case FQ_PARSER_ENTRY_LOOP: goto entry_loop;
        case FQ_PARSER_ENTRY_QUALITY: goto entry_quality;        
        case FQ_PARSER_ENTRY_DONE: goto entry_done;
    }
entry_start:
    p->current_state = FQ_PARSER_STATE_INIT;
    p->line_number = 1;
    // Ask for chunks of data, until we're not given any more:
    while(1){
        // Get a new chunk if needed:
        if(p->input_buffer_offset == p->input_buffer_size){
            p->input_buffer_size = p->callbacks->readBuffer(p->user, p->input_buffer, p->input_buffer_max);
            // Check for the end of the file:
            if(p->input_buffer_size == 0){
                // Check we're in the correct state to end:
                if(p->current_state != FQ_PARSER_STATE_INIT){
                    //There was an error;
                    p->callbacks->error(p->user, FQ_ERROR_INCOMPLETE_FINAL_READ, p->line_number, p->current_character);
                    p->error = 1;
                }
                return FQ_PARSER_COMPLETE;
            }
            p->input_buffer_offset = 0;
        }
        //Process the chunk of data we have:
        while(p->input_buffer_offset < p->input_buffer_size){
            //Check if we've been asked to stop:
            if(p->callbacks->interrupt != 0){
                p->entry_point = FQ_PARSER_ENTRY_DONE;
                return FQ_PARSER_COMPLETE;
            }
            // Get the next character, and advance the input buffer offset:
            p->current_character = p->input_buffer[p->input_buffer_offset];
            p->input_buffer_offset++;
            if(p->current_character == '\n') p->line_number ++; // If this is a line break, increment out line count.
            // Parse the single character based on the state we're currently in:
            switch(p->current_state){
                case FQ_PARSER_STATE_INIT:{
                    if(p->current_character == '@'){
                        p->current_state = FQ_PARSER_STATE_HEADER_1;
                        p->output_buffer_offset = 0;
                        p->callbacks->startRead(p->user);
                        p->entry_point = FQ_PARSER_ENTRY_LOOP;
                        return FQ_PARSER_INCOMPLETE;
                    }
                    if(p->current_character == '\n') break;
                    p->callbacks->error(p->user, FQ_ERROR_MISSING_HEADER, p->line_number, p->current_character);
                    p->entry_point = FQ_PARSER_ENTRY_DONE;
                    p->error = 1;
                    return FQ_PARSER_COMPLETE;
                } // End of processing FQ_PARSER_STATE_INIT state.
                case FQ_PARSER_STATE_HEADER_1:{
                    if (p->current_character == '\n'){
                        p->callbacks->header1Block(p->user, p->output_buffer, p->output_buffer_offset, 1);
                        p->output_buffer_offset = 0;
                        p->current_state = FQ_PARSER_STATE_SEQUENCE;
                        p->sequence_length = 0;
                        p->entry_point = FQ_PARSER_ENTRY_LOOP;
                        return FQ_PARSER_INCOMPLETE;
                    } else {
                        p->output_buffer[p->output_buffer_offset] = p->current_character;
                        p->output_buffer_offset ++;
                        if(p->output_buffer_offset == p->output_buffer_max){
                            p->callbacks->header1Block(p->user, p->output_buffer, p->output_buffer_offset, 0);
                            p->output_buffer_offset = 0;
                            p->entry_point = FQ_PARSER_ENTRY_LOOP;
                            return FQ_PARSER_INCOMPLETE;
                        }
                    }
					break;
                } // End of processing FQ_PARSER_STATE_HEADER_1 state.
                case FQ_PARSER_STATE_SEQUENCE:{
                    if(p->current_character == '\n'){
                        p->current_state = FQ_PARSER_STATE_SEQUENCE_NEWLINE;
                    }
                    else {
                        if(p->valid_sequence_characters[(int)(p->current_character)] != 0){
                            p->callbacks->error(p->user, FQ_ERROR_INVALID_SEQUENCE_CHARACTER, p->line_number, p->current_character);
                            p->entry_point = FQ_PARSER_ENTRY_DONE;
                            p->error = 1;
                            return FQ_PARSER_COMPLETE;
                        }
                        p->output_buffer[p->output_buffer_offset] = p->current_character;
                        p->output_buffer_offset ++;
                        p->sequence_length ++ ;
                        if(p->output_buffer_offset == p->output_buffer_max){
                            p->callbacks->sequenceBlock(p->user, p->output_buffer, p->output_buffer_offset, 0);
                            p->output_buffer_offset = 0;
                            p->entry_point = FQ_PARSER_ENTRY_LOOP;
                            return FQ_PARSER_INCOMPLETE;
                        }
                    }
                    break;
                } // End of processing FQ_PARSER_STATE_SEQUENCE state.
                case FQ_PARSER_STATE_SEQUENCE_NEWLINE:{
					if(p->current_character == '+'){
						p->callbacks->sequenceBlock(p->user, p->output_buffer, p->output_buffer_offset, 1);
						p->output_buffer_offset = 0;
						p->current_state = FQ_PARSER_STATE_HEADER_2;
						p->entry_point = FQ_PARSER_ENTRY_LOOP;
						return FQ_PARSER_INCOMPLETE;
					}
					else p->current_state = FQ_PARSER_STATE_SEQUENCE;
                } // End of processing FQ_PARSER_STATE_SEQUENCE_NEWLINE state.
                case FQ_PARSER_STATE_HEADER_2:{
					if(p->current_character == '\n'){
						p->callbacks->header2Block(p->user, p->output_buffer, p->output_buffer_offset, 1);
						p->output_buffer_offset = 0;
						p->current_state = FQ_PARSER_STATE_QUALITY;
						p->quality_length = 0;
						p->entry_point = FQ_PARSER_ENTRY_LOOP;
						return FQ_PARSER_INCOMPLETE;
					} else {
						p->output_buffer[p->output_buffer_offset] = p->current_character;
						p->output_buffer_offset ++;
						if(p->output_buffer_offset == p->output_buffer_max){
							p->callbacks->header2Block(p->user, p->output_buffer, p->output_buffer_offset, 0);
							p->output_buffer_offset = 0;
							p->entry_point = FQ_PARSER_ENTRY_LOOP;
							return FQ_PARSER_INCOMPLETE;
						}
					}
					break;
                } // End of processing FQ_PARSER_STATE_HEADER_2 state.
                case FQ_PARSER_STATE_QUALITY:{
					if(p->current_character != '\n'){
						p->output_buffer[p->output_buffer_offset] = p->current_character;
						p->output_buffer_offset ++;
                        if(p->valid_quality_characters[(int)(p->current_character)] != 0){
                            p->callbacks->error(p->user, FQ_ERROR_INVALID_QUALITY_CHARACTER, p->line_number, p->current_character);
                            p->entry_point = FQ_PARSER_ENTRY_DONE;
                            p->error = 1;
                            return FQ_PARSER_COMPLETE;
                        }
						p->quality_length ++;
						if(p->output_buffer_offset == p->output_buffer_max){
							p->callbacks->qualityBlock(p->user, p->output_buffer, p->output_buffer_offset, 0);
							p->output_buffer_offset = 0;
							p->entry_point = FQ_PARSER_ENTRY_LOOP;
							return FQ_PARSER_INCOMPLETE;
						}
					}
					if(p->quality_length == p->sequence_length){
						p->callbacks->qualityBlock(p->user, p->output_buffer, p->output_buffer_offset, 1);
						p->entry_point = FQ_PARSER_ENTRY_QUALITY;
						return FQ_PARSER_INCOMPLETE;
entry_quality:
						p->output_buffer_offset = 0;
						p->current_state = FQ_PARSER_STATE_INIT;
						p->callbacks->endRead(p->user);
                        if(p->interleaved == FQ_INTERLEAVED) p->user = !(p->user);
						p->entry_point = FQ_PARSER_ENTRY_LOOP;
						return FQ_PARSER_INCOMPLETE;
					}
                } // End of processing FQ_PARSER_STATE_QUALITY state.
            } // End of the state switch
entry_loop:;
        } // End of processing the chunk.
    }
    entry_done:
    return FQ_PARSER_COMPLETE;
}
