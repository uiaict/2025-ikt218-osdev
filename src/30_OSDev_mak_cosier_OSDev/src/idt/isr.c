#include "../include/libc/isr.h"

/* 
   Exception-meldinger for de første 32 avbruddene.
   Her kaster vi strengene til (unsigned char *) for å matche 
   deklarasjonen av exception_messages.
 */
static unsigned char* exception_messages[32] = 
{
    (unsigned char*)"Division By Zero",
    (unsigned char*)"Debug",
    (unsigned char*)"Non Maskable Interrupt",
    (unsigned char*)"Breakpoint",
    (unsigned char*)"Into Detected Overflow",
    (unsigned char*)"Out of Bounds",
    (unsigned char*)"Invalid Opcode",
    (unsigned char*)"No Coprocessor",
    (unsigned char*)"Double fault",
    (unsigned char*)"Coprocessor Segment Overrun",
    (unsigned char*)"Bad TSS",
    (unsigned char*)"Segment not present",
    (unsigned char*)"Stack fault",
    (unsigned char*)"General protection fault",
    (unsigned char*)"Page fault",
    (unsigned char*)"Unknown Interrupt",
    (unsigned char*)"Coprocessor Fault",
    (unsigned char*)"Alignment Fault",
    (unsigned char*)"Machine Check",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved",
    (unsigned char*)"Reserved"
};

// Denne funksjonen kalles fra vår ASM ISR stub.
void isr_handler(struct InterruptRegisters* regs)
{
    if (regs->int_no < 32) 
    {
        printf(exception_messages[regs->int_no]);
        printf(" (Interrupt Number: ");
        //print(char*(regs->int_no));
        printf(")\n");
    } else 
    {
        printf("Received interrupt: ");
        //printf(char*(regs->int_no));
        printf('\n');
    }
}
