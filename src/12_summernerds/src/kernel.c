#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "screen.h"
#include "../src/arch/i386/keyboard.h"
#include "../src/arch/i386/print.h"

struct multiboot_info {
    uint32_t size; 
    uint32_t reserved; 
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr)
{
    init_irq();
    isr_init();
    write_to_terminal("Hello Summernerds!!!", 1);
    init_keyboard();  

    return 0;
}
