#pragma once

// Since ESP and EBX are not used in syscalls and things we can use them for the tape pointer and value registers


#define AL 0
#define AX 0
#define EAX 0
#define RAX 0

#define CL 1
#define CX 1
#define ECX 1
#define RCX 1

#define DL 2
#define DX 2
#define EDX 2
#define RDX 2

#define BL 3
#define BX 3
#define EBX 3
#define RBX 3

#define AH 4
#define SP 4
#define ESP 4
#define RSP 4

#define CH 5
#define BP 5
#define EBP 5
#define RBP 5

#define DH 6
#define SI 6
#define ESI 6
#define RSI 6

#define BH 7
#define DI 7
#define EDI 7
#define RDI 7
