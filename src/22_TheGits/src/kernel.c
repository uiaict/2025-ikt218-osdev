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
#include "memory/memory.h"
#include "pit/pit.h"



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end; // End of kernel memory

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    init_gdt();
    // timer_phase(100); denne erstattes av init_pit();
    printf("Hello, World!\n");
    remap_pic();
    init_idt();
    init_irq();
    //test_div_zero();

    init_kernel_memory(&end);
    init_paging();

    int counter = 0; 
    init_pit();

    void* test = malloc(100);
        printf("Malloc adresse: 0x%x\n", (uint32_t)test);

    // Aktiver interrupts
    __asm__ volatile ("sti");


    while(true){
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
    };

    /*while (1) {
        __asm__ volatile ("hlt");
    }*/


    return 0;
}