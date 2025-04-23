#include "libc/stdint.h"
// Sett en definisjon for å unngå typekonflikter med size_t
#define _SIZE_T_DEFINED
#include "miscFuncs.h"
#include "descriptorTables.h"
#include "interruptHandler.h"
#include "testFuncs.h"
#include "memory_manager.h"
#include "programmableIntervalTimer.h"
#include "menu.h"
#include "display.h"

// This is defined in the linker script
extern uint32_t end;

/**
 * Starts the operating system
 * 
 * This function initializes all necessary components
 * and runs tests to verify that everything works.
 */
static void startOS() {
    // Initialize terminal for output
    display_initialize();
    
    // Initialize system components and run tests first
    // This includes PIT and interrupt setup
    test_system_initialization();
    
    // Clear the screen completely for the boot logo
    display_clear();
    
    // Now that everything is initialized, we can show the boot logo
    display_boot_logo();
    
    // Use sleep_interrupt which is more efficient than busy waiting
    sleep_interrupt(3000);  // Wait for 3 seconds to view the logo
    
    // Start menu system without overlapping the logo
    display_clear();  // Clear screen before showing menu
    run_menu_loop();

    // Halt the CPU
    halt();
}

/**
 * Kernel entry point - called from boot.s
 */
void main(uint32_t magic, void* mb_info_addr) {
    // Verify multiboot magic number
    if (!verify_boot_magic(magic)) {
        display_write_color("ERROR: Invalid multiboot magic number!\n", COLOR_LIGHT_RED);
        halt();
        return;
    }
    
    // Start the operating system
    startOS();
}