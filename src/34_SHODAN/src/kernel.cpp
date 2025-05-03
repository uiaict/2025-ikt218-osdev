extern "C" {
    #include "terminal.h"
    #include "gdt.h"
    #include "idt.h"
    #include "irq.h"
}

extern "C" void kernel_main() {
    terminal_initialize();
    terminal_write("Hello from kernel_main!\n");

    gdt_install();
    terminal_write("GDT is installed!\n");

    irq_remap();  // 👈 Remap IRQs before loading IDT
    terminal_write("IRQs remapped!\n");

    idt_install();
    terminal_write("IDT is installed!\n");
    __asm__ __volatile__("sti");
    // Trigger software interrupts (ISRs)
    __asm__ __volatile__("int $0x0");
    __asm__ __volatile__("int $0x3");
    __asm__ __volatile__("int $0x1");

    // Trigger IRQ interrupts manually for testing
    __asm__ __volatile__("int $0x20");  // IRQ0 - Timer
      // IRQ1 - Keyboard

    terminal_write("Back from interrupts.\n");
    terminal_write("\nPress a key:\n");
    terminal_write("Press keys now:\n");
    while (1) {
        __asm__ volatile("hlt");
    }
}