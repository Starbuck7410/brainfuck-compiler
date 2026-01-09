#include "../include/headers.h"

Elf64_Ehdr generate_ehdr(Elf64_Half phdr_count){ 
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
        .e_entry = VADDR_START + EHDR_SIZE+ phdr_count * PHDR_SIZE,
        .e_phoff = EHDR_SIZE,
        .e_shoff = 0,
        .e_flags = 0,
        .e_ehsize = EHDR_SIZE, 
        .e_phentsize = PHDR_SIZE, 
        .e_phnum = phdr_count,
        .e_shentsize = 0,
        .e_shnum = 0,
        .e_shstrndx = SHN_UNDEF,
        .e_version = EV_CURRENT
    };
    return executable_header;
}

Elf64_Phdr generate_phdr(Elf64_Off offset, Elf64_Xword size, Elf64_Word flags){
    Elf64_Phdr program_header = {
        .p_type = PT_LOAD,
        .p_offset = offset,
        .p_vaddr = VADDR_START + offset,
        .p_paddr = offset,
        .p_filesz = size, 
        .p_memsz = size, 
        .p_flags = flags,
        .p_align = PAGE_SIZE
    };
    return program_header;
}
 
    
