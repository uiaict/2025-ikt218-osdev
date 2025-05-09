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
#include "command.h"        // Include the command system header
#include "snake_game.h"     // Include snake game header for process_pending tasks

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
    
    // Initialize PIT with higher frequency for better game responsiveness
    init_pit();

    // Install IDT and enable keyboard interrupts
    idt_install();
    enable_irq(0); // Enable timer IRQ
    enable_irq(1); // Enable keyboard IRQ
    __asm__ __volatile__("sti"); // Enable interrupts globally

    sleep_interrupt(2000); // Sleep for 2 seconds to allow for system stabilization

    // Initialize command system
    init_command_buffer();

    // Display the loading screen
    display_loading_screen();
    
    // Reset the system state before showing the command line
    reset_pit_timer();
    enable_irq(0);
    enable_irq(1);
    
    // Clear terminal after loading screen
    terminal_clear();
    
    // Welcome message
    writeline("Daemon Duo OS v1.0\n");
    writeline("Type 'help' for a list of commands\n\n");
    
    // Initial command prompt
    writeline("daemon-duo> ");
    
    // Main kernel loop
    while(true) {
        // Process any pending game updates
        process_pending_tasks();
        
        // Periodically ensure interrupts are enabled
        __asm__ __volatile__("sti");
        
        // Halt until next interrupt
        __asm__ __volatile__("hlt");
    }
}