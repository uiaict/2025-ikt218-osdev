// kernel.c

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/multiboot2.h"
#include "libc/teminal.h"   // terminal_putc, kprint, move_cursor, etc.
#include "libc/gdt.h"
#include "libc/idt.h"
#include "libc/keyboard.h"
#include "libc/vga.h"
#include "libc/io.h"
#include "libc/pit.h"
#include "libc/memory.h"
#include "libc/song.h"
#include "libc/snake.h"    // ← new include for your snake game

// Linker symbol marking end of kernel in memory
extern uint32_t end;

int main(uint32_t magic, struct multiboot_info* mb_info_addr) 
{
    // --- Core init ---
    init_gdt();
    init_idt();
    initKeyboard();           // start IRQ1 handler
    __asm__ volatile("sti");  // enable interrupts

    // --- Memory management ---
    init_kernel_memory((uint32_t)&end);
    paging_init();
    print_memory_layout();

    void* mem1 = malloc(1000);
    void* mem2 = malloc(500);
    kprint("Allocated memory blocks at %x and %x\n", mem1, mem2);

    // --- PIT (1 kHz tick = 1 ms) ---
    init_pit();

    // --- Launch Snake ---
    // This will take over the screen and never return.
    snake_run();

    // (unreachable)
    return 0;
}
