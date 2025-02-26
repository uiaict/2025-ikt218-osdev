#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#include "globalDescriptorTable.h"
#include "miscFuncs.h"



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    terminal_initialize();
    terminal_write("Hello and welcome to SweaterOS!\n");
    terminal_write_color("This is red text!\n", COLOR_RED);
    terminal_write_color("This is green text!\n", COLOR_GREEN);
    terminal_write_color("This is blue text!\n", COLOR_BLUE);
    terminal_write_color("This is yellow text!\n", COLOR_YELLOW);

    while (1);

    return 0;
}