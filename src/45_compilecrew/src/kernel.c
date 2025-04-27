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
#include "libc/keyboard.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

__attribute__((noreturn)) void exception_handler(uint32_t int_number) {
    if (int_number >= 32 && int_number <= 47) {
        uint8_t irq = int_number - 32;

        if (irq == 1) {
            keyboard_handler();              // ✅ handle keyboard input
            irq_acknowledge(irq);            // ✅ acknowledge IRQ1
            return;
        }

        // Optional: print other IRQs for debugging
        // printf("IRQ %d received\n", irq);

        irq_acknowledge(irq);
        return;
    }

    // For exceptions (0–31), halt the system
    printf("Exception: interrupt %d\n", (int)int_number);
    __asm__ volatile("cli; hlt");
    __builtin_unreachable();
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    init_gdt();
    idt_init();

    printf("Hello World\n");
    printf("Number: %d\n", 42);
    printf("Hex: %x\n", 255);
    printf("Character: %c\n", 'A');
    printf("String: %s\n", "Kernel Booted");

    // ✅ Stay idle and wait for IRQs (keyboard/timer etc.)
    while (1) {
        __asm__ volatile("hlt");
    }
}
