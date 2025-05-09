#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#include "descriptor_tables.h" // init_gdt
#include "terminal.h"   // terminal_write, terminal_init, terminal_put
#include "interrupts.h" // init_interrupts
#include "input.h" // init_input
#include "memory.h"
#include "pit.h"



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end;



int main(uint32_t magic, struct multiboot_info* mb_info_addr) {


    init_gdt();

    terminal_init();
    terminal_write("\n\n\n\n\n\n\nHello World\n\n");
    terminal_write("I had ambitions.\n");
    init_interrupts();

    init_irq();

    init_input();


    //init_kernel_memory(&end);
    //init_paging();
    // print_memory_layout();
    // init_pit();


}