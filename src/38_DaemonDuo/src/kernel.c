#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#include "gdt.h"
#include "idt.h"
#include "terminal.h"
#include "memory.h"
#include "pit.h"
#include "song.h"
#include "song_player.h"
#include "loading_screen.h" // Include the loading screen header

// Reference to the end of the kernel in memory
extern uint32_t end;

// External declaration for example_song
extern const struct note example_song[];
extern const struct note mario_theme[];

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    gdt_install(); // Initialize the GDT
    terminal_initialize(); // Initialize the terminal

    // Initialize memory management
    init_kernel_memory(&end);
    init_paging();
    
    // Print memory layout information
    print_memory_layout();
    
    // Initialize PIT
    init_pit();

    // Install IDT and enable keyboard interrupts
    idt_install();
    enable_irq(0); // Enable timer IRQ
    enable_irq(1); // Enable keyboard IRQ
    __asm__ __volatile__("sti"); // Enable interrupts globally

    writeline("\n\nHello World\n\n\n"); // Print to the terminal
    writeline ("Kernel loaded successfully! Sleeping for 5 seconds.\n");

    sleep_interrupt(5000); // Sleep for 1 seconds
    terminal_clear(); // Clear the terminal

    // Display the loading screen
    display_loading_screen();
    
    sleep_interrupt(5000); // Sleep for 1 second

    // Clear terminal after loading screen
    terminal_clear();

    //play_song(mario_theme); // Play the example song
    
    while(true) {
        __asm__ __volatile__("hlt"); // Halt the CPU until an interrupt occurs
    }
}