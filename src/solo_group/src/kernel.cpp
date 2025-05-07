extern "C"{
    #include "interupts.h"
    #include "libc/stdio.h"
    #include "libc/stddef.h"
    #include "common.h"
    #include "input.h"
}

//TODO: Første bokstav forsvinner igjen
extern "C" int kernel_main(void);
int kernel_main(){
    registerInterruptHandler(ISR1, [](registers_t* regs, void* context) {
        printf("Interrupt 1 - OK\n");
    }, NULL);
    registerInterruptHandler(ISR2, [](registers_t* regs, void* context) {
        printf("Interrupt 2 - OK\n");
    }, NULL);
    registerInterruptHandler(ISR3, [](registers_t* regs, void* context) {
        printf("Interrupt 3 - OK\n");
    }, NULL);

    printf("ABCDEFG\n");

    asm volatile ("int $0x1");
    asm volatile ("int $0x2");
    asm volatile ("int $0x3");

    asm volatile("sti");

    registerIrqHandler(IRQ1, [](registers_t*, void*) {
        // Read from keyboard
        uint8_t scancode = inb(0x60);
        char c = scancodeToAscii(&scancode);
        if (c != 0)
            printf("%c", c);

        outb(0x20, 0x20); // EOI to master PIC        
    }, NULL);

    
    while (true){}
    return 0;
}