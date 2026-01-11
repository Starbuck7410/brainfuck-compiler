#pragma once

// MASSIVE thank you to Felix (if that is his name),
// the guy behind this awesome website:
// https://www.felixcloutier.com/x86/

#define SET(reg) (0xb8 + reg) // B8+ rw iw
#define STORE 0x89
#define STORE_B 0x88
#define LOAD 0x8b
#define SYSCALL (0x0f05)

typedef enum {
    INST_SET,           // set dest imm
    INST_STORE_IMM,     // store_i imm src
    INST_LOAD_IMM,      // load_i dest imm
    INST_STORE_REG,     // store_r [dest] src
    INST_LOAD_REG,      // load_r dest [src]
    INST_STORE_B_IMM,     // storeb_i imm src
    INST_SYSCALL        // syscall 0 0
} instruction_T;

// System calls

#define SYS_WRITE 1
#define SYS_EXIT 60