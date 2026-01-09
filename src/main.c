#include <stdlib.h>
#include <stdio.h>
#include "../include/opcodes.h"
#include "../include/registers.h"
#include "../include/instructions.h" 
#include "../include/headers.h" 



#define MAX_PROGRAM 1024
size_t write_instruction(FILE * file, instruction_T instruction, int32_t param0, int32_t param1);
int main(){
    // char * object_code = malloc(MAX_PROGRAM);

    FILE * output_file = fopen("output", "w");
    

    Elf64_Ehdr executable_header = generate_ehdr();
    Elf64_Phdr program_header = generate_phdr(7);
    // Elf64_Shdr section_header = generate_shdr(SHT_PROGBITS);
    fwrite(&executable_header, 1, sizeof(executable_header), output_file);
    fwrite(&program_header, 1, sizeof(program_header), output_file);
    write_instruction(output_file, INST_MOV, EAX, SYS_EXIT);
    write_instruction(output_file, INST_SYSCALL, 0, 0);
    fclose(output_file);
    // fwrite(&section_header, 1, sizeof(section_header), output_file);
    // free(object_code);
    return 0;
}