#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

#include "gdt.h"        /* our new GDT setup */
#include "io.h"         /* VGA text helpers  */
#include <multiboot2.h> /* keep your existing boot header */

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

/* --------------------------------------------------------------------- */
/*  Kernel entry point – called from multiboot2.asm / Limine            */
/* --------------------------------------------------------------------- */
int main(uint32_t magic, struct multiboot_info *mb_info_addr)
{
    (void)magic;        /* we’re not using them yet, silence warnings   */
    (void)mb_info_addr;

    /* 1. Install the Global Descriptor Table and switch segments */
    gdt_init();

    /* 2. Use VGA text mode to say hello */
    set_color(0x0A);            /* light-green on black                */
    puts("The byte of 33: GDT loaded!\n");
    puts("Hello from your brand-new kernel.\n");

    /* 3. Halt CPU in an idle loop */
    for (;;)
        __asm__ volatile ("hlt");

    /* never reached, but keep the prototype happy */
    return 0;
}