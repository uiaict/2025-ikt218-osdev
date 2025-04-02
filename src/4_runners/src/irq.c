#include "libc/stdint.h"
#include "idt.h"
#include "irq.h"

#define KEYBOARD_DATA_PORT 0x60
#define PIC1_COMMAND 0x20


// External IRQ stubs from irq_stubs.asm
extern void irq0_stub(void);
extern void irq1_stub(void);
extern void irq2_stub(void);
extern void irq3_stub(void);
extern void irq4_stub(void);
extern void irq5_stub(void);
extern void irq6_stub(void);
extern void irq7_stub(void);
extern void irq8_stub(void);
extern void irq9_stub(void);
extern void irq10_stub(void);
extern void irq11_stub(void);
extern void irq12_stub(void);
extern void irq13_stub(void);
extern void irq14_stub(void);
extern void irq15_stub(void);

void terminal_write(const char* str);
void terminal_put_char(char c);

// I/O port functions
static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Simple US QWERTY scancode to ASCII mapping
static char scancode_to_ascii[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // Backspace
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',   // Enter
    0,   // Control
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/', 0, '*',
    0,  ' ', // Space
    // Rest are zero or extended keys
};

// IRQ1 handler for keyboard input
void irq1_handler(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    // Ignore key releases (scancodes >= 0x80)
    if (scancode & 0x80) {
        outb(PIC1_COMMAND, 0x20); // Send EOI
        return;
    }

    // Convert scancode to ASCII
    if (scancode < sizeof(scancode_to_ascii)) {
        char ascii = scancode_to_ascii[scancode];
        if (ascii) {
            terminal_put_char(ascii);
        }
    }

    // Send End Of Interrupt to PIC
    outb(PIC1_COMMAND, 0x20);
}


// Remap the PIC
void pic_remap() {
    outb(0x20, 0x11); // Master command
    outb(0xA0, 0x11); // Slave command

    outb(0x21, 0x20); // Master offset 0x20
    outb(0xA1, 0x28); // Slave offset 0x28

    outb(0x21, 0x04); // Tell Master about Slave at IRQ2
    outb(0xA1, 0x02); // Tell Slave its cascade ID

    outb(0x21, 0x01); // 8086 mode
    outb(0xA1, 0x01);

    outb(0x21, 0x00); // Unmask master
    outb(0xA1, 0x00); // Unmask slave
}

// Initialize IRQs and assign stubs to IDT
void irq_init(void) {
    pic_remap();

    set_idt_entry(32, (uint32_t)irq0_stub, 0x08, 0x8E);
    set_idt_entry(33, (uint32_t)irq1_stub, 0x08, 0x8E);
    set_idt_entry(34, (uint32_t)irq2_stub, 0x08, 0x8E);
    set_idt_entry(35, (uint32_t)irq3_stub, 0x08, 0x8E);
    set_idt_entry(36, (uint32_t)irq4_stub, 0x08, 0x8E);
    set_idt_entry(37, (uint32_t)irq5_stub, 0x08, 0x8E);
    set_idt_entry(38, (uint32_t)irq6_stub, 0x08, 0x8E);
    set_idt_entry(39, (uint32_t)irq7_stub, 0x08, 0x8E);
    set_idt_entry(40, (uint32_t)irq8_stub, 0x08, 0x8E);
    set_idt_entry(41, (uint32_t)irq9_stub, 0x08, 0x8E);
    set_idt_entry(42, (uint32_t)irq10_stub, 0x08, 0x8E);
    set_idt_entry(43, (uint32_t)irq11_stub, 0x08, 0x8E);
    set_idt_entry(44, (uint32_t)irq12_stub, 0x08, 0x8E);
    set_idt_entry(45, (uint32_t)irq13_stub, 0x08, 0x8E);
    set_idt_entry(46, (uint32_t)irq14_stub, 0x08, 0x8E);
    set_idt_entry(47, (uint32_t)irq15_stub, 0x08, 0x8E);
}
