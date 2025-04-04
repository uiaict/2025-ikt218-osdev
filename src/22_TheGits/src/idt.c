
#include "libc/idt.h"



#define IDT_SIZE 256

// IDT-tabellen og IDT-peker
struct idt_entry idt[IDT_SIZE];
struct idt_ptr idt_descriptor;

// Funksjon for å sette en IDT-oppføring
void set_idt_entry(int index, uint32_t isr, uint16_t selector, uint8_t type_attr) {
    idt[index].offset_low = isr & 0xFFFF;
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].type_attr = type_attr;
    idt[index].offset_high = (isr >> 16) & 0xFFFF;
}

extern void isr_timer();
extern void isr_keyboard();
extern void isr_syscall();

void init_idt() {
    idt_descriptor.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idt_descriptor.base = (uint32_t)&idt;

    // Sett alle oppføringer til default handler
    for (int i = 0; i < IDT_SIZE; i++) {
        set_idt_entry(i, (uint32_t)default_int_handler, 0x08, 0x8E);
    }

    // Sett opp ISR-er
    set_idt_entry(0x20, (uint32_t)isr_timer, 0x08, 0x8E);    // IRQ0: Timer
    set_idt_entry(0x21, (uint32_t)isr_keyboard, 0x08, 0x8E); // IRQ1: Tastatur
    set_idt_entry(0x80, (uint32_t)isr_syscall, 0x08, 0xEE);  // INT 0x80: Systemkall

    // Last IDT
    __asm__ volatile ("lidt (%0)" : : "r"(&idt_descriptor));
}


void remap_pic() {
    outb(0x20, 0x11); // Init master PIC
    outb(0xA0, 0x11); // Init slave PIC
    outb(0x21, 0x20); // Remap master til 0x20-0x27
    outb(0xA1, 0x28); // Remap slave til 0x28-0x2F
    outb(0x21, 0x04); // Cascade
    outb(0xA1, 0x02); // Cascade
    outb(0x21, 0x01); // 8086 mode
    outb(0xA1, 0x01); // 8086 mode
    outb(0x21, 0x0);  // Enable all IRQs on master
    outb(0xA1, 0x0);  // Enable all IRQs on slave
}

void send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(0xA0, 0x20); // EOI til slave PIC
    }
    outb(0x20, 0x20); // EOI til master PIC
}

void default_int_handler() {
    printf("Unhandled interrupt triggered!\n");
    while (1) {
        __asm__ volatile ("hlt"); // Stopp systemet
    }
}


