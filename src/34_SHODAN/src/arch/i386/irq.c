#include "irq.h"
#include "port_io.h"
#include "terminal.h"

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

static const char scancode_ascii[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ',
};

void irq_remap(void) {
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, 0xFD); // Unmask IRQ1 (keyboard)
    outb(PIC2_DATA, 0xFF); // Mask all on slave
}

__attribute__((visibility("default")))
void irq_handler(irq_regs_t* regs) {
    terminal_write("IRQ ");
    terminal_putint(regs->int_no);
    terminal_write(" handler invoked\n");

    if (regs->int_no == 33) { // IRQ1 = keyboard
        uint8_t scancode = inb(0x60);
        if (scancode < 128) {
            char c = scancode_ascii[scancode];
            if (c) terminal_putchar(c);
        }
    }

    if (regs->int_no >= 40) {
        outb(PIC2_COMMAND, 0x20);
    }
    outb(PIC1_COMMAND, 0x20);
}
