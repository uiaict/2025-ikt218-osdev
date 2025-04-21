#include "arch/gdt.h"
#include "printf.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "arch/idt.h"
#include "arch/isr.h"
#include "arch/irq.h"



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

void putc_raw(char c) {
    volatile char* video = (volatile char*)(0xB8000 + 160 * 23); // linje 24
    video[0] = c;
    video[1] = 0x07;
}


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    putc_raw('Z');       // Debugmarkør
    idt_init();          // Last IDT
    isr_install();       // <-- Denne MÅ inn for å registrere alle ISR-ene
    irq_install();        // <-- legg til denne!
    gdt_init();          // Last GDT
    printf("Hello, Nils!\n");
    putc_raw('T');     // <- denne skal vises i hjørnet!
    asm("int $0x00"); // skal gi 0
    asm("int $0x2A"); // skal gi 42
    asm("int $0x20"); // IRQ0 (timer)
    asm("int $0x21"); // IRQ1 (keyboard)

    return 0;
}
