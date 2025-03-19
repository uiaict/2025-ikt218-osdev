#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#include "gdt.h"
#include "idt.h"
#include "terminal.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    gdt_install();
    terminal_initialize();
    writeline("Hello World\n");

    idt_install();

    // Trigger the ISRs
    __asm__ __volatile__("int $0x20");
    __asm__ __volatile__("int $0x21");
    __asm__ __volatile__("int $0x22");

    while(true)
    {
        // Do nothing
    }
    return 0;

}