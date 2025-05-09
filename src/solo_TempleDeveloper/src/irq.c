#include "irq.h"
#include "idt.h"
#include "libc/stdio.h"
#include "libc/stdint.h"
#include "pit.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1
#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

// Extern stubs defined in irq_asm.asm
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

// Array of IRQ stub pointers
void (*irq_stubs[16])(void) = {
    irq0,  irq1,  irq2,  irq3,
    irq4,  irq5,  irq6,  irq7,
    irq8,  irq9,  irq10, irq11,
    irq12, irq13, irq14, irq15
};

// Write to an I/O port
static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

// Remap PIC and install IRQ stubs into the IDT
void irq_install(void) {
    // Mask all IRQs (this turns them off)
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);

    // Start init sequence (ICW1)
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // Set vector offsets (ICW2)
    outb(PIC1_DATA, 0x20);  // IRQ0–7  → IDT 32–39
    outb(PIC2_DATA, 0x28);  // IRQ8–15 → IDT 40–47

    // Tell masters/slaves about their cascading (ICW3)
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);

    // Set mode (ICW4)
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // Unmask only IRQ0 (PIT) and IRQ1 (keyboard), keep others masked
    outb(PIC1_DATA, ~(1 << 0 | 1 << 1)); // Unmask IRQ0 og IRQ1 (0xFC)
    outb(PIC2_DATA, 0xFF);

    // Install handlers for IRQ0–IRQ15 at IDT entries 32–47
    for (int i = 0; i < 16; i++) {
        idt_set_gate(32 + i, (uint32_t)irq_stubs[i], 0x08, 0x8E);
    }
}

// Common IRQ handler: send EOI and print
void irq_handler(int irq_number) {
    if (irq_number == 0) {
        pit_tick();  // track PIT ticks
    }
    
    if (irq_number >= 8) outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);
    //printf("Handled IRQ %d\n", irq_number);
}

// Simple scancode→ASCII map
static const char scancode_ascii[128] = {
    0,   27, '1','2','3','4','5','6','7','8','9','0','+','\\','\b',
   '\t','q','w','e','r','t','y','u','i','o','p','å','¨','\n', 0,
    'a','s','d','f','g','h','j','k','l','ø','æ','\'', 0,'|','z',
    'x','c','v','b','n','m',',','.','-',' ', 0, 0,   0,   0,  0,
    0,   0,  0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,  0,
    0,   0,  0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,  0,
    0,   0,  0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,  0,
    0,   0,  0,  0,  0,  0,   0,   0
};



// Keyboard handler for IRQ1: read scancode and print character
void keyboard_handler(void) {
    uint8_t scancode;
    __asm__ volatile ("inb $0x60, %0" : "=a"(scancode));

    // Ignore break codes
    if (scancode & 0x80) {
        outb(PIC1_COMMAND, 0x20);
        return;
    }

    char c = scancode_ascii[scancode];
    if (c) putchar(c);

    // Send EOI after handling keyboard
    outb(PIC1_COMMAND, 0x20);
}

