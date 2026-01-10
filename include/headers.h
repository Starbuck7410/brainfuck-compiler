#pragma once

#include <elf.h> 
#include <stdlib.h>

#define VADDR_START 0x400000
#define EHDR_SIZE sizeof(Elf64_Ehdr)
#define PHDR_SIZE sizeof(Elf64_Phdr)
#define PAGE_SIZE 0x1000
#define MAX_PROGRAMS 5

typedef struct {
    Elf64_Off phdr_offset;
    Elf64_Xword phdr_size;
    Elf64_Xword program_offset;
    Elf64_Xword program_size;
    Elf64_Xword data_offset;
    Elf64_Xword data_size;
} offsets_T;

Elf64_Ehdr generate_ehdr(Elf64_Half phdr_count);
Elf64_Phdr generate_phdr(Elf64_Off offset, Elf64_Xword size, Elf64_Word flags);
Elf64_Phdr generate_tape(Elf64_Off offset, Elf64_Xword size, Elf64_Word flags);