#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../include/opcodes.h"

#define MAX_INSTRUCTION 10

typedef struct program_T{
    size_t head;
    char * buffer;
} program_T;

typedef union inst_buffer_T {
    int32_t integer;
    char bytes[4];
} inst_buffer_T;


size_t write_instruction(program_T * program, instruction_T instruction, int32_t param0, int32_t param1);
size_t write_data(program_T * program, char * data, int32_t count);