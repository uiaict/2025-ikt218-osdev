#include "terminal/print.h"
#include "interrupts/isr.h"
#include "libc/io.h"
#include "interrupts/keyboard/keyboard_map.h"

#define DEBUG_INTERRUPTS 0
#define KEYBOARD_ENABLED 1 


void irq_handler(int irq) {
#if DEBUG_INTERRUPTS
    static int counter = 0;

    if (irq < 32) {
        printf("Exception: Interupt (%d) - %x\n", irq, irq);
    }
    else if (irq >= 0x20 && irq <= 0x2F) {
        if (counter % 10 == 0)
            printf("IRQ %d (mapped to vector %d): Interrupt - %x\n", irq - 0x20, irq, irq);
        counter++;
    }
    else {
        printf("Other interrupt (%d) - %x\n", irq, irq);
    }  
#endif

#if KEYBOARD_ENABLED
    if (irq == 0x21) {
        uint8_t scancode = inb(0x60);
        if (!(scancode & 0x80) && scancode < 128) {
            char key = keyboard_normal[scancode];
            if (key) {
                // Print the key pressed
                printf("Key pressed: %c\n", key);
            }
        }
    }
#endif

    // Acknowledge the PIC (IRQ0–IRQ15)
    if (irq >= 0x20 && irq <= 0x2F) {
        outb(0x20, 0x20); // Master PIC
        if (irq >= 0x28) {
            outb(0xA0, 0x20); // Also slave PIC
        }
    }
}