#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "keyboard.h"
#include "terminal.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <string.h>
#include "memory.h" 
#include "pit.h"

extern uint32_t end; 


int kernel_main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    (void)magic;
    (void)mb_info_addr;

    terminal_initialize();
    terminal_setcolor(0x0A); // Green text

    terminal_printf("[INFO] Terminal initialized.\n");

    gdt_install();
    terminal_printf("[OK] GDT installed.\n");

    idt_install();
    terminal_printf("[OK] IDT installed.\n");

    keyboard_install();
    terminal_printf("[OK] Keyboard IRQ installed.\n");

    terminal_printf("[+] Kernel initialization complete!\n\n");

    // Memory Setup
    terminal_printf("=== [MEMORY SETUP] ===\n");

    init_kernel_memory(&end); // starting kernel heap

    void* block1 = malloc(32);
    void* block2 = malloc(32);

    terminal_printf("[OK] Allocated memory blocks:\n");
    terminal_printf("  Block1 at 0x%x\n", (uint32_t)block1);
    terminal_printf("  Block2 at 0x%x\n", (uint32_t)block2);


    terminal_printf("[INFO] Kernel heap starts at 0x%x\n", (uint32_t)&end);
    terminal_printf("[INFO] Heap allocation starts near 0x%x\n", (uint32_t)block1);

    print_memory_layout();

    // Paging
    terminal_printf("\n[INFO] Setting up paging...\n");
    init_paging();   // If we have a paging function, it is called here.
    terminal_printf("[OK] Paging successfully enabled!\n");

    // PIT
    init_pit();
    terminal_printf("[OK] PIT initialized.\n\n");

    // Hello World
    terminal_printf("Hello from Group 9!\n");

    // Test interrupts
    terminal_printf("\n=== [INTERRUPT TEST] ===\n");
    __asm__ __volatile__("int $0x3");
    terminal_printf("[OK] Interrupt 3 triggered.\n");

    __asm__ __volatile__("int $0x4");
    terminal_printf("[OK] Interrupt 4 triggered.\n");

    // Sleep tests
    terminal_printf("\n=== [SLEEP TESTS] ===\n");

    int counter = 0;
    while (counter < 4) {
        terminal_printf("[INFO] [%d] Sleeping with busy-waiting (HIGH CPU)...\n", counter);
        sleep_busy(1000);
        terminal_printf("[INFO] [%d] Slept using busy-waiting.\n", counter++);

        terminal_printf("[INFO] [%d] Sleeping with interrupts (LOW CPU)...\n", counter);
        sleep_interrupt(1000);
        terminal_printf("[INFO] [%d] Slept using interrupts.\n", counter++);
    }

    // Wait forever (don't let CPU idle)
    while (1) {
        __asm__ __volatile__("hlt");
    }

}