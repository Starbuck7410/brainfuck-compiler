// MASSIVE thank you to Felix (if that is his name),
// the guy behind this awesome website:
// https://www.felixcloutier.com/x86/

#define MOV(reg) (0xb8 + reg) // B8+ rw iw
#define SYSCALL (0x0f05)

// System calls

#define SYS_WRITE 1
#define SYS_EXIT 60

typedef enum {
    INST_MOV,
    INST_SYSCALL
} instruction_T;