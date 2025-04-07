#include "terminal/print.h"
#include "interrupts/isr.h"
#include "libc/io.h"
#include "interrupts/keyboard/keyboard_map.h"

#include "libc/stdbool.h"

#define DEBUG_INTERRUPTS 1
#define KEYBOARD_ENABLED 1 

static bool shift_pressed = false;
static bool altgr_pressed = false;

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

#if KEYBOARD_ENABLED
    if (irq == 0x21) {
        uint8_t scancode = inb(0x60);
        
        if (scancode & 0x80) {
            uint8_t released = scancode & 0x7F;
            if (released == KEYBOARD_LSHIFT || released == KEYBOARD_RSHIFT) {
                shift_pressed = false;
            } else if (released == KEYBOARD_ALT_GR) {
                altgr_pressed = false;
            }
        } 
        else {
            if (scancode == KEYBOARD_LSHIFT || scancode == KEYBOARD_RSHIFT) {
                shift_pressed = true;
            } else if (scancode == KEYBOARD_ALT_GR) {
                altgr_pressed = true;
            } else {
                char key = keyboard_normal[scancode];
                if (shift_pressed) {
                    key = keyboard_shift[scancode];
                }
                if (altgr_pressed) {
                    key = keyboard_altgr[scancode];
                }
                
                if (key != 0) {
                    printf("%c", key);
                }
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