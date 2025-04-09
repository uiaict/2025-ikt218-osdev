#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "libc/stdarg.h"
#include "libc/gdt.h" 
#include "libc/scrn.h"
#include "libc/idt.h"
#include "libc/isr_handlers.h"
#include "libc/irq.h"
#include "libc/memory.h"



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end; // End of kernel memory

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    init_gdt();
    timer_phase(100);
    printf("Hello, World!\n");
    remap_pic();
    init_idt();
    init_irq();
    //test_div_zero();

    init_kernel_memory(&end);
    init_paging();

    void* test = malloc(100);
        printf("Malloc adresse: 0x%x\n", (uint32_t)test);

    // Aktiver interrupts
    __asm__ volatile ("sti");

    while (1) {
        __asm__ volatile ("hlt");
    }

    return 0;
}