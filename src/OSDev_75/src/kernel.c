#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "arch/i386/GDT/gdt.h"
#include "arch/i386/interrupts/idt.h"
#include "drivers/VGA/vga.h"
#include "arch/i386/interrupts/keyboard.h"
#include "../memory/memory.h"
#include "../PIT/pit.h"
#include "drivers/audio/song.h"
#include "menu.h" 

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    initGdt();
    initIdt();
    initKeyboard();

    Reset();
    show_animation();
    print("OSDev_75 Booted Successfully!\r\n");

    print("Initializing memory management...\n");
    init_kernel_memory(&end);
    
    print("Initializing paging...\n");
    init_paging();
    
    print("Memory layout:\n");
    print_memory_layout();

    print("Initializing PIT...\n");
    init_pit();
    
    print("Starting menu system with Pong game...\n");
    sleep_interrupt(1000); // Give user time to read messages
    
    // Run our new menu system
    run_menu();
    
    // This code will not be reached, but is here as a fallback
    for (;;) {
        __asm__ __volatile__("hlt");
    }
    return 0;
}