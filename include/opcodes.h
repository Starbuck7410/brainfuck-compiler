#pragma once

// MASSIVE thank you to Felix (if that is his name),
// the guy behind this awesome website:
// https://www.felixcloutier.com/x86/

#define MOV(reg) (0xb8 + reg) // B8+ rw iw
#define LOAD 0xa1
#define STORE 0xa3
#define SYSCALL (0x0f05)

typedef enum {
    INST_MOV,
    INST_LOAD,
    INST_STORE,
    INST_SYSCALL
} instruction_T;

// System calls

#define SYS_WRITE 1
#define SYS_EXIT 60