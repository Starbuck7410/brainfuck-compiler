#include <stdlib.h>
#include <stdio.h>
#include "../include/opcodes.h"
#include "../include/registers.h"
#include "../include/instructions.h" 
#include "../include/headers.h" 
#include "../include/stack.h"

// #define ALIGN16(number) (((number) + 15) & ~15)
#define PROGRAM_PAGES 4
#define TAPE_PAGES 4

#define BUFFER_OUTPUT (0x401000 + ((PAGE_SIZE) * (PROGRAM_PAGES)))
#define BUFFER_TAPE (0x402000 + ((PAGE_SIZE) * (PROGRAM_PAGES)) + ((TAPE_PAGES * (PAGE_SIZE / 2))))
#define TAPE_VAL EBX
#define TAPE_PTR ESP


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
    int jump_distance = 0;
    int source_address = 0;
    // Setup code
    write_instruction(&program, INST_SET_IMM, TAPE_PTR, BUFFER_TAPE);
    write_instruction(&program, INST_SET_IMM, TAPE_VAL, 0);
    
    int current_char = 0;
    while((current_char = fgetc(input_file)) != EOF){
        switch (current_char){
            case '+':
            write_instruction(&program, INST_LOAD_REG, TAPE_VAL, TAPE_PTR);
            write_instruction(&program, INST_ADD_IMM, TAPE_VAL, 1); 
            write_instruction(&program, INST_STORE_REG, TAPE_PTR, TAPE_VAL);
            break;

            case '-':
            write_instruction(&program, INST_LOAD_REG, TAPE_VAL, TAPE_PTR);
            write_instruction(&program, INST_SUB_IMM, TAPE_VAL, 1); 
            write_instruction(&program, INST_STORE_REG, TAPE_PTR, TAPE_VAL);
            break;

            case '>':
            write_instruction(&program, INST_ADD_IMM, TAPE_PTR, 1); 
            break;

            case '<':
            write_instruction(&program, INST_SUB_IMM, TAPE_PTR, 1); 
            break;

            case '.':
            write_instruction(&program, INST_LOAD_REG, TAPE_VAL, TAPE_PTR);
            write_instruction(&program, INST_STORE_B_IMM, BUFFER_OUTPUT, EBX);
            write_instruction(&program, INST_SET_IMM, TAPE_VAL, 0);
            write_instruction(&program, INST_STORE_B_IMM, BUFFER_OUTPUT + 1, TAPE_VAL);

            write_instruction(&program, INST_SET_IMM, ESI, BUFFER_OUTPUT);
            write_instruction(&program, INST_SET_IMM, EAX, SYS_WRITE);
            write_instruction(&program, INST_SET_IMM, EDI, FD_STDOUT);
            write_instruction(&program, INST_SET_IMM, EDX, 1);
            write_instruction(&program, INST_SYSCALL, 0, 0);
            break;
            
            case ',':
            write_instruction(&program, INST_SET_IMM, EAX, SYS_READ);
            write_instruction(&program, INST_SET_IMM, EDI, FD_STDIN);
            write_instruction(&program, INST_SET_REG, ESI, TAPE_PTR);
            write_instruction(&program, INST_SET_IMM, EDX, 1);
            write_instruction(&program, INST_SYSCALL, 0, 0); 
            break;
            
            case '[':
            write_instruction(&program, INST_LOAD_REG, TAPE_VAL, TAPE_PTR);
            write_instruction(&program, INST_CMP, TAPE_VAL, 0);
            write_instruction(&program, INST_JZ, 0, 0); // Do nothing unless overwritten (Might be tweaked later)
            stack_push(&address_stack, program.head); // Stores the location of the back jump
            break;

            case ']':
            source_address = stack_pop(&address_stack);
            write_instruction(&program, INST_LOAD_REG, TAPE_VAL, TAPE_PTR);
            write_instruction(&program, INST_CMP, TAPE_VAL, 0);
            write_instruction(&program, INST_JNZ, 0, 0); 
            
            jump_distance = program.head - source_address;
            overwrite_jump(&program, source_address - 4, jump_distance);
            overwrite_jump(&program, program.head - 4, - jump_distance);
            break;

            default:
            break;
        }
    }
    
    // Write \n to output buffer to flush STDOUT
    write_instruction(&program, INST_SET_IMM, EBX, '\n');
    write_instruction(&program, INST_STORE_B_IMM, BUFFER_OUTPUT, EBX);
    write_instruction(&program, INST_SET_IMM, EAX, 0);
    write_instruction(&program, INST_STORE_B_IMM, BUFFER_OUTPUT + 1, EBX);

    // Flush
    write_instruction(&program, INST_SET_IMM, ESI, BUFFER_OUTPUT);
    write_instruction(&program, INST_SET_IMM, EAX, SYS_WRITE);
    write_instruction(&program, INST_SET_IMM, EDI, FD_STDOUT);
    write_instruction(&program, INST_SET_IMM, EDX, 1);
    write_instruction(&program, INST_SYSCALL, 0, 0);

    // Exit with return code 0
    write_instruction(&program, INST_SUB_REG, EDI, EDI);  
    write_instruction(&program, INST_SET_IMM, EAX, SYS_EXIT);
    write_instruction(&program, INST_SYSCALL, 0, 0);    

    
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
