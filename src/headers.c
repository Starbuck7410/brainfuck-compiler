#include "../include/headers.h"




Elf64_Ehdr generate_ehdr(){ 
    Elf64_Ehdr executable_header = {
        .e_ident = {
            ELFMAG0,
            ELFMAG1,
            ELFMAG2,
            ELFMAG3,
            ELFCLASS64,
            ELFDATA2LSB,
            EV_CURRENT,
            ELFOSABI_SYSV,
            0, 0, 0, 0, 0, 0, 0
        },
        .e_type = ET_EXEC,
        .e_machine = EM_X86_64,
        .e_entry = MEM_START + EHDR_SIZE + PHDR_SIZE,
        .e_phoff = EHDR_SIZE,
        .e_shoff = 0,
        .e_flags = 0,
        .e_ehsize = EHDR_SIZE, 
        .e_phentsize = PHDR_SIZE, 
        .e_phnum = 1,
        .e_shentsize = 0,
        .e_shnum = 0,
        .e_shstrndx = SHN_UNDEF,
        .e_version = EV_CURRENT
    };
    return executable_header;
}

Elf64_Phdr generate_phdr(size_t program_size){
    Elf64_Phdr program_header = {
        .p_type = PT_LOAD,
        .p_offset = EHDR_SIZE + PHDR_SIZE,
        .p_vaddr = MEM_START + EHDR_SIZE + PHDR_SIZE,
        .p_paddr = MEM_START + EHDR_SIZE + PHDR_SIZE,
        .p_filesz = program_size, // I suspect this is the program size in bytes
        .p_memsz = program_size, // And this is the memory required to hold the program
        .p_flags = PF_X | PF_R,
        .p_align = 0x8
    };
    return program_header;
}
 
    
Elf64_Shdr generate_shdr(Elf64_Word type){ 
    Elf64_Shdr section_header = {
        .sh_type = type,
        .sh_addralign = 0x8,
        .sh_flags = SHF_WRITE,

    };
    return section_header;
}
