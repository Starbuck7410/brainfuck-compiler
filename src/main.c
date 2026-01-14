#include <stdlib.h>
#include <stdio.h>
#include "../include/opcodes.h"
#include "../include/registers.h"
#include "../include/instructions.h" 
#include "../include/headers.h" 

#define ALIGN16(number) (((number) + 15) & ~15)




int main(int argc, char ** argv){
    if(argc != 3){
        printf("Usage: bfc <source_file> <output_binary>\n");
        return -1;
    }

    segment_T program = {
        .head = 0,
        .buffer = malloc(PAGE_SIZE)
    };
    segment_T data = {
        .head = 0,
        .buffer = malloc(PAGE_SIZE)
    };
    char * zeroes = malloc(PAGE_SIZE);
    char * message = "https://www.youtube.com/watch?v=dQw?w9WgXcQ\n";
    
    write_instruction(&program, INST_SET, EAX, SYS_READ);
    write_instruction(&program, INST_SET, EDI, FD_STDIN);
    write_instruction(&program, INST_SET, ESI, 0x402000);
    write_instruction(&program, INST_SET, EDX, 1);
    write_instruction(&program, INST_SYSCALL, 0, 0);  
    
    // write_instruction(&program, INST_SET, EAX, '4');
    // write_instruction(&program, INST_STORE_B_IMM, 0x402023, EAX);
    write_instruction(&program, INST_SET, ESI, 0x402000);
    write_instruction(&program, INST_SET, EAX, SYS_WRITE);
    write_instruction(&program, INST_SET, EDI, FD_STDOUT);
    write_instruction(&program, INST_SET, EDX, strlen(message));
    write_instruction(&program, INST_SYSCALL, 0, 0);  
    
    // write_instruction(&program, INST_SET, EDI, 0);
    write_instruction(&program, INST_LOAD_IMM, EDI, 0x402000);
    write_instruction(&program, INST_SUB_REG, EDI, EDI);  
    write_instruction(&program, INST_SET, EAX, SYS_EXIT);
    write_instruction(&program, INST_SYSCALL, 0, 0);    
    write_data(&data, message, PAGE_SIZE);
    
    Elf64_Ehdr executable_header = generate_ehdr(4);
    Elf64_Phdr ehdr_header = generate_phdr(0, PAGE_SIZE, PF_R);
    Elf64_Phdr program_header = generate_phdr(PAGE_SIZE, PAGE_SIZE, PF_X | PF_R);
    Elf64_Phdr data_header = generate_phdr(2 * PAGE_SIZE, PAGE_SIZE, PF_W | PF_R);
    Elf64_Phdr tape_header = generate_tape(3 * PAGE_SIZE, PAGE_SIZE, PF_W | PF_R);

    FILE * output_file = fopen(argv[2], "w");
    fwrite(&executable_header, 1, EHDR_SIZE, output_file);
    fwrite(&ehdr_header, 1, PHDR_SIZE, output_file);
    fwrite(&program_header, 1, PHDR_SIZE, output_file);
    fwrite(&data_header, 1, PHDR_SIZE, output_file);
    fwrite(&tape_header, 1, PHDR_SIZE, output_file);
    fwrite(zeroes, 1, PAGE_SIZE - 4 * PHDR_SIZE - EHDR_SIZE, output_file);
    fwrite(program.buffer, PAGE_SIZE, 1, output_file);
    fwrite(data.buffer, PAGE_SIZE, 1, output_file);


    fclose(output_file);
    free(program.buffer);
    free(data.buffer);
    free(zeroes);
    return 0;
}
