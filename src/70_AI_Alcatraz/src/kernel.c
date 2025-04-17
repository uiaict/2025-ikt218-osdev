#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "GDT.h"
#include "IDT.h"
#include "printf.h"
#include "keyboard.h"
#include "memory.h"
#include "pit.h"
#include <multiboot2.h>

// End of kernel - defined in linker script
extern uint32_t end;

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

// Test memory allocation
void test_memory_allocation() {
    printf("\nTesting memory allocation:\n");
    
    // Allocate some memory blocks
    void* ptr1 = malloc(100);
    void* ptr2 = malloc(200);
    void* ptr3 = malloc(300);
    
    printf("Allocated: ptr1=0x%x (100 bytes), ptr2=0x%x (200 bytes), ptr3=0x%x (300 bytes)\n", 
           (uint32_t)ptr1, (uint32_t)ptr2, (uint32_t)ptr3);
    
    // Free one block and allocate another
    printf("Freeing ptr2\n");
    free(ptr2);
    
    void* ptr4 = malloc(150);
    printf("Allocated: ptr4=0x%x (150 bytes)\n", (uint32_t)ptr4);
    
    print_memory_layout();
}

// Test PIT functions
void test_pit() {
    printf("\nTesting PIT sleep functions:\n");
    
    int counter = 0;
    
    // Test both sleep methods once
    printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
    sleep_busy(1000);
    printf("[%d]: Slept using busy-waiting.\n", counter++);
    
    printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
    sleep_interrupt(1000);
    printf("[%d]: Slept using interrupts.\n", counter++);
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize GDT
    gdt_init();
    
    // Clear the screen
    clear_screen();
    printf("Hello, Kernel!\n");
    
    // Initialize memory management
    init_kernel_memory(&end);
    
    // Initialize paging
    init_paging();
    
    // Print memory layout
    print_memory_layout();
    
    // Initialize IDT (which also initializes IRQs)
    idt_init();
    
    // Initialize PIT
    init_pit();
    
    // Clear screen again after all the initialization messages
    clear_screen();
    printf("Hello, Kernel!\n");
    printf("System initialized with Memory Management and PIT\n");
    
    // Test memory allocation
    test_memory_allocation();
    
    // Test PIT functions
    test_pit();
    
    // Test the breakpoint interrupt
    test_breakpoint();
    
    printf("IRQs are now enabled. Press keys to see keyboard IRQ handling.\n");
    printf("Keyboard input: ");
    
    // Kernel main loop
    printf("Entering kernel main loop...\n");
    
    // Continuous test of sleep functions in main loop
    int counter = 2; // Start at 2 since we already did 0 and 1 in test_pit
    while(1) {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);
        
        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
        
        // The timer and keyboard IRQs will now be handled automatically
    }
    
    return 0;
}