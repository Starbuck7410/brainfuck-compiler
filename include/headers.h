#pragma once

#include <elf.h> 
#include <stdlib.h>

#define MEM_START 0x400000
#define EHDR_SIZE 64
#define PHDR_SIZE 56
Elf64_Ehdr generate_ehdr();
Elf64_Phdr generate_phdr(size_t program_size);
Elf64_Shdr generate_shdr(Elf64_Word type);