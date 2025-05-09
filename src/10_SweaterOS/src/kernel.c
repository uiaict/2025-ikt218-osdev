#include "libc/stdint.h"
// Definerer size_t for å unngå typekonflikter
#define _SIZE_T_DEFINED
#include "miscFuncs.h"
#include "descriptorTables.h"
#include "interruptHandler.h"
#include "testFuncs.h"
#include "memory_manager.h"
#include "programmableIntervalTimer.h"
#include "menu.h"
#include "display.h"
#include "snake.h"

// Dette er definert i linker scriptet
extern uint32_t end;

static void startOS() {
    initialize_system();
    snake_init();
    display_clear();
    display_boot_logo();
    sleep_interrupt(1000);
    
    display_write_color("\n\n            Press any key to continue...", COLOR_YELLOW);
    
    // Venter på tastetrykk
    while (!keyboard_data_available()) {
        __asm__ volatile("hlt");
    }
    keyboard_getchar();

    display_clear();
    run_menu_loop();

    // Stanser CPU-en, midlertidig bro
    halt();
}

/**
 * magic: Multiboot magic number
 * mb_info_addr: Adresse til Multiboot informasjonen
 */
void main(uint32_t magic, void* mb_info_addr) {
    startOS();
}