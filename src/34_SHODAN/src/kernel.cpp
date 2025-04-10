extern "C" {
    #include "terminal.h"
    #include "gdt.h"
    #include "idt.h"
    #include "irq.h"
    #include "keyboard.h" 
    #include "memory/memory.h"

}

// Declare the end symbol from the linker script
extern uint32_t end;

extern "C" void kernel_main() {
    terminal_initialize();
    terminal_write("Hello from kernel_main!\n");

    // Initialize the Global Descriptor Table (GDT).
    gdt_install();
    terminal_write("GDT is installed!\n");

    // Initialize the Interrupt Descriptor Table (IDT).
    idt_install();
    terminal_write("IDT is installed!\n");

    // Initialize the hardware interrupts.
    irq_install(); 
    terminal_write("IRQs are installed!\n"); 

    keyboard_install();
    terminal_write("Keyboard driver installed!\n");

     // Initialize kernel paging and memory management
     init_kernel_memory(&end);
     init_paging();
     terminal_write("Paging and memory management initialized!\n");

     // Test ISR 0
    // __asm__ volatile("int $0");
    //__asm__ volatile("int $1");
    //__asm__ volatile("int $2");

    // Test basic dynamic memory allocation
    terminal_write("Testing malloc...\n");

    // Allocate three chunks of memory
    void* some_memory = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(33333);

    // Print confirmation if allocation succeeded
    if (some_memory && memory2 && memory3) {
        terminal_write("Malloc allocation successful!\n");
    } else {
    terminal_write("Malloc allocation failed!\n");
    }

    // Free the memory
    free(some_memory);
    free(memory2);
    free(memory3);
    terminal_write("Memory was freed!\n");

  
     // Allow keyboard input
    __asm__ volatile("sti");

     //terminal_write("\n--- Keyboard test begins below ---\n");

    while (1) {
        __asm__ volatile ("hlt");
    }
}
