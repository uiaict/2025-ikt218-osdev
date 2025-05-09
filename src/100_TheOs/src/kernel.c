#include <multiboot2.h>
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/system.h"
#include "pit.h"
#include "common.h"
#include "descriptor_tables.h"
#include "interrupts.h"
#include "monitor.h"
#include "memory/memory.h"
#include "keyboard.h"

// Structure to hold multiboot information.
struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Forward declaration for the C++ kernel main function.
int kernel_main();

// End of the kernel image, defined elsewhere.
extern uint32_t end;

void detect_cpu(void);
void display_cpu_info(void);
uint32_t get_uptime_seconds(void); 

bool is_key_pressed() {
    if (inb(0x64) & 1) {
        inb(0x60);
        return true;
    }
    return false;
}


void wait_with_skip(int seconds) {
    printf("\nWaiting %d seconds (press any key to continue)...\n", seconds);
   
    for (int i = seconds; i > 0; i--) {
        printf("%d... ", i);
       
        
        for (int j = 0; j < 10; j++) {  
            for (int k = 0; k < 100; k++) {
                if (is_key_pressed()) {
                    printf("\nKey pressed! Continuing...\n");
                    
                    extern void terminal_clear(void);
                    terminal_clear();
                    return;
                }
               
                for (volatile int l = 0; l < 100000; l++) { }
            }
        }
    }
    printf("Done!\n");
    extern void terminal_clear(void);
    terminal_clear();
}
// Add this function to display a loading screen
void display_loading_screen() {
    terminal_clear();
    printf(" _____  _                                \n");
    printf("|_   _|| |                               \n");
    printf("  | |  | |__    ___            ___   ___ \n");
    printf("  | |  | '_ \\  / _ \\          / _ \\ / __|\n");
    printf("  | |  | | | ||  __/ _  _  _ | (_) |\\__ \\\n");
    printf("  \\_/  |_| |_| \\___|(_)(_)(_) \\___/ |___/\n");
    printf("                                         \n");
    printf("Version 0.4 - Initializing System        \n");
    printf("                                         \n");
}

void display_welcome_screen() {
    // Check if paging is enabled
    uint32_t cr0;
    asm volatile("movl %%cr0, %0" : "=r" (cr0));
    bool paging_enabled = (cr0 & 0x80000000) ? true : false;
    
    terminal_clear();
    printf(" _____  _                                \n");
    printf("|_   _|| |                               \n");
    printf("  | |  | |__    ___            ___   ___ \n");
    printf("  | |  | '_ \\  / _ \\          / _ \\ / __|\n");
    printf("  | |  | | | ||  __/ _  _  _ | (_) |\\__ \\\n");
    printf("  \\_/  |_| |_| \\___|(_)(_)(_) \\___/ |___/\n");
    printf("                                         \n");
    printf("Version 0.4 - System Ready               \n");
    printf("========================================= \n");

   
    printf("========================================= \n");
    printf("Type 'help' for available commands\n");
    printf("\n");
}

// Main entry point for the kernel, called from boot code.
// magic: The multiboot magic number, should be MULTIBOOT2_BOOTLOADER_MAGIC.
// mb_info_addr: Pointer to the multiboot information structure.
int kernel_main_c(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize monitor first so we can display text
    monitor_initialize();
   
    // Display the loading screen
    display_loading_screen();
   
    // Initialize core components with progress indicators
    detect_cpu();
   
    init_kernel_memory(&end);
   
    init_paging();
   
 
    init_pit();
    
    // Wait for 5 seconds or until a key is pressed
    // This must be after PIT initialization since it depends on the timer
    printf("\n");
    printf("Skip to mainscreen, wait for system information\n");
    wait_with_skip(5);

    // Display condensed system information
    display_cpu_info();
    // Spacers
    printf("\n");
    printf("\n");
    // Display condensed memory information
    print_memory_layout();

    printf("\n");
    printf("Skip to mainscreen\n");
    wait_with_skip(5);
    
    // Display the welcome screen with system information
    display_welcome_screen();
   
    // Return to C++ kernel for the rest of initialization
    
    return kernel_main();
}