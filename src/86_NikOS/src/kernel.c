#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "multiboot2.h"
#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "pic.h"
#include "irq.h"
#include "keyboard.h"
#include "ports.h"
#include "welcome.h"
#include "memory.h"
#include "paging.h"
#include "pit.h"
#include "rng.h"
#include "song.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end;


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    gdt_install();
    idt_install();
    isr_install();
    pic_remap();
    irq_install();

    keyboard_install();

    kernel_memory_init(&end);
    paging_init();
    enable_virtual_memory();

    pit_init();
    
    terminal_initialize();
    welcome_message();
    terminal_writestring("> ");

    __asm__ __volatile__("sti");

    while (1) {
        __asm__ __volatile__("hlt");
    }
    
    return 0;

}