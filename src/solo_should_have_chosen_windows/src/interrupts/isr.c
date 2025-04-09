#include "interrupts/keyboard/keyboard.h"

#include "terminal/print.h"
#include "interrupts/isr.h"
#include "libc/io.h"
#include "interrupts/keyboard/keyboard_map.h"

#include "libc/stdbool.h"

#define DEBUG_INTERRUPTS 0
#define KEYBOARD_ENABLED 1 

static bool shift_pressed = false;
static bool altgr_pressed = false;

extern void pit_tick();

void irq_handler(int irq) {


#if DEBUG_INTERRUPTS
    static int counter = 0;

    if (irq < 32) {
        printf("Exception: Interupt (%d) - %x\n", irq, irq);
    }
    else if (irq == 0x20) {
        if (counter % 1000 == 0)
            printf("IRQ %d (mapped to vector %d): Interrupt - %x\n", irq - 0x20, irq, irq);
        counter++;
    }
    else {
        printf("Other interrupt (%d) - %x\n", irq, irq);
    }  
#endif

    // Handle the PIT interrupt (IRQ0)
    if (irq == 0x20) {
        pit_tick();
    }
    
#if KEYBOARD_ENABLED
    if (irq == 0x21) {
        uint8_t scancode = inb(0x60);
        keyboard_handle_scancode(scancode);
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