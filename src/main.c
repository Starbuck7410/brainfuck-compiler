#include <stdlib.h>
#include <stdio.h>
#include "../include/opcodes.h"
#include "../include/registers.h"
#include "../include/instructions.h" 
#include "../include/headers.h" 

// #define ALIGN16(number) (((number) + 15) & ~15)
#define PROGRAM_PAGES 4
#define TAPE_PAGES 2

#define BUFFER_OUTPUT (0x401000 + ((PAGE_SIZE) * (PROGRAM_PAGES)))
#define BUFFER_TAPE (0x402000 + ((PAGE_SIZE) * (PROGRAM_PAGES)) + ((TAPE_PAGES * (PAGE_SIZE / 2))))



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
    char * zeroes = malloc(PAGE_SIZE);
    FILE * input_file = fopen(argv[1], "r");
    
    // Setup code
    write_instruction(&program, INST_SET_IMM, ESP, BUFFER_TAPE);
    write_instruction(&program, INST_SET_IMM, EBX, 0);
    
    int current_char = 0;
    while((current_char = fgetc(input_file)) != EOF){
        switch (current_char){
            case '+':
                write_instruction(&program, INST_LOAD_REG, EBX, ESP);
                write_instruction(&program, INST_ADD_IMM, EBX, 1); 
                write_instruction(&program, INST_STORE_REG, EBX, ESP);
                break;
            case '-':
                write_instruction(&program, INST_LOAD_REG, EBX, ESP);
                write_instruction(&program, INST_SUB_IMM, EBX, 1); 
                write_instruction(&program, INST_STORE_REG, EBX, ESP);
                break;

            case '>':
                write_instruction(&program, INST_ADD_IMM, ESP, 1); 
                break;
            case '<':
                write_instruction(&program, INST_SUB_IMM, ESP, 1); 
                break;
            case '.':
                write_instruction(&program, INST_LOAD_REG, EBX, ESP);
                write_instruction(&program, INST_STORE_B_IMM, BUFFER_OUTPUT, EBX);
                write_instruction(&program, INST_SET_IMM, EAX, 0);
                write_instruction(&program, INST_STORE_B_IMM, BUFFER_OUTPUT + 1, EBX);

                write_instruction(&program, INST_SET_IMM, ESI, BUFFER_OUTPUT);
                write_instruction(&program, INST_SET_IMM, EAX, SYS_WRITE);
                write_instruction(&program, INST_SET_IMM, EDI, FD_STDOUT);
                write_instruction(&program, INST_SET_IMM, EDX, 1);
                write_instruction(&program, INST_SYSCALL, 0, 0);
                break;
            
            case ',':
                write_instruction(&program, INST_SET_IMM, EAX, SYS_READ);
                write_instruction(&program, INST_SET_IMM, EDI, FD_STDIN);
                write_instruction(&program, INST_SET_REG, ESI, ESP);
                write_instruction(&program, INST_SET_IMM, EDX, 1);
                write_instruction(&program, INST_SYSCALL, 0, 0); 
                break;

            default:
                break;
        }
    }
    
    
    // // write_instruction(&program, INST_SET_IMM, EAX, '4');
    // // write_instruction(&program, INST_STORE_B_IMM, 0x402023, EAX);
    // write_instruction(&program, INST_SET_IMM, ESI, BUFFER_OUTPUT);
    // write_instruction(&program, INST_SET_IMM, EAX, SYS_WRITE);
    // write_instruction(&program, INST_SET_IMM, EDI, FD_STDOUT);
    // write_instruction(&program, INST_SET_IMM, EDX, 2);
    // write_instruction(&program, INST_SYSCALL, 0, 0);  
    
    // // write_instruction(&program, INST_SET_IMM, EDI, 0);
    // write_instruction(&program, INST_SUB_REG, EDI, EDI);  
    // write_instruction(&program, INST_LOAD_IMM, EDI, BUFFER_OUTPUT);
    write_instruction(&program, INST_SET_IMM, EAX, SYS_EXIT);
    write_instruction(&program, INST_SYSCALL, 0, 0);    
    // write_data(&data, message, PAGE_SIZE);
    
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


    fclose(output_file);
    free(program.buffer);
    free(data.buffer);
    free(zeroes);
    return 0;
}
