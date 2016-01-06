#include "fqheader.h"

fqstatus fqparser_init(fqparser *p, fqparser_callbacks *callbacks, fqbytecount in_bufsize, fqbytecount out_bufsize, void *user){
    p->input_buffer = (char*)malloc(in_bufsize * sizeof(char));
    if(p->input_buffer == NULL) return FQ_STATUS_FAIL;
    p->output_buffer = (char*)malloc((out_bufsize + 1) * sizeof(char));
    if(p->output_buffer == NULL){
        free(p->input_buffer);
        return FQ_STATUS_FAIL;
    }
    p->input_buffer_size = 0;
    p->input_buffer_max = in_bufsize;
    p->input_buffer_offset = 0;
    p->output_buffer_size = 0;
    p->output_buffer_max = out_bufsize;
    p->output_buffer_offset = 0;
    p->output_buffer[p->output_buffer_max] = '\0';
    p->user = user;
    p->callbacks = callbacks;
    p->entry_point = FQ_PARSER_ENTRY_START;
    p->sequence_length = 0;
    p->quality_length = 0;
    p->line_number = 0;
    return FQ_STATUS_OK;
}

// fqstatus fqparser_init(fqparser *p, fqbuffer *buffer_in, fqbuffer *buffer_out, fqparser_callbacks *callbacks, void *user){
//     p->buffer_in = buffer_in;
//     p->buffer_out = buffer_out;
//     p->callbacks = callbacks;
//     p->user = user;
//     
//     p->error = 0;
//     p->index_buffer_in = 0;
//     p->length_buffer_in = 0;
//     p->line_number = 0;
//     return FQ_STATUS_OK;
// }
//
void fqparser_free(fqparser *p){
    free(p->input_buffer);
    free(p->output_buffer);
    return;
}

char fqparser_step(fqparser *p){
    switch(p->entry_point){
        case FQ_PARSER_ENTRY_START: goto entry_start;
        case FQ_PARSER_ENTRY_LOOP: goto entry_loop;
        case FQ_PARSER_ENTRY_DONE: goto entry_done;
    }
entry_start:
    p->current_state = FQ_PARSER_STATE_INIT;
    p->line_number = 1;
    // Ask for chunks of data, until we're not given any more:
    while(1){
        // Get a new chunk if needed:
        if(p->input_buffer_offset == p->input_buffer_size){
            p->input_buffer_size = p->callbacks->readBuffer(p->input_buffer, p->input_buffer_max);
            if(p->input_buffer_size == 0) return 1; // Checks for EOF.
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
                        return 0;
                    }
                    if(p->current_character == '\n') break;
                    p->callbacks->error(p->user, FQ_ERROR_MISSING_HEADER, p->line_number, p->current_character);
                    p->entry_point = FQ_PARSER_ENTRY_DONE;
                    p->error = 1;
                    return 1;
                } // End of processing FQ_PARSER_STATE_INIT state.
                case FQ_PARSER_STATE_HEADER_1:{
                    if (p->current_character == '\n'){
                        p->callbacks->header1Block(p->user, p->output_buffer, p->output_buffer_offset, 1);
                        p->output_buffer_offset = 0;
                        p->current_state = FQ_PARSER_STATE_SEQUENCE;
                        p->sequence_length = 0;
                        p->entry_point = FQ_PARSER_ENTRY_LOOP;
                        return 0;
                    } else {
                    //     p->output_buffer[p->output_buffer_pos] = p->current_character;
                    //     p->output_buffer_pos ++;
                    //     if(p->output_buffer_pos == p->output_buffer_size){
                    //         p->callbacks->header1Block(p->user, p->output_buffer, p->output_buffer_pos, 0);
                    //         p->output_buffer_pos = 0;
                    //         p->entry_point = ENTRY_LOOP;
                    //         return 0;
                    //     }
                    }
					break;
                } // End of processing FQ_PARSER_STATE_HEADER_1 state.
                case FQ_PARSER_STATE_SEQUENCE:{} // End of processing FQ_PARSER_STATE_SEQUENCE state.
                case FQ_PARSER_STATE_SEQUENCE_NEWLINE:{} // End of processing FQ_PARSER_STATE_SEQUENCE_NEWLINE state.
                case FQ_PARSER_STATE_HEADER_2:{} // End of processing FQ_PARSER_STATE_HEADER_2 state.
                case FQ_PARSER_STATE_QUALITY:{} // End of processing FQ_PARSER_STATE_QUALITY state.
            } // End of the state switch
entry_loop:;
        } // End of processing the chunk.
    }
entry_done:
return 1;
}
//
// char fqparser_step(fqparser *p){
//     switch(p->entry_point){
//         case FQ_PARSER_ENTRY_START: goto entry_start;
//         case FQ_PARSER_ENTRY_LOOP: goto entry_loop;
//         case FQ_PARSER_ENTRY_QUALITY: goto entry_quality;
//         case FQ_PARSER_ENTRY_DONE: goto entry_done;
//     }
//     //START state
// entry_start:
//     p->current_state = FQ_PARSER_STATE_INIT;
//     p->line_number = 1;
//     // Loop through blocks of the input buffer, and process them one character at a time:
//     while(1){
//         if(p->index_buffer_in == p->length_buffer_in){
//             p->index_buffer_in = 0;
//             p->callbacks->readBuffer(&(p->length_buffer_in));
//             if (p->length_buffer_in == 0) return FQ_PARSER_COMPLETE;
//         }
//         // Loop through the block reading into the input buffer:
//         while(p->index_buffer_in < p->length_buffer_in){
//             //Check if we've been asked to stop:
//             if(p->callbacks->interrupt != 0){
//                 p->entry_point = FQ_PARSER_ENTRY_DONE;
//                 return FQ_PARSER_COMPLETE;
//             }
//             // Get a single character from the input buffer and update out position in it:
//             p->current_character = p->buffer_in->data[p->index_buffer_in];
//             p->index_buffer_in ++;
//             if(p->current_character == '\n') p->line_number ++;
//             // Parse the single character based on the state we're currently in:
//             switch(p->current_state){
//                 case FQ_PARSER_STATE_INIT:{
//                     if(p->current_character == '@'){
//                         p->current_state = FQ_PARSER_STATE_HEADER_1;
//                         p->callbacks->startRead(p->user);
//                         p->entry_point = FQ_PARSER_ENTRY_LOOP;
//                         return FQ_PARSER_INCOMPLETE;
//                     }
//                     if(p->current_character == '\n') break;
//                     p->callbacks->error(p->user, FQ_ERROR_MISSING_HEADER, p->line_number, p->current_character);
//                     p->entry_point = FQ_PARSER_ENTRY_DONE;
//                     p->error = 1;
//                     return FQ_PARSER_COMPLETE;
//                 } // End block FQ_PARSER_STATE_INIT
//                 case FQ_PARSER_STATE_HEADER_1:{
//                     if (p->current_character == '\n'){
//                         p->callbacks->header1Block(p->user, p->buffer_out, 1);
//                         fqbuffer_reset(p->buffer_out);
//                         p->current_state = FQ_PARSER_STATE_SEQUENCE;
//                         p->sequence_length = 0;
//                         p->entry_point = FQ_PARSER_ENTRY_LOOP;
//                         return FQ_PARSER_INCOMPLETE;
//                     } else {
//                         fqbuffer_appendchar(p->buffer_out, p->current_character);
//                     }
//                     break;
//                 } // End block FQ_PARSER_STATE_HEADER_1
//                 case FQ_PARSER_STATE_SEQUENCE_NEWLINE:{
//                     if(p->current_character == '+'){
//                         p->callbacks->sequenceBlock(p->user, p->buffer_out, 1);
//                         fqbuffer_reset(p->buffer_out);
//                         p->current_state = FQ_PARSER_STATE_HEADER_2;
//                         p->entry_point = FQ_PARSER_ENTRY_LOOP;
//                         return FQ_PARSER_INCOMPLETE;
//                     }
//                     else p->current_state = FQ_PARSER_STATE_SEQUENCE;
//                 } // end sequence new line switch block
//                 case FQ_PARSER_STATE_SEQUENCE:{
//                     if(p->current_character == '\n'){
//                             p->current_state = FQ_PARSER_STATE_SEQUENCE_NEWLINE;
//                         }
//                         else {
//                         // if(fq_valid_character(p->current_character, p->valid_chars) == 0){
//                         //     p->callbacks->error(p->user, FQ_ERROR_INVALID_SEQUENCE_CHARACTER, p->line_number, p->current_character);
//                         //     p->entry_point = FQ_PARSER_ENTRY_DONE;
//                         //     p->error = 1;
//                         //     return 1;
//                         // }
//                         fqbuffer_appendchar(p->buffer_out, p->current_character);
//                         p->sequence_length ++ ;
//                     }
//                     break;
//                 } // end sequence state switch block
//                 case FQ_PARSER_STATE_HEADER_2:{
//                     if(p->current_character == '\n'){
//                         p->callbacks->header2Block(p->user, p->buffer_out, 1);
//                         fqbuffer_reset(p->buffer_out);
//                         p->current_state = FQ_PARSER_STATE_QUALITY;
//                         p->quality_length = 0;
//                         p->entry_point = FQ_PARSER_ENTRY_LOOP;
//                         return FQ_PARSER_INCOMPLETE;
//                     } else {
//                         fqbuffer_appendchar(p->buffer_out, p->current_character);
//                     }
//                     break;
//                 } // end header 2 state switch block
//                 case FQ_PARSER_STATE_QUALITY:{
//                     if(p->current_character != '\n'){
//                         fqbuffer_appendchar(p->buffer_out, p->current_character);
//                         p->quality_length ++;
//                         // // Check the quality character range:
//                         // if((p->current_character < p->file_offset) || (p->current_character > 126)){
//                         //     p->callbacks->error(p->user, FQ_ERROR_INVALID_QUALITY_CHARACTER, p->line_number, p->current_character);
//                         //     p->entry_point = ENTRY_DONE;
//                         //     p->error = 1;
//                         //     return 1;
//                         // }
//                     }
//                     if(p->quality_length == p->sequence_length){
//                         p->callbacks->qualityBlock(p->user, p->buffer_out, 1);
//                         p->entry_point = FQ_PARSER_ENTRY_QUALITY;
//                         return FQ_PARSER_INCOMPLETE;
// entry_quality:
//                         fqbuffer_reset(p->buffer_out);
//                         p->current_state = FQ_PARSER_STATE_INIT;
//                         p->callbacks->endRead(p->user);
//                         p->entry_point = FQ_PARSER_ENTRY_LOOP;
//                         return FQ_PARSER_INCOMPLETE;
//                     }
//                 } // end quality state switch block
//             } // End the character state switch
// entry_loop:;
//         }
//         //Check for EOF HERE!
//     }
//     if(p->current_state != FQ_PARSER_STATE_INIT){
//         //There was an error;
//         p->callbacks->error(p->user, FQ_ERROR_INCOMPLETE_FINAL_READ, p->line_number, p->current_character);
//         p->error = 1;
//     }
//     p->entry_point = FQ_PARSER_ENTRY_DONE;
// entry_done:
//     return FQ_PARSER_COMPLETE;
// }
