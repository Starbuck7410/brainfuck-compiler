#pragma once
#include "../include/instructions.h"
#include "../include/stack.h"
#include "../include/registers.h"
#include "../include/headers.h"


#define TAPE_VAL EBX
#define TAPE_PTR ESP
#define NUM_OPERATIONS 8

// These might seem useless for now, 
// but I might later use them as indices for an array.
typedef enum { 
    OP_NONE = -1,
    OP_ADD = 0,
    OP_SUB,
    OP_LEFT,
    OP_RIGHT,
    OP_OPEN,
    OP_CLOSE,
    OP_IN,
    OP_OUT
} operation_T;

operation_T char_to_op(char character);
void select_operation(operation_T current_op, segment_T * program, stack_T * address_stack, int param1, int param2);
int is_op_countable(operation_T current_op);


// Write operation functions
void write_setup(segment_T * program, stack_T * address_stack, int unused1, int unused2);
void write_add(segment_T * program, stack_T * address_stack, int count, int unused);
void write_sub(segment_T * program, stack_T * address_stack, int count, int unused);
void write_right(segment_T * program, stack_T * address_stack, int count, int unused);
void write_left(segment_T * program, stack_T * address_stack, int count, int unused);
void write_zero(segment_T * program, stack_T * address_stack, int unused1, int unused2);
void write_out(segment_T * program, stack_T * address_stack, int unused1, int unused2);
void write_in(segment_T * program, stack_T * address_stack, int unused1, int unused2);
void write_open(segment_T * program, stack_T * address_stack, int unused1, int unused2);
void write_close(segment_T * program, stack_T * address_stack, int unused1, int unused2);
void write_end(segment_T * program, stack_T * address_stack, int unused1, int unused2);