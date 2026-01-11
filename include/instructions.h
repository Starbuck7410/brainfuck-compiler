#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../include/opcodes.h"

#define MAX_INSTRUCTION 10

typedef struct segment_T{
    size_t head;
    char * buffer;
} segment_T;

typedef union inst_buffer_T {
    int32_t integer;
    char bytes[4];
} inst_buffer_T;


size_t write_instruction(segment_T * program, instruction_T instruction, int32_t param0, int32_t param1);
size_t write_data(segment_T * program, char * data, int32_t count);
int32_t mod_rm(int mod, int reg, int rm);
int32_t sib_byte(int scale, int index, int base);