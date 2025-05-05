#include "libc/stdint.h"
#include "libc/stddef.h"
#include <libc/stdbool.h>
#include "libc/stdio.h"

#include "gdt.h"
#include "io.h"
#include "kernel_memory.h"
#include "pit.h"
#include "paging.h"
#include <multiboot2.h>
#include "interrupt.h"
#include "keyboard.h"
#include "menu.h"
#include "matrix.h"
#include "boot.h"

extern uint32_t end;

int main(uint32_t magic, struct multiboot_info *mb)
{
    (void)magic;
    (void)mb;

    gdt_init();
    print_boot_art();

    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();
    init_pit();

    register_interrupt_handler(0, isr0_handler);
    register_interrupt_handler(1, isr1_handler);
    register_interrupt_handler(2, isr2_handler);
    register_interrupt_handler(33, keyboard_handler);

    init_idt();
    init_irq();
    init_keyboard_controller();

    uint32_t eflags;
    __asm__ volatile("pushf; pop %0" : "=r"(eflags));
    printf("Interrupts enabled: %s\n", (eflags & 0x200) ? "Yes" : "No");
    outb(0x21, 0xFC);

    puts("GDT loaded!\n");

    return kernel_main();
}
