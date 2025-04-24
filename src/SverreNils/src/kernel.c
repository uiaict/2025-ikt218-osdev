
#include "multiboot2.h"
#include "arch/idt.h"
#include "arch/isr.h"
#include "arch/irq.h"
#include "shell.h"
#include "devices/keyboard.h"
#include "arch/gdt.h"

#include "printf.h"
#include "stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"




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
      
    gdt_init();       

    __asm__ volatile("sti");  

    

    printf("Hello, Nils!\n");

    shell_prompt();

    // asm volatile("int $0x21");  // 🔍 Manuelt kall til IRQ1 for testing
    

    

    while (1) {
        __asm__ volatile ("hlt");
    }
}
