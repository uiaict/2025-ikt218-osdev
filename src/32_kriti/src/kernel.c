//Kernel.c

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "kprint.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "multiboot2.h"
#include "kprint.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "keyboard.h"
#include "memory.h"
#include "pit.h"  // Include the PIT header

// This is defined in the linker script (linker.ld)
extern unsigned long end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Main kernel entry point
int main(unsigned long magic, struct multiboot_info* mb_info_addr) {
    // Write "Hello World" directly to video memory (VGA text mode)
    const char *str = "Hello World";
    char *video_memory = (char*) 0xb8000;
    for (int i = 0; str[i] != '\0'; i++) {
        video_memory[i * 2]     = str[i];
        video_memory[i * 2 + 1] = 0x07;  // White on black
    }

    kprint("Loading GDT...\n");
    init_gdt();
    kprint("GDT loaded\n");

    kprint("Initializing IDT...\n");
    idt_init();
    kprint("IDT initialized\n");

    kprint("Initializing ISR...\n");
    isr_init();
    kprint("ISR initialized\n");

    kprint("Initializing PIC...\n");
    pic_init();

    kprint("PIC initialized\n");

    // Enable interrupts early to ensure PIT works
    kprint("Enabling interrupts...\n");
    __asm__ volatile ("sti");
    kprint("Interrupts enabled\n");

    // Initialize the kernel's memory manager using the end address of the kernel
    kprint("Initializing kernel memory manager...\n");
    init_kernel_memory(&end);
    
    // Initialize paging for memory management
    kprint("Initializing paging...\n");
    init_paging();
    
    // Print memory information
    kprint("Printing memory layout...\n");
    print_memory_layout();
    
    // Initialize PIT
     kprint("Initializing PIT...\n");
    init_pit();
    
    // Check initial tick count
    kprint("Initial tick count: ");
    kprint_dec(get_tick_count());
    kprint("\n");

    // Test sleep_interrupt
    kprint("Testing sleep_interrupt for 1000ms...\n");
    sleep_interrupt(1000);  // Sleep for 1 second
    
    kprint("Tick count after 1s interrupt sleep: ");
    kprint_dec(get_tick_count());
    kprint("\n");
    
    // Test sleep_busy
    kprint("Testing sleep_busy for 500ms...\n");
    sleep_busy(500);  // Sleep for 0.5 seconds
    
    kprint("Tick count after 0.5s busy sleep: ");
    kprint_dec(get_tick_count());
    kprint("\n");

        // Add a long delay here to observe all the messages
    kprint("\n--- Pausing for 10 seconds to observe output, press any key to continue ---\n");

    // Option 1: Use a much longer sleep_interrupt
    sleep_interrupt(10000);  // 10 seconds

    // Option 2: Wait for a key press before continuing
    // This requires implementing a function to wait for a key
    // wait_for_keypress();  // Uncomment if you have this function

    // Continue with the rest of your initialization
    kprint("\nContinuing kernel initialization...\n");

    // Initialize keyboard
    kprint("Initializing keyboard...\n");
    keyboard_init();

    // Test interrupts
    kprint("Testing NMI interrupt (int 0x2)...\n");
    __asm__ volatile ("int $0x2");
    
    kprint("Testing breakpoint interrupt (int 0x3)...\n");
    __asm__ volatile ("int $0x3");

    // Test memory allocation
    kprint("\nTesting memory allocation...\n");
    void* some_memory = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(13331);
    
    kprint("Allocated memory at: 0x");
    kprint_hex((unsigned long)some_memory);
    kprint("\n");
    
    kprint("Allocated memory at: 0x");
    kprint_hex((unsigned long)memory2);
    kprint("\n");
    
    kprint("Allocated memory at: 0x");
    kprint_hex((unsigned long)memory3);
    kprint("\n");
    
    // Print updated memory layout
    kprint("\nUpdated memory layout after allocations:\n");
    print_memory_layout();
    
    // Test memory freeing
    kprint("\nFreeing memory...\n");
    free(memory2);
    
    kprint("Memory layout after free:\n");
    print_memory_layout();

    kprint("\nSystem initialized successfully!\n");
    kprint("Press any key to see keyboard input...\n");

    // Unmask keyboard interrupt (IRQ1)
    outb(PIC1_DATA_PORT, inb(PIC1_DATA_PORT) & ~0x02);  // Enable IRQ1 (keyboard)

    // Simple main loop without heartbeat
    while (1) {
        __asm__ volatile ("hlt");
    }

    return 0;
}

#ifdef __cplusplus
// Optional C++ kernel main function if needed
int cpp_kernel_main() {
    kprint("Entering C++ kernel function\n");
    
    // Test C++ memory allocation with operator new
    kprint("Testing C++ memory allocation...\n");
    int* test_array = new int[100];
    
    kprint("C++ allocation at: 0x");
    kprint_hex((unsigned long)test_array);
    kprint("\n");
    
    // Initialize array
    for (int i = 0; i < 100; i++) {
        test_array[i] = i;
    }
    
    // Use array
    int sum = 0;
    for (int i = 0; i < 100; i++) {
        sum += test_array[i];
    }
    
    kprint("Sum of array elements: ");
    kprint_dec(sum);
    kprint("\n");
    
    // Free memory
    delete[] test_array;
    
    kprint("C++ memory test complete\n");
    
    return 0;
}
#endif