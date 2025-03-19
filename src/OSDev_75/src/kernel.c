#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

// Include GDT and IDT headers
#include "arch/i386/GDT/gdt.h"
#include "arch/i386/interrupts/idt.h"

// VGA driver
#include "drivers/VGA/vga.h"

// Keyboard header
#include "arch/i386/interrupts/keyboard.h"

// Example multiboot info struct (if needed)
struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize GDT and IDT
    initGdt();
    initIdt();

    // Initialize the keyboard handler
    initKeyboard();

    // Clear screen and show animation
    Reset();
    show_animation();
    print("OSDev_75 Booted Successfully!\r\n");

    // Test ISR examples
    print("Triggering ISR1 (Debug)...\n");
    asm("int $0x1");  // Should print "ISR: Debug"

    print("Triggering ISR2 (NMI)...\n");
    asm("int $0x2");  // Should print "ISR: Non Maskable Interrupt"

    print("Triggering ISR3 (Breakpoint)...\n");
    asm("int $0x3");  // Should print "ISR: Breakpoint"

    print("Triggering ISR128 (Syscall)...\n");
    asm("int $0x80");  // Should print "ISR: System Call (int 0x80)"

    // Main loop: keep the OS running
    for (;;) {
        __asm__ __volatile__("hlt");
    }
    return 0;
}
