#include "libc/stdbool.h"
#include "multiboot2.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/string.h"
#include "libc/stdio.h"
#include "libc/terminal.h"
#include "memory/memory.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "pit.h"

struct multiboot_info
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end;

int main(uint32_t magic, struct multiboot_info *mb_info_addr)
{
    terminal_initialize();
    printf("Terminal initialized\n");
    init_gdt();
    printf("GDT initialized\n");
    // Initialize the Global Descriptor Table (GDT).
    init_gdt();
    printf("GDT initialized\n");

    // Initialize the Interrupt Descriptor Table (IDT).
    init_idt();
    printf("IDT initialized\n");

    // Initialize the hardware interrupts.
    init_irq();
    printf("IDT initialized\n");

    init_isr_handlers();
    init_irq_handlers();
    printf("ISR handlers initialized\n");

    init_keyboard();
    printf("Keyboard initialized\n");

    init_kernel_memory((uint32_t *)&end);
    init_paging();
    printf("Kernel memory initialized & paging\n");

    print_memory_layout();
    init_pit();

    asm volatile("sti");
    printf("Interrupts enabled\n");

    void *ptr = malloc(12345);
    void *ptr2 = malloc(54321);
    void *ptr3 = malloc(100);

    // Testing interrupt 3, 4 & 5
    asm volatile("int $0x3");
    asm volatile("int $0x4");
    asm volatile("int $0x5");
    // Uncomment to cause panic
    // asm volatile("int $0x6");

    int counter = 0;
    while (1)
    {
        // Uncomment to test PIT sleep busy/interrupt
        // printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        // sleep_busy(1000);
        // printf("[%d]: Slept using busy-waiting.\n", counter++);

        // printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        // sleep_interrupt(1000);
        // printf("[%d]: Slept using interrupts.\n", counter++);
        asm volatile("hlt");
    }
    // This should never be reached
    printf("Exiting...\n");

    return 0;
}