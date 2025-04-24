#include "multiboot2.h"
#include "arch/idt.h"
#include "arch/isr.h"
#include "arch/irq.h"
#include "shell.h"
#include "devices/keyboard.h"
#include "arch/gdt.h"
#include "kernel_memory.h"
#include "paging.h"
#include "pit.h"

#include "printf.h"
#include "stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

extern uint32_t end; // Definert av linker.ld

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

void putc_raw(char c) {
    volatile char* video = (volatile char*)(0xB8000 + 160 * 23); // linje 24
    video[0] = c;
    video[1] = 0x07;
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    idt_init();          
    isr_install();       
    irq_install();       
    init_keyboard();     // Kall den faktiske init-funksjonen, ikke definer den her

    gdt_init();          

    __asm__ volatile("sti");  // Aktiver maskinavbrudd

    init_kernel_memory(&end);  // ✅ Memory management
    init_paging();             // ✅ Paging
    print_memory_layout();     // ✅ Vis minnelayout
    init_pit();                // ✅ PIT-timer

    printf("Hello, Nils!\n");

    void* some_memory = malloc(12345); 
    void* memory2 = malloc(54321); 
    void* memory3 = malloc(13331);

    shell_prompt();

    int counter = 0;
    while (1) {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);

        __asm__ volatile ("hlt");
    }
}
