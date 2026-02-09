#include "../include/operations.h"



void select_operation(operation_T current_op, segment_T * program, stack_T * address_stack, int param1, int param2){
    switch (current_op){
        case OP_ADD:
        write_add(program, address_stack, param1, 0);
        break;

        case OP_SUB:
        write_sub(program, address_stack, param1, 0);
        break;

        case OP_RIGHT:
        write_right(program, address_stack, param1, 0);
        break;

        case OP_LEFT:
        write_left(program, address_stack, param1, 0);
        break;

        case OP_OUT:
        write_out(program, address_stack, 0, 0);
        break;
        
        case OP_IN:
        write_in(program, address_stack, 0, 0); 
        break;
        
        case OP_OPEN:
        write_open(program, address_stack, 0, 0);
        break;

        case OP_CLOSE:
        write_close(program, address_stack, 0, 0);
        break;

        default:
        break;
    }
}



operation_T char_to_op(char character){
    switch (character){
        case '+':
        return OP_ADD;
        
        case '-':
        return OP_SUB;

        case '>':
        return OP_RIGHT;
        
        case '<':
        return OP_LEFT;

        case '[':
        return OP_OPEN;
        
        case ']':
        return OP_CLOSE;

        case '.':
        return OP_OUT;
        
        case ',':
        return OP_IN;
        
        default:
        return OP_NONE;
    }
}

int is_op_countable(operation_T current_op){
    if(current_op == OP_ADD || current_op == OP_SUB) return 1;
    if(current_op == OP_LEFT || current_op == OP_RIGHT) return 1;
    return 0;
}

// From here start the "write operations" functions.
// In order to fit them into the optimizer thingy they must follow the following format:
// void(segment_T * program, stack_T * address_stack, int param1, int param2)
// obviously rename the parameters into something meaningful
void write_setup(segment_T * program, stack_T * address_stack, int unused1, int unused2){
    write_instruction(program, INST_SET_IMM, TAPE_PTR, BUFFER_TAPE);
    write_instruction(program, INST_SET_IMM, TAPE_VAL, 0);
    return;
}


void write_add(segment_T * program, stack_T * address_stack, int count, int unused){
    write_instruction(program, INST_LOAD_REG, TAPE_VAL, TAPE_PTR);
    write_instruction(program, INST_ADD_IMM, TAPE_VAL, count); 
    write_instruction(program, INST_STORE_REG, TAPE_PTR, TAPE_VAL);
    return;
}


void write_sub(segment_T * program, stack_T * address_stack, int count, int unused){
    write_instruction(program, INST_LOAD_REG, TAPE_VAL, TAPE_PTR);
    write_instruction(program, INST_SUB_IMM, TAPE_VAL, count); 
    write_instruction(program, INST_STORE_REG, TAPE_PTR, TAPE_VAL);
    return;
}

void write_right(segment_T * program, stack_T * address_stack, int count, int unused){
    write_instruction(program, INST_ADD_IMM, TAPE_PTR, 4 * count); 
    return;
}

void write_left(segment_T * program, stack_T * address_stack, int count, int unused){
    write_instruction(program, INST_SUB_IMM, TAPE_PTR, 4 * count);
    return;
}

void write_zero(segment_T * program, stack_T * address_stack, int unused1, int unused2){
    write_instruction(program, INST_SET_IMM, TAPE_VAL, 0);
    write_instruction(program, INST_STORE_REG, TAPE_PTR, TAPE_VAL);
    return;
}


void write_out(segment_T * program, stack_T * address_stack, int unused1, int unused2){
    write_instruction(program, INST_LOAD_REG, TAPE_VAL, TAPE_PTR);
    write_instruction(program, INST_STORE_B_IMM, BUFFER_OUTPUT, EBX);
    write_instruction(program, INST_SET_IMM, TAPE_VAL, 0);
    write_instruction(program, INST_STORE_B_IMM, BUFFER_OUTPUT + 1, TAPE_VAL);

    write_instruction(program, INST_SET_IMM, ESI, BUFFER_OUTPUT);
    write_instruction(program, INST_SET_IMM, EAX, SYS_WRITE);
    write_instruction(program, INST_SET_IMM, EDI, FD_STDOUT);
    write_instruction(program, INST_SET_IMM, EDX, 1);
    write_instruction(program, INST_SYSCALL, 0, 0);
    return;
}

void write_in(segment_T * program, stack_T * address_stack, int unused1, int unused2){
    write_instruction(program, INST_SET_IMM, EAX, SYS_READ);
    write_instruction(program, INST_SET_IMM, EDI, FD_STDIN);
    write_instruction(program, INST_SET_REG, ESI, TAPE_PTR);
    write_instruction(program, INST_SET_IMM, EDX, 1);
    write_instruction(program, INST_SYSCALL, 0, 0);
    return;
}

void write_open(segment_T * program, stack_T * address_stack, int unused1, int unused2){
    write_instruction(program, INST_LOAD_REG, TAPE_VAL, TAPE_PTR);
    write_instruction(program, INST_CMP, TAPE_VAL, 0);
    write_instruction(program, INST_JZ, 0, 0); // Do nothing unless overwritten (Might be tweaked later)
    stack_push(address_stack, program->head); // Stores the location of the back jump   
    return;
}

void write_close(segment_T * program, stack_T * address_stack, int unused1, int unused2){
    int source_address = 0, jump_distance = 0;
    source_address = stack_pop(address_stack);
    write_instruction(program, INST_LOAD_REG, TAPE_VAL, TAPE_PTR);
    write_instruction(program, INST_CMP, TAPE_VAL, 0);
    write_instruction(program, INST_JNZ, 0, 0); 
    
    jump_distance = program->head - source_address;
    overwrite_jump(program, source_address - 4, jump_distance);
    overwrite_jump(program, program->head - 4, - jump_distance);
    return;
}

void write_end(segment_T * program, stack_T * address_stack, int unused1, int unused2){
    // Write \n to output buffer to flush STDOUT
    write_instruction(program, INST_SET_IMM, EBX, '\n');
    write_instruction(program, INST_STORE_B_IMM, BUFFER_OUTPUT, EBX);
    write_instruction(program, INST_SET_IMM, EAX, 0);
    write_instruction(program, INST_STORE_B_IMM, BUFFER_OUTPUT + 1, EBX);

    // Flush
    write_instruction(program, INST_SET_IMM, ESI, BUFFER_OUTPUT);
    write_instruction(program, INST_SET_IMM, EAX, SYS_WRITE);
    write_instruction(program, INST_SET_IMM, EDI, FD_STDOUT);
    write_instruction(program, INST_SET_IMM, EDX, 1);
    write_instruction(program, INST_SYSCALL, 0, 0);

    // Exit with return code 0
    write_instruction(program, INST_SUB_REG, EDI, EDI);  
    write_instruction(program, INST_SET_IMM, EAX, SYS_EXIT);
    write_instruction(program, INST_SYSCALL, 0, 0);
    return;
}