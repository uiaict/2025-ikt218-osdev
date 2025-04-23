#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "libc/string.h"
#include "libc/gdt.h"
#include "libc/terminal.h"
#include "libc/printf.h"
#include "libc/idt.h"
#include "libc/irq.h"


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};



__attribute__((noreturn)) void exception_handler(uint32_t int_number) {
    if ((int)int_number >= 32 && (int)int_number <= 47) {
        // IRQs 0–15
        uint8_t irq = (int)int_number - 32;
        printf("IRQ %d received\n", irq);
        irq_acknowledge(irq);

        return; // Return instead of halting for IRQs
    } else {
        printf("Exception: interrupt %d\n", (int)int_number);
        __asm__ volatile("cli; hlt"); // Only halt for exceptions
        __builtin_unreachable();
    }
}



int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    idt_init();
    int noop = 0;
    init_gdt();
    
    printf("Hello World\n");
    printf("Number: %d\n", 42);
    printf("Hex: %x\n", 255);

    asm volatile("int $0x20"); // IRQ0 - timer
    asm volatile("int $0x21"); // IRQ1 - keyboard
    asm volatile("int $0x22"); // IRQ2 - cascade
    asm volatile("int $0x23"); // IRQ3 - COM2
    //asm("int $0x0");
    asm("int $0x1");
    asm("int $0x2");
    printf("Character: %c\n", 'A');
    printf("String: %s\n", "Kernel Booted");
    
    return 0;

}
