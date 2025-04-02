#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "GDT.h"
#include "IDT.h"
#include "printf.h"
#include "keyboard.h"
#include <multiboot2.h>

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Test divide by zero interrupt (INT 0)
void test_divide_by_zero() {
    printf("Triggering divide by zero exception...\n");
    int a = 10;
    int b = 0;
    // Using inline assembly to perform division that will cause exception
    // We could also write a = a / b, but some compilers might optimize it away
    asm volatile("div %1" : : "a"(a), "r"(b));
    printf("This line should not be reached\n");
}

// Test breakpoint interrupt (INT 3)
void test_breakpoint() {
    printf("Triggering breakpoint exception...\n");
    asm volatile("int $0x3");
    printf("Returned from breakpoint interrupt\n");
}

// Test general protection fault (INT 13)
void test_general_protection_fault() {
    printf("Triggering general protection fault...\n");
    // Try to execute a privileged instruction from ring 3
    // This is simulated using INT 13 directly since we're already in ring 0
    asm volatile("int $0x0D");
    printf("This line should not be reached\n");
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize GDT
    gdt_init();
    
    // Clear the screen
    clear_screen();
    printf("Hello, Kernel!\n");
    
    // Initialize IDT (which also initializes IRQs)
    idt_init();
    printf("IDT and IRQs initialized\n");
    
    // Test the breakpoint interrupt (most likely to return control)
    test_breakpoint();
    
    // Test general protection fault
    // Uncomment the line below to test
    // test_general_protection_fault();
    
    // Test divide by zero
    // Uncomment the line below to test
    // test_divide_by_zero();
    
    printf("IRQs are now enabled. Press keys to see keyboard IRQ handling.\n");
    printf("Keyboard input: ");
    
    // Kernel main loop
    printf("Entering kernel main loop...\n");
    while(1) {
        // Infinite loop to keep the kernel running
        // The timer and keyboard IRQs will now be handled automatically
    }
    
    return 0;
}