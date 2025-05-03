extern "C" {
    #include "terminal.h"
    #include "gdt.h"
    #include "idt.h"
    #include "irq.h"
    #include "memory.h"
    #include "pit.h"
}

extern "C" uint32_t end;

extern "C" void kernel_main() {
    gdt_install();
    irq_remap();  // Remap IRQs before loading IDT
    idt_install();
    init_kernel_memory(&end);
    
    terminal_initialize();  // ✅ Now VGA memory is safely mapped
    terminal_write("Hello from kernel_main!\n");
    init_paging();  // 🛠 Enable paging BEFORE touching VGA memory

    terminal_write("GDT is installed!\n");
    terminal_write("IRQs remapped!\n");
    terminal_write("IDT is installed!\n");
    terminal_write("Kernel memory manager initialized!\n");
    terminal_write("Paging initialized!\n");

    print_memory_layout();

    init_pit();
    terminal_write("PIT initialized!\n");

    void* a = malloc(1234);
    void* b = malloc(5678);
    terminal_write("Allocated memory!\n");

    __asm__ __volatile__("sti");

    // Trigger software interrupts (ISRs)
    __asm__ __volatile__("int $0x0");
    __asm__ __volatile__("int $0x3");
    __asm__ __volatile__("int $0x1");

    // Trigger IRQ0 (Timer) for test
    __asm__ __volatile__("int $0x20");

    terminal_write("Back from interrupts.\n");
    terminal_write("\nPress a key:\n");
    terminal_write("Press keys now:\n");

    int counter = 0;
    while (1) {
        terminal_write("[");
        terminal_putint(counter);
        terminal_write("]: Sleeping with busy-waiting (HIGH CPU)...\n");
        sleep_busy(1000);

        terminal_write("[");
        terminal_putint(counter);
        terminal_write("]: Slept using busy-waiting.\n");
        counter++;

        terminal_write("[");
        terminal_putint(counter);
        terminal_write("]: Sleeping with interrupts (LOW CPU)...\n");
        sleep_interrupt(1000);

        terminal_write("[");
        terminal_putint(counter);
        terminal_write("]: Slept using interrupts.\n");
        counter++;
    }
}
