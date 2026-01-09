#include "../include/instructions.h"
#include "../include/opcodes.h"

// This function returns the amount of bytes the instruction occupies
size_t write_instruction(FILE * file, instruction_T instruction, int32_t param0, int32_t param1){
    int32_t buffer;
    switch (instruction){
        case INST_MOV:
            buffer = MOV(param0);
            fwrite(&buffer, 1, 1, file);
            buffer = param1;
            fwrite(&buffer, 1, 4, file);
            return 5;
        case INST_SYSCALL:
            buffer = __builtin_bswap16(SYSCALL);
            fwrite(&buffer, 2, 1, file);
            return 2;
        default:
            break;
    }
    return 0;
}