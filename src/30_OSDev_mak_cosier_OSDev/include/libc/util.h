#include "stdint.h"

// Memory function declaration
void *memset(void *dest, unsigned char val, uint32_t count);  // Change 'char' to 'unsigned char' for consistency

// I/O port functions
void outPortB(uint16_t Port, uint8_t Value);
char inPortB(uint16_t port);

// Other defines and structures
#define CEIL_DIV(a,b) (((a + b) - 1)/b)

struct InterruptRegisters {
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, csm, eflags, useresp, ss;
};



/*
void memset(void *dest, char val, uint32_t count);
void outPortB(uint16_t port, uint8_t value);
char inPortB(uint16_t port);


struct InterruptRegisters 
{
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, csm, eflags, useresp, ss;
};

*/