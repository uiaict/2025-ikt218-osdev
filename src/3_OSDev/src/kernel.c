// Standard library headers first
#include <libc/stddef.h>
#include <libc/stdint.h>
#include <libc/stdbool.h>
#include <libc/stdio.h>

// System headers
#include <multiboot2.h>
#include "descriptor_table.h">
#include "interrupts.h"
#include "pit.h"
#include "vga.h"

// Project-specific headers
#include "memory/memory.h"
#include "menu.h"

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Display Introduction
    Reset();
    
    // Initialize GDT, IDT, and IRQ handlers
    init_gdt();
    init_idt();
    init_irq();
    init_irq_handlers();
    enable_interrupts();

    // Initilise paging for memory management.
    init_kernel_memory(&end);
    init_paging();

    // Initilise PIT.
    init_pit();

    // Allocate some memory
    // void* some_memory = malloc(12345); 
    // void* memory2 = malloc(54321); 
    // void* memory3 = malloc(13331);

    // print_memory_layout();

    // Test PIT
    // uint32_t counter = 0;
    // while(true){
    //     printf(0x0F, "[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
    //     sleep_busy(1000);
    //     printf(0x0F, "[%d]: Slept using busy-waiting.\n", counter++);

    //     printf(0x0F, "[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
    //     sleep_interrupt(1000);
    //     printf(0x0F, "[%d]    : Slept using interrupts.\n", counter++);
    // };

    // Display the menu
    display_menu();

    // __asm__("int $0x2C"); // Mouse interrupt
    // __asm__("int $0x2A"); // Network interrupt

    while (1) {}
    return 0;
}