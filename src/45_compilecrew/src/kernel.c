#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "libc/string.h"
#include "libc/gdt.h"
#include "libc/terminal.h"
#include "libc/printf.h"
#include "libc/idt.h"
#include "libc/irq.h"
#include "libc/keyboard.h"
#include "pit.h"
#include "memory.h"   // include memory manager

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

__attribute__((noreturn)) void exception_handler(uint32_t int_number) {
    if (int_number >= 32 && int_number <= 47) {
        uint8_t irq = int_number - 32;

        if (irq == 0 && irq_handlers[irq]) {
            irq_handlers[irq]();
            irq_acknowledge(irq);
            return; // ✅ just return, NO halt
        }

        if (irq == 1) {
            keyboard_handler();
            irq_acknowledge(irq);
            return; // ✅ just return, NO halt
        }

        irq_acknowledge(irq);
        return; // ✅ normal IRQ return
    }

    // 🛑 Only halt for real EXCEPTIONS (divide by zero, etc)
    printf("Exception: interrupt %d\n", (int)int_number);
    __asm__ volatile("cli; hlt");
    __builtin_unreachable();
}


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    init_gdt();
    idt_init();
    
    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();
    
    init_pit();
    irq_install_handler(0, pit_callback);
    __asm__ volatile ("sti");

    printf("Hello World\n");

    void* some_memory = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(13331);

    int counter = 0;
    while(true){
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
    };

    while (1) {
        __asm__ volatile("hlt");
    }
}
