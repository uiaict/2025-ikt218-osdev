#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include <multiboot2.h>

#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/isr.h"
#include "kernel/pit.h"
#include "drivers/terminal.h"
#include "drivers/keyboard.h"
#include "memory/memory.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end; // Henta fra linker.ld
int kernel_main(void);

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Calculate end of kernel as the end of multiboot info structure initially
    // Initialize memory system with this preliminary end pointer
    terminal_initialize();

    gdt_init();
    idt_init();
    
    isrs_install();
    irq_install();
    asm volatile("sti"); // Enable interrupts

    init_kernel_memory(&end);
    init_paging();

    print_memory_layout();
    keyboard_init();

    // Register handler for breakpoint exception
    register_interrupt_handler(3, print_interrupts);

    init_pit();

    printf("Hello World!\n");
    
    void *mem1 = malloc(1000);

    int counter = 0;
    printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
    sleep_busy(1000);
    printf("[%d]: Slept using busy-waiting.\n", counter++);

    printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
    sleep_interrupt(1000);
    printf("[%d]: Slept using interrupts.\n", counter++);

    return kernel_main();
}
