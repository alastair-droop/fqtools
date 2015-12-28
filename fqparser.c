#include "fqheader.h"

fqstatus fqparser_init(fqparser *p, fqbuffer *buffer_in, fqbuffer *buffer_out, fqparser_callbacks *callbacks, void *user){
    p->buffer_in = buffer_in;
    p->buffer_out = buffer_out;
    p->callbacks = callbacks;
    p->user = user;
	p->entry_point = FQ_PARSER_ENTRY_START;
	p->error = 0;
    p->index_buffer_in = 0;
    p->length_buffer_in = 0;
    p->line_number = 0;
    return FQ_STATUS_OK;
}

void fqparser_free(fqparser *p){
    return;
}

char fqparser_step(fqparser *p){
    // fqbytecount bytes_read;
    
    switch(p->entry_point){
        case FQ_PARSER_ENTRY_START: goto entry_start;
        // case ENTRY_LOOP: goto entry_loop;
        // case ENTRY_QUALITY: goto entry_quality;
        case FQ_PARSER_ENTRY_DONE: goto entry_done;
    }
    //START state
entry_start:
    p->current_state = FQ_PARSER_STATE_INIT;
    p->line_number = 1;
    
    // Loop through blocks of the input buffer, and process them one character at a time:
    while(1){
        if(p->index_buffer_in == p->length_buffer_in){
            p->index_buffer_in = 0;
            p->callbacks->readBuffer(&(p->length_buffer_in));
            if (p->length_buffer_in == 0) return 1;
        }
        // Loop through the block reading into the input buffer:
        while(p->index_buffer_in < p->length_buffer_in){
            
            //Check if we've been asked to stop:
			if(p->callbacks->interrupt != 0){
				p->entry_point = FQ_PARSER_ENTRY_DONE;
				return 1;
			}
            
			// Get a single character from the input buffer and update out position in it:
            p->current_character = p->buffer_in->data[p->index_buffer_in];
            p->index_buffer_in ++;
            // printf("[%4zd] Current character: '%c'\n", p->line_number, p->current_character);
			if(p->current_character == '\n') p->line_number ++;
            // fqbuffer_appendchar(p->buffer_out, p->current_character);
            // p->callbacks->writeBuffer();
            // fqbuffer_reset(p->buffer_out);
            
			// Parse the single character based on the state we're currently in:
			switch(p->current_state){
                case FQ_PARSER_STATE_INIT:{
                    if(p->current_character == '@'){
                        p->current_state = FQ_PARSER_STATE_HEADER_1;
                        // p->output_buffer_pos = 0;
                        p->callbacks->startRead(p->user);
                        p->entry_point = FQ_PARSER_ENTRY_LOOP;
                        return 0;
                    }
                    if(p->current_character == '\n') break;
                    p->callbacks->error(p->user, FQ_ERROR_MISSING_HEADER, p->line_number, p->current_character);
                    p->entry_point = FQ_PARSER_ENTRY_DONE;
                    p->error = 1;
                    return 1;
                } // End block FQ_PARSER_STATE_INIT
				case FQ_PARSER_STATE_HEADER_1:{
					if (p->current_character == '\n'){
						p->callbacks->header1Block(p->user, p->output_buffer, p->output_buffer_pos, 1);
						p->output_buffer_pos = 0;
						p->current_state = STATE_SEQUENCE;
						p->sequence_length = 0;
						p->entry_point = ENTRY_LOOP;
						return 0;
					} else {
						p->output_buffer[p->output_buffer_pos] = p->current_character;
						p->output_buffer_pos ++;
						if(p->output_buffer_pos == p->output_buffer_size){
							p->callbacks->header1Block(p->user, p->output_buffer, p->output_buffer_pos, 0);
							p->output_buffer_pos = 0;
							p->entry_point = ENTRY_LOOP;
							return 0;
						}
					}
					break;
				} // End block FQ_PARSER_STATE_HEADER_1
                
            } // End the character state switch
            
        }
    }
	if(p->current_state != FQ_PARSER_STATE_INIT){
		//There was an error;
		p->callbacks->error(p->user, FQ_ERROR_INCOMPLETE_FINAL_READ, p->line_number, p->current_character);
		p->error = 1;
	}
	p->entry_point = FQ_PARSER_ENTRY_DONE;
entry_done:
    return 1;
}
