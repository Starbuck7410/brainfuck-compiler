#include "../include/instructions.h"

size_t store_sequence(program_T * program, int32_t sequence, size_t length){
    inst_buffer_T inst_buffer = {.integer = sequence};
    for(size_t i = 0; i < length; i++){
        program->buffer[program->head] = inst_buffer.bytes[i];
        program->head++;
    }
    return length;
}


// This function returns the amount of bytes the instruction occupies
size_t write_instruction(program_T * program, instruction_T instruction, int32_t param0, int32_t param1){
    switch (instruction){
        case INST_MOV:
        size_t count = 0;
            count += store_sequence(program, MOV(param0), 1);
            count += store_sequence(program, param1, 4);
            return count;
        case INST_SYSCALL:       
            return store_sequence(program, __builtin_bswap16(SYSCALL), 2);
        default:
            return 0;
    }
}



size_t write_data(program_T * program, char * data, int32_t count){
    if(!data) return 0;
    int i = 0;
    while(i < count && data[i] != '\0'){
        program->buffer[program->head] = data[i];
        program->head++;
        i++;
    }
    return i;
}