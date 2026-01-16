#include "../include/instructions.h"
#include "../include/registers.h"

size_t store_sequence(segment_T * program, int32_t sequence, size_t length){
    inst_buffer_T inst_buffer = {.integer = sequence};
    for(size_t i = 0; i < length; i++){
        program->buffer[program->head] = inst_buffer.bytes[i];
        program->head++;
    }
    return length;
}


// This function returns the amount of bytes the instruction occupies
size_t write_instruction(segment_T * program, instruction_T instruction, int32_t param0, int32_t param1){
    size_t count = 0;
    int32_t modrm = 0;
    int32_t sib = 0;
    switch (instruction){

        case INST_SET_REG:
            modrm = mod_rm(0b11, param0 & 0b111, param1 & 0b111);
            count += store_sequence(program, LOAD, 1);
            count += store_sequence(program, modrm, 1);
            return count;


        case INST_SET_IMM:
            count += store_sequence(program, SET_IMM(param0), 1);
            count += store_sequence(program, param1, 4);
            return count;

        case INST_SYSCALL:       
            return store_sequence(program, __builtin_bswap16(SYSCALL), 2);

            
        case INST_LOAD_REG:

            modrm = (param1 != EBP) ? mod_rm(0b00, param0 & 0b111, param1 & 0b111) : mod_rm(0b01, param0 & 0b111, param1 & 0b111);
            count += store_sequence(program, LOAD, 1);
            count += store_sequence(program, modrm, 1);
            if(param1 == ESP) count += store_sequence(program, sib_byte(0b00, 0b100, 0b100), 1);
            return count;
        
        case INST_LOAD_IMM:
            modrm = mod_rm(0b00, param0 & 0b111, 0b100);
            sib = sib_byte(0b00, 0b100, 0b101);
            count += store_sequence(program, LOAD, 1);
            count += store_sequence(program, modrm, 1);
            count += store_sequence(program, sib, 1);
            count += store_sequence(program, param1, 4);
            return count;
        
        case INST_STORE_REG:
            modrm = (param1 != EBP) ? mod_rm(0b00, param1 & 0b111, param0 & 0b111) : mod_rm(0b01, param1 & 0b111, param0 & 0b111);
            count += store_sequence(program, STORE, 1);
            count += store_sequence(program, modrm, 1);
            if(param1 == ESP) count += store_sequence(program, sib_byte(0b00, 0b100, 0b100), 1);
            return count;
        
        case INST_STORE_IMM:
            modrm = mod_rm(0b00, param1 & 0b111, 0b101);
            sib = sib_byte(0b00, 0b100, 0b101);
            count += store_sequence(program, STORE, 1);
            count += store_sequence(program, modrm, 1);
            count += store_sequence(program, sib, 1);
            count += store_sequence(program, param0, 4);
            return count;
            
        case INST_STORE_B_IMM:
            modrm = mod_rm(0b00, param1 & 0b111, 0b100);
            sib = sib_byte(0b00, 0b100, 0b101);
            count += store_sequence(program, STORE_B, 1);
            count += store_sequence(program, modrm, 1);
            count += store_sequence(program, sib, 1);
            count += store_sequence(program, param0, 4);
            return count;

        case INST_ADD_REG:
            modrm = mod_rm(0b11, param0 & 0b111, param1 & 0b111);
            count += store_sequence(program, ADD_REG, 1);
            count += store_sequence(program, modrm, 1);
            return count;

        case INST_ADD_IMM:
            modrm = mod_rm(0b11, 0b000, param0 & 0b111);
            count += store_sequence(program, ADDSUB_IMM, 1);
            count += store_sequence(program, modrm, 1);
            count += store_sequence(program, param1, 4);
            return count;

        case INST_SUB_REG:
            modrm = mod_rm(0b11, param0 & 0b111, param1 & 0b111);
            count += store_sequence(program, SUB_REG, 1);
            count += store_sequence(program, modrm, 1);
            return count;

        case INST_SUB_IMM:
            modrm = mod_rm(0b11, 0b101, param0 & 0b111);
            count += store_sequence(program, ADDSUB_IMM, 1);
            count += store_sequence(program, modrm, 1);
            count += store_sequence(program, param1, 4);
            return count;

        case INST_CMP:
            modrm = mod_rm(0b11, 0b111, param0 & 0b111);
            count += store_sequence(program, CMP, 1);
            count += store_sequence(program, modrm, 1);
            count += store_sequence(program, param1, 4);
            return count;

        case INST_JZ:
            count += store_sequence(program, __builtin_bswap16(JZ), 2);
            count += store_sequence(program, param0, 4);
            return count;

        case INST_JNZ:
            count += store_sequence(program, __builtin_bswap16(JNZ), 2);
            count += store_sequence(program, param0, 4);
            return count;

        default:
            return 0;
    }
}


size_t write_data(segment_T * program, char * data, int32_t count){
    if(!data) return 0;
    int i = 0;
    while(i < count && data[i] != '\0'){
        program->buffer[program->head] = data[i];
        program->head++;
        i++;
    }
    return i;
}


int32_t mod_rm(int mod, int reg, int rm){
    mod &= 0b11;
    reg &= 0b111;
    rm &= 0b111;
    return ((mod << 6) | (reg << 3) | rm) & 0xFF;
}

int32_t sib_byte(int scale, int index, int base){
    scale &= 0b11;
    index &= 0b111;
    base &= 0b111;
    return ((scale << 6) | (index << 3) | base) & 0xFF;
}

void overwrite_jump(segment_T * program, int location, int jump_distace){
    inst_buffer_T buffer = {.integer = jump_distace};
    
    for (int i = 0; i < 4; i++){
        program->buffer[location + i] = buffer.bytes[i];
    }
}