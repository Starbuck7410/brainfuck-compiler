#pragma once

// MASSIVE thank you to Felix (if that is his name),
// the guy behind this awesome website:
// https://www.felixcloutier.com/x86/

#define SET_IMM(reg) (0xb8 + reg) // B8+ rw iw
#define STORE 0x89
#define STORE_B 0x88
#define LOAD 0x8b
#define ADDSUB_IMM 0x81
#define ADD_REG 0x03
#define SUB_REG 0x2b
#define CMP 0x81 
#define JZ (0x0F84)
#define JNZ (0x0F85)
#define SYSCALL (0x0f05)

typedef enum {
    INST_SET_REG,           // set_reg dest src
    INST_SET_IMM,           // set_imm dest imm
    INST_LOAD_REG,          // load_r dest [src]
    INST_LOAD_IMM,          // load_i dest imm
    INST_STORE_REG,         // store_r [dest] src
    INST_STORE_IMM,         // store_i imm src
    INST_STORE_B_IMM,       // storeb_i imm src
    INST_ADD_REG,           // add dest reg
    INST_ADD_IMM,           // add dest imm
    INST_SUB_REG,           // sub dest reg
    INST_SUB_IMM,           // sub dest imm
    INST_CMP,               // cmp reg imm
    INST_JZ,                // jz imm 0
    INST_JNZ,               // jnz imm 0
    INST_SYSCALL            // syscall 0 0
} instruction_T;

// System calls

#define SYS_WRITE 1
#define SYS_READ 0
#define SYS_EXIT 60

// File descriptors
#define FD_STDIN 0
#define FD_STDOUT 1