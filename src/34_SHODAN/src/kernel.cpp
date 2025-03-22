extern "C" {
    #include "terminal.h"
    #include "gdt.h"
}

extern "C" void kernel_main() {
    terminal_initialize();
    terminal_write("Hello from kernel_main!\n");

    gdt_install();
    terminal_write("GDT is installed!\n");

    while (1) {
        __asm__ volatile ("hlt");
    }
}
