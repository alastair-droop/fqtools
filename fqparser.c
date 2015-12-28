#include "fqheader.h"

fqstatus fqparser_init(fqparser *p, fqfileset *fs_in, fqfileset *fs_out, fqparser_callbacks *callbacks, void *user){
    p->fs_in = fs_in;
    p->fs_out = fs_out;
    p->callbacks = callbacks;
    p->user = user;
	p->entry_point = FQ_PARSER_ENTRY_START;
	p->error = 0;
    return FQ_STATUS_OK;
}

void fqparser_free(fqparser *p){
    return;
}

fqstatus fqparser_step(fqparser *p){
    fqbytecount bytes_read_1, bytes_read_2;
    
    switch(p->entry_point){
        case FQ_PARSER_ENTRY_START: goto entry_start;
        // case ENTRY_LOOP: goto entry_loop;
        // case ENTRY_QUALITY: goto entry_quality;
        // case ENTRY_DONE: goto entry_done;
    }
    //START state
entry_start:
    p->current_state = FQ_PARSER_STATE_INIT;
    p->line_number = 1;
    // Loop through blocks of the input buffer, and process them one character at a time:
    while(1){
        // Read a block into the input buffer:
        fqfileset_read(p->fs_in, &bytes_read_1, &bytes_read_2);
        printf("Main loop: read %zd bytes\n", bytes_read_1);
        if(bytes_read_1 != p->fs_in->file_1.buffer.size) return FQ_STATUS_OK;
    }
//         // Reset the input buffer & get a chunk from the file:
//         p->input_buffer_pos = 0;
//         p->characters_read = fqfile_read(f, p->input_buffer, p->input_buffer_size, &(p->file_status));
//         if(p->file_status == FQFILE_STATUS_ERR){
//             p->callbacks->error(p->user, FQ_ERROR_FILE, p->line_number, p->current_character);
//             p->entry_point = ENTRY_DONE;
//             p->error = 1;
//             return 1;
//         }
//         //Loop through the input buffer...
//         while(p->input_buffer_pos < p->characters_read){
//             if(p->callbacks->interrupt != 0){
//                 p->entry_point = ENTRY_DONE;
//                 return 1;
//             }
//             // Get a single character from the input buffer and update out position in it:
//             p->current_character = p->input_buffer[p->input_buffer_pos];
//             p->input_buffer_pos ++;
//             if(p->current_character == '\n') p->line_number ++;
//             // Parse the single character based on the state we're currently in:
//             switch(p->current_state){
//                 case STATE_INIT:{
//                     if(p->current_character == '@'){
//                         p->current_state = STATE_HEADER_1;
//                         p->output_buffer_pos = 0;
//                         p->callbacks->startRead(p->user);
//                         p->entry_point = ENTRY_LOOP;
//                         return 0;
//                     }
//                     if(p->current_character == '\n') break;
//                     p->callbacks->error(p->user, FQ_ERROR_MISSING_HEADER, p->line_number, p->current_character);
//                     p->entry_point = ENTRY_DONE;
//                     p->error = 1;
//                     return 1;
//                 } // End initial state switch block
//                 case STATE_HEADER_1:{
//                     if (p->current_character == '\n'){
//                         p->callbacks->header1Block(p->user, p->output_buffer, p->output_buffer_pos, 1);
//                         p->output_buffer_pos = 0;
//                         p->current_state = STATE_SEQUENCE;
//                         p->sequence_length = 0;
//                         p->entry_point = ENTRY_LOOP;
//                         return 0;
//                     } else {
//                         p->output_buffer[p->output_buffer_pos] = p->current_character;
//                         p->output_buffer_pos ++;
//                         if(p->output_buffer_pos == p->output_buffer_size){
//                             p->callbacks->header1Block(p->user, p->output_buffer, p->output_buffer_pos, 0);
//                             p->output_buffer_pos = 0;
//                             p->entry_point = ENTRY_LOOP;
//                             return 0;
//                         }
//                     }
//                     break;
//                 } // End header 1 state switch block
//                 case STATE_SEQUENCE_NEWLINE:{
//                     if(p->current_character == '+'){
//                         p->callbacks->sequenceBlock(p->user, p->output_buffer, p->output_buffer_pos, 1);
//                         p->output_buffer_pos = 0;
//                         p->current_state = STATE_HEADER_2;
//                         p->entry_point = ENTRY_LOOP;
//                         return 0;
//                     }
//                     else p->current_state = STATE_SEQUENCE;
//                 } // end sequence new line switch block
//                 case STATE_SEQUENCE:{
//                     if(p->current_character == '\n'){
//                         p->current_state = STATE_SEQUENCE_NEWLINE;
//                     }
//                     else {
//                         if(fq_valid_character(p->current_character, p->valid_chars) == 0){
//                             p->callbacks->error(p->user, FQ_ERROR_INVALID_SEQUENCE_CHARACTER, p->line_number, p->current_character);
//                             p->entry_point = ENTRY_DONE;
//                             p->error = 1;
//                             return 1;
//                         }
//                         p->output_buffer[p->output_buffer_pos] = p->current_character;
//                         p->output_buffer_pos ++;
//                         p->sequence_length ++ ;
//                         if(p->output_buffer_pos == p->output_buffer_size){
//                             p->callbacks->sequenceBlock(p->user, p->output_buffer, p->output_buffer_pos, 0);
//                             p->output_buffer_pos = 0;
//                             p->entry_point = ENTRY_LOOP;
//                             return 0;
//                         }
//                     }
//                     break;
//                 } // end sequence state switch block
//                 case STATE_HEADER_2:{
//                     if(p->current_character == '\n'){
//                         p->callbacks->header2Block(p->user, p->output_buffer, p->output_buffer_pos, 1);
//                         p->output_buffer_pos = 0;
//                         p->current_state = STATE_QUALITY;
//                         p->quality_length = 0;
//                         p->entry_point = ENTRY_LOOP;
//                         return 0;
//                     } else {
//                         p->output_buffer[p->output_buffer_pos] = p->current_character;
//                         p->output_buffer_pos ++;
//                         if(p->output_buffer_pos == p->output_buffer_size){
//                             p->callbacks->header2Block(p->user, p->output_buffer, p->output_buffer_pos, 0);
//                             p->output_buffer_pos = 0;
//                             p->entry_point = ENTRY_LOOP;
//                             return 0;
//                         }
//                     }
//                     break;
//                 } // end header 2 state switch block
//                 case STATE_QUALITY:{
//                     if(p->current_character != '\n'){
//                         p->output_buffer[p->output_buffer_pos] = p->current_character;
//                         p->output_buffer_pos ++;
//                         // Check the quality character range:
//                         if((p->current_character < p->file_offset) || (p->current_character > 126)){
//                             p->callbacks->error(p->user, FQ_ERROR_INVALID_QUALITY_CHARACTER, p->line_number, p->current_character);
//                             p->entry_point = ENTRY_DONE;
//                             p->error = 1;
//                             return 1;
//                         }
//                         p->quality_length ++;
//                         if(p->output_buffer_pos == p->output_buffer_size){
//                             p->callbacks->qualityBlock(p->user, p->output_buffer, p->output_buffer_pos, 0);
//                             p->output_buffer_pos = 0;
//                             p->entry_point = ENTRY_LOOP;
//                             return 0;
//                         }
//                     }
//                     if(p->quality_length == p->sequence_length){
//                         p->callbacks->qualityBlock(p->user, p->output_buffer, p->output_buffer_pos, 1);
//                         p->entry_point = ENTRY_QUALITY;
//                         return 0;
// entry_quality:
//                         p->output_buffer_pos = 0;
//                         p->current_state = STATE_INIT;
//                         p->callbacks->endRead(p->user);
//                         p->entry_point = ENTRY_LOOP;
//                         return 0;
//                     }
//                 } // end quality state switch block
//             } // End current state switch
//
// entry_loop:;
//         } // End input buffer loop.
//         if(p->file_status == FQFILE_STATUS_EOF) break;
//     } // End while loop
//     if(p->current_state != STATE_INIT){
//         //There was an error;
//         p->callbacks->error(p->user, FQ_ERROR_INCOMPLETE_FINAL_READ, p->line_number, p->current_character);
//         p->error = 1;
//     }
//     p->entry_point = ENTRY_DONE;
// entry_done:
//     return 1;
    return FQ_STATUS_OK;
}
