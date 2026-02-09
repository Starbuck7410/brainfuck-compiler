#include <stdlib.h>
#include <stdio.h>
#include "../include/opcodes.h"
#include "../include/registers.h"
#include "../include/instructions.h" 
#include "../include/headers.h" 
#include "../include/stack.h"
#include "../include/operations.h"


int main(int argc, char ** argv){
    if(argc != 3){
        printf("Usage: bfc <source_file> <output_binary>\n");
        return -1;
    }

    segment_T program = {
        .head = 0,
        .buffer = malloc(PROGRAM_PAGES * PAGE_SIZE)
    };
    segment_T data = {
        .head = 0,
        .buffer = malloc(PAGE_SIZE)
    };

    stack_T address_stack = stack_init();
    char * zeroes = malloc(PAGE_SIZE);
    FILE * input_file = fopen(argv[1], "r");
    // Setup code

    write_setup(&program, &address_stack, 0, 0);
    int current_char = 0;
    int count = 0;
    operation_T last_op = OP_NONE;
    while((current_char = fgetc(input_file)) != EOF){
        operation_T current_op = char_to_op(current_char);
        if(is_op_countable(current_op) && current_op == last_op){
            count++;
            continue;
        } 

        if(is_op_countable(last_op)){
            printf("Param1: %d |\t", count);
            select_operation(last_op, &program, &address_stack, count, 0);
        }
        count = 1;
        if(!is_op_countable(current_op)){
            select_operation(current_op, &program, &address_stack, count, 0);
        }
        last_op = current_op;
    }
    if(is_op_countable(last_op)){
        select_operation(last_op, &program, &address_stack, count, 0);
    }
    printf("\n");
    write_end(&program, &address_stack, 0, 0);
        

    
    Elf64_Ehdr executable_header = generate_ehdr(4);
    Elf64_Phdr ehdr_header = generate_phdr(0, PAGE_SIZE, PF_R);
    Elf64_Phdr program_header = generate_phdr(PAGE_SIZE, PROGRAM_PAGES* PAGE_SIZE, PF_X | PF_R);
    Elf64_Phdr data_header = generate_phdr((1 + PROGRAM_PAGES) * PAGE_SIZE, PAGE_SIZE, PF_W | PF_R);
    Elf64_Phdr tape_header = generate_tape((2 + PROGRAM_PAGES) * PAGE_SIZE, TAPE_PAGES * PAGE_SIZE, PF_W | PF_R);

    FILE * output_file = fopen(argv[2], "w");
    fwrite(&executable_header, 1, EHDR_SIZE, output_file);
    fwrite(&ehdr_header, 1, PHDR_SIZE, output_file);
    fwrite(&program_header, 1, PHDR_SIZE, output_file);
    fwrite(&data_header, 1, PHDR_SIZE, output_file);
    fwrite(&tape_header, 1, PHDR_SIZE, output_file);
    fwrite(zeroes, 1, PAGE_SIZE - 4 * PHDR_SIZE - EHDR_SIZE, output_file);
    fwrite(program.buffer, PROGRAM_PAGES * PAGE_SIZE, 1, output_file);
    fwrite(data.buffer, PAGE_SIZE, 1, output_file);


    stack_destroy(&address_stack);
    fclose(output_file);
    free(program.buffer);
    free(data.buffer);
    free(zeroes);
    return 0;
}


