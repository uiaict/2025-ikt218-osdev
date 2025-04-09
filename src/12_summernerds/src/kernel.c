#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "screen.h"
#include "../src/arch/i386/keyboard.h"
//#include "../src/arch/i386/print.h"
#include "../src/arch/i386/print.h"
#include "../src/arch/i386/gdt.h"
#include "../src/arch/i386/IDT.h"
#include "../src/arch/i386/ISR.h"
#include "../src/arch/i386/IRQ.h"


struct multiboot_info {
    uint32_t size; 
    uint32_t reserved; 
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr)
{
    init_gdt();
    init_idt();
    init_irq();  // flytt gjerne denne hit etter IDT init
    isr_init();
    
    write_to_terminal("Hello Summernerds!!!", 1);
    init_keyboard();

    asm volatile("sti"); 
    return 0;
}
