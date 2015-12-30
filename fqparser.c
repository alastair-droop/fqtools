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
    switch(p->entry_point){
        case FQ_PARSER_ENTRY_START: goto entry_start;
        case FQ_PARSER_ENTRY_LOOP: goto entry_loop;
        case FQ_PARSER_ENTRY_QUALITY: goto entry_quality;
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
            if (p->length_buffer_in == 0) return FQ_PARSER_COMPLETE;
        }
        // Loop through the block reading into the input buffer:
        while(p->index_buffer_in < p->length_buffer_in){
            //Check if we've been asked to stop:
            if(p->callbacks->interrupt != 0){
                p->entry_point = FQ_PARSER_ENTRY_DONE;
                return FQ_PARSER_COMPLETE;
            }
            // Get a single character from the input buffer and update out position in it:
            p->current_character = p->buffer_in->data[p->index_buffer_in];
            p->index_buffer_in ++;
            if(p->current_character == '\n') p->line_number ++;
            // Parse the single character based on the state we're currently in:
            switch(p->current_state){
                case FQ_PARSER_STATE_INIT:{
                    if(p->current_character == '@'){
                        p->current_state = FQ_PARSER_STATE_HEADER_1;
                        p->callbacks->startRead(p->user);
                        p->entry_point = FQ_PARSER_ENTRY_LOOP;
                        return FQ_PARSER_INCOMPLETE;
                    }
                    if(p->current_character == '\n') break;
                    p->callbacks->error(p->user, FQ_ERROR_MISSING_HEADER, p->line_number, p->current_character);
                    p->entry_point = FQ_PARSER_ENTRY_DONE;
                    p->error = 1;
                    return FQ_PARSER_COMPLETE;
                } // End block FQ_PARSER_STATE_INIT
                case FQ_PARSER_STATE_HEADER_1:{
                    if (p->current_character == '\n'){
                        p->callbacks->header1Block(p->user, p->buffer_out, 1);
                        fqbuffer_reset(p->buffer_out);
                        p->current_state = FQ_PARSER_STATE_SEQUENCE;
                        p->sequence_length = 0;
                        p->entry_point = FQ_PARSER_ENTRY_LOOP;
                        return FQ_PARSER_INCOMPLETE;
                    } else {
                        fqbuffer_appendchar(p->buffer_out, p->current_character);
                    }
                    break;
                } // End block FQ_PARSER_STATE_HEADER_1
                case FQ_PARSER_STATE_SEQUENCE_NEWLINE:{
                    if(p->current_character == '+'){
                        p->callbacks->sequenceBlock(p->user, p->buffer_out, 1);
                        fqbuffer_reset(p->buffer_out);
                        p->current_state = FQ_PARSER_STATE_HEADER_2;
                        p->entry_point = FQ_PARSER_ENTRY_LOOP;
                        return FQ_PARSER_INCOMPLETE;
                    }
                    else p->current_state = FQ_PARSER_STATE_SEQUENCE;
                } // end sequence new line switch block
                case FQ_PARSER_STATE_SEQUENCE:{
                    if(p->current_character == '\n'){
                            p->current_state = FQ_PARSER_STATE_SEQUENCE_NEWLINE;
                        }
                        else {
                        // if(fq_valid_character(p->current_character, p->valid_chars) == 0){
                        //     p->callbacks->error(p->user, FQ_ERROR_INVALID_SEQUENCE_CHARACTER, p->line_number, p->current_character);
                        //     p->entry_point = FQ_PARSER_ENTRY_DONE;
                        //     p->error = 1;
                        //     return 1;
                        // }
                        fqbuffer_appendchar(p->buffer_out, p->current_character);
                        p->sequence_length ++ ;
                    }
                    break;
                } // end sequence state switch block
                case FQ_PARSER_STATE_HEADER_2:{
                    if(p->current_character == '\n'){
                        p->callbacks->header2Block(p->user, p->buffer_out, 1);
                        fqbuffer_reset(p->buffer_out);
                        p->current_state = FQ_PARSER_STATE_QUALITY;
                        p->quality_length = 0;
                        p->entry_point = FQ_PARSER_ENTRY_LOOP;
                        return FQ_PARSER_INCOMPLETE;
                    } else {
                        fqbuffer_appendchar(p->buffer_out, p->current_character);
                    }
                    break;
                } // end header 2 state switch block
                case FQ_PARSER_STATE_QUALITY:{
                    if(p->current_character != '\n'){
                        fqbuffer_appendchar(p->buffer_out, p->current_character);
                        p->quality_length ++;
                        // // Check the quality character range:
                        // if((p->current_character < p->file_offset) || (p->current_character > 126)){
                        //     p->callbacks->error(p->user, FQ_ERROR_INVALID_QUALITY_CHARACTER, p->line_number, p->current_character);
                        //     p->entry_point = ENTRY_DONE;
                        //     p->error = 1;
                        //     return 1;
                        // }
                    }
                    if(p->quality_length == p->sequence_length){
                        p->callbacks->qualityBlock(p->user, p->buffer_out, 1);
                        p->entry_point = FQ_PARSER_ENTRY_QUALITY;
                        return FQ_PARSER_INCOMPLETE;
entry_quality:
                        fqbuffer_reset(p->buffer_out);
                        p->current_state = FQ_PARSER_STATE_INIT;
                        p->callbacks->endRead(p->user);
                        p->entry_point = FQ_PARSER_ENTRY_LOOP;
                        return FQ_PARSER_INCOMPLETE;
                    }
                } // end quality state switch block
            } // End the character state switch
entry_loop:;
        }
        //Check for EOF HERE!
    }
    if(p->current_state != FQ_PARSER_STATE_INIT){
        //There was an error;
        p->callbacks->error(p->user, FQ_ERROR_INCOMPLETE_FINAL_READ, p->line_number, p->current_character);
        p->error = 1;
    }
    p->entry_point = FQ_PARSER_ENTRY_DONE;
entry_done:
    return FQ_PARSER_COMPLETE;
}
