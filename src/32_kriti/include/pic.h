#ifndef PIC_H
#define PIC_H

#include <libc/stdint.h>

// PIC ports
#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1

// PIC commands
#define PIC_EOI         0x20    // End of Interrupt

// Initialization Command Words
#define ICW1_INIT       0x10    // Initialization
#define ICW1_ICW4       0x01    // ICW4 needed
#define ICW4_8086       0x01    // 8086/88 mode

// Offsets for remapping
#define PIC1_OFFSET     0x20    // IRQ 0-7: int 0x20-0x27
#define PIC2_OFFSET     0x28    // IRQ 8-15: int 0x28-0x2F

// I/O port functions
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

// PIC functions
void pic_init(void);                // Initialize and remap PIC
void pic_send_eoi(uint8_t irq);     // Send End-of-Interrupt
void pic_set_mask(uint8_t irq);     // Disable specific IRQ
void pic_clear_mask(uint8_t irq);   // Enable specific IRQ
void pic_disable(void);             // Disable PIC (useful when using APIC)

#endif /* PIC_H */