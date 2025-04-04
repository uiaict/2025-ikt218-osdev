#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "kprint.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "keyboard.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Write "Hello World" directly to video memory (VGA text mode)
    const char *str = "Hello World";
    char *video_memory = (char*) 0xb8000;
    for (int i = 0; str[i] != '\0'; i++) {
        video_memory[i * 2]     = str[i];
        video_memory[i * 2 + 1] = 0x07;  // White on black
    }

    kprint("Loading GDT...\n");
    init_gdt();
    kprint("GDT loaded\n");

    kprint("Initializing IDT...\n");
    idt_init();
    kprint("IDT initialized\n");

    kprint("Initializing ISR...\n");
    isr_init();
    kprint("ISR initialized\n");

    kprint("Initializing PIC...\n");
    pic_init();
    kprint("PIC initialized\n");

    keyboard_init();

    // Enable interrupts after all initialization
    kprint("Enabling interrupts...\n");
    __asm__ volatile ("sti");
    kprint("Interrupts enabled\n");

    // Test interrupts
    kprint("Testing NMI interrupt (int 0x2)...\n");
    __asm__ volatile ("int $0x2");
    
    kprint("Testing breakpoint interrupt (int 0x3)...\n");
    __asm__ volatile ("int $0x3");

    kprint("System initialized successfully!\n");
    kprint("Press any key to see keyboard input...\n");

    // Unmask keyboard interrupt (IRQ1)
    outb(PIC1_DATA, 0xFD);  // 1111 1101 - enable IRQ1 (keyboard)

    // Main loop - use hlt to save power while waiting for interrupts
    while (1) {
        __asm__ volatile ("hlt");
    }

    return 0;
}