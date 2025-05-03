#include "libc/stdio.h"
#include "drivers/keyboard.h"
#include "interrupts/pit.h"
#include "interrupts/io.h"
#include "libc/stdint.h"
#include "interrupts/desTables.h"

// Multiboot-info struct hvis du skal bruke den senere
struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag* first;
};

extern volatile uint32_t pit_ticks; // Du må legge dette i pit.h

void kmain(uint32_t magic, struct multiboot_info* mb_info_addr) {
    printf("Hello World!\n");

    initDesTables();  // Initialiserer GDT og IDT

    printf("[TEST] Triggering interrupts...\n");

    // Trigger noen typiske CPU exceptions:
    asm volatile ("int $0x0");  // Division by zero
    asm volatile ("int $0x6");  // Invalid opcode
    asm volatile ("int $0x3");  // Invalid opcode

   
    
    initKeyboard();   // Registrerer IRQ1-handler for tastatur
    initPit();        // Registrerer IRQ0-handler for timer
    

    printf("[OK] IDT initialized\n");
    printf("[OK] Keyboard initialized\n");
    printf("[OK] PIT initialized\n");

    // Evig loop
    while (1) {
        __asm__ volatile("hlt");
    }
}
