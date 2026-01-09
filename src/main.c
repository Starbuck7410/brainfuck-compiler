#include <stdlib.h>
#include <stdio.h>
#include "../include/opcodes.h"
#include "../include/registers.h"
#include "../include/instructions.h" 
#include "../include/headers.h" 

#define ALIGN16(number) (((number) + 15) & ~15)

#define MAX_PROGRAM PAGE_SIZE


int main(){
    segment_T program = {
        .head = 0,
        .buffer = malloc(MAX_PROGRAM)
    };
    segment_T data = {
        .head = 0,
        .buffer = malloc(MAX_PROGRAM)
    };
    
    char * message = "https://www.youtube.com/watch?v=AXwGVXD7qEQ\n";
    
    write_instruction(&program, INST_MOV, EAX, SYS_WRITE);
    write_instruction(&program, INST_MOV, EDI, 1);
    write_instruction(&program, INST_MOV, EDX, strlen(message));
    write_instruction(&program, INST_MOV, ESI, 0x4010b0);
    write_instruction(&program, INST_SYSCALL, 0, 0);  

    write_instruction(&program, INST_MOV, EDI, 0);
    write_instruction(&program, INST_MOV, EAX, SYS_EXIT);
    write_instruction(&program, INST_SYSCALL, 0, 0);    
    write_data(&data, message, strlen(message));

    program.head = PAGE_SIZE;
    data.head = strlen(message);
    Elf64_Word program_offset = EHDR_SIZE + 2 * PHDR_SIZE;
    Elf64_Word data_offset = program.head + program_offset;
    // printf("Program offset: %u, Data offset: %u\n", program_offset, data_offset);

    Elf64_Ehdr executable_header = generate_ehdr(2);
    Elf64_Phdr program_header = generate_phdr(program_offset, program.head, PF_X | PF_R);
    Elf64_Phdr data_header = generate_phdr(data_offset, data.head, PF_W | PF_R);



    FILE * output_file = fopen("output", "w");
    fwrite(&executable_header, 1, EHDR_SIZE, output_file);
    fwrite(&program_header, 1, PHDR_SIZE, output_file);
    fwrite(&data_header, 1, PHDR_SIZE, output_file);
    fwrite(program.buffer, program.head, 1, output_file);
    fwrite(data.buffer, strlen(message), 1, output_file);
    fclose(output_file);
    free(program.buffer);
    return 0;
}