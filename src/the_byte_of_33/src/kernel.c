#include "libc/stdint.h"
#include "libc/stddef.h"
#include <libc/stdbool.h>
#include "libc/stdio.h"

#include "gdt.h"        /* our new GDT setup */
#include "io.h"         /* VGA text helpers  */
#include "kernel_memory.h" /* kernel memory management */
#include "memory_layout.h" /* memory layout printing */
#include "pit.h"
#include "paging.h"
#include <multiboot2.h> /* keep your existing boot header */
#include "interrupt.h"
#include "keyboard.h"
#include "kernel_main.h"


extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

/* --------------------------------------------------------------------- */
/*  Kernel entry point – called from multiboot2.asm / Limine            */
/* --------------------------------------------------------------------- */
int main(uint32_t magic, struct multiboot_info *mb_info_addr)
{
    (void)magic;        /* we’re not using them yet, silence warnings   */
    (void)mb_info_addr;

    /* 1. Install the Global Descriptor Table and switch segments */
    gdt_init();

    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();
    init_pit();
    /* 2. Use VGA text mode to say hello */
    set_color(0x0A);            /* light-green on black                */
    puts("The byte of 33: GDT loaded!\n");
    puts("Penis\n");

    init_idt();
    init_irq();

    // Enable interrupts
    __asm__ volatile ("sti");

    // Test ISRs
    puts("Triggering ISR tests....\n");
    __asm__ volatile ("int $0"); // Trigger interrupt 0
    __asm__ volatile ("int $1"); // Trigger interrupt 1
    __asm__ volatile ("int $2"); // Trigger interrupt 2

    puts("Type on the keyboard to see characters....\n");

    /* 3. Halt CPU in an idle loop */
    return kernel_main();
}