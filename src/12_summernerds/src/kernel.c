#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "screen.h"
#include "../src/arch/i386/keyboard.h"
//#include "../src/arch/i386/print.h"
#include "../src/arch/i386/io.h"
#include "../src/arch/i386/gdt.h"
//#include "../src/arch/i386/IDT.h"
//#include "../src/arch/i386/ISR.h"
#include "../src/arch/i386/interuptRegister.h"
#include "../src/arch/i386/monitor.h"
#include "../include/kernel/memutils.h"
//#include "../include/kernel/memutils.h"


struct multiboot_info {
    uint32_t size; 
    uint32_t reserved; 
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr);

extern uint32_t end();

void isr_handler(struct InterruptRegister* regs) {
    // Handle the interrupt here
    // Prints the interrupt number
    printf("Interrupt: %d\n", regs->int_no);
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr)
{
    monitor_init(); // Initialize the monitor


    init_gdt();
    init_idt();
    init_irq();  // flytt gjerne denne hit etter IDT init
    isr_init();
    init_kernel_memory(&end);

    // Initialize paging for memory management.
    init_paging();

    // Print memory information.
    print_memory_layout();

    init_pit();
    
    write_to_terminal("Hello Summernerds!!!", 1);
    init_keyboard();

    asm volatile("sti"); 
    return 0;
}
