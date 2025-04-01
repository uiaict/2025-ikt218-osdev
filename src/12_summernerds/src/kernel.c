#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "screen.h"
#include "Keyboard.c"

struct multiboot_info {
    uint32_t size; 
    uint32_t reserved; 
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr)
{
    write_to_terminal("Hello Summernerds!!!", 1);

    return 0;
}
