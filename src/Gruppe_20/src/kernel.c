#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "libc/system.h"

#include <multiboot2.h>
#include "libc/gdt_idt_table.h"
#include "libc/gdt.h"
#include "libc/idt.h"
#include "libc/isr.h"
#include "libc/keyboard.h"
#include "libc/print.h"
#include "pit.h"
#include "interrupts.h"
#include "memory/memory.h"
#include "libc/common.h"
#include "input.h"
#include "Music/song.h"
#include "Music/frequencies.h"
#include "libc/matrix_rain.h"


// Add kernel_main prototype if it exists in another file
int kernel_main();

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};
extern uint32_t end;

extern volatile uint32_t ticks;


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    init_gdt();
    init_idt();
    init_irq();
    init_interrupts();
    register_interrupt_handler(IRQ1, keyboard_callback);
    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();
    init_pit();
    asm volatile("int $0x04"); // Trigger a timer interrupt for testing

    printf("Hello %s", "World\n");
    sleep_interrupt(2000);
    
    matrix_rain_intro(150, 20);

    return kernel_main();


}