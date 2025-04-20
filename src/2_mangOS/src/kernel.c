#include "libc/stdbool.h"
#include "multiboot2.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/string.h"
#include "libc/stdio.h"
#include "libc/terminal.h"
#include "gdt.h"
#include "idt.h"

struct multiboot_info
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info *mb_info_addr)
{
    terminal_initialize();
    printf("Terminal initialized\n");
    init_gdt();
    printf("GDT initialized\n");
    init_idt();
    init_irq();

    printf("Interrupts initialized\n");

    printf("Hello %s!\n", "World");

    return 0;
}