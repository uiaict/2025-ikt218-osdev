#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "libc/system.h"

#include <multiboot2.h>
#include "libc/gdt_idt_table.h"
#include "libc/gdt.h"
#include "libc/idt.h"
#include "libc/isr.h"
#include "libc/print.h"
#include "pit.h"
#include "interrupts.h"
#include "memory/memory.h"
#include "libc/common.h"
#include "input.h"

// Add kernel_main prototype if it exists in another file
int kernel_main();

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end;

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    print_int(0);  // Pass a dummy value or actual value
    
    init_gdt();
    init_idt();
    init_irq();
    init_interrupts();
    init_kernel_memory(&end);
    init_paging();
    //print_memory_layout();
    init_pit();
    printf("Hello %s", "World");

    return kernel_main();

}