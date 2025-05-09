#include "libc/system.h"
#include <multiboot2.h>

#include "descriptor_tables.h"
#include "printf.h"
#include "idt.h"
#include "keyboard.h"
#include "malloc.h"
#include "paging.h"
#include "pit.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();
extern uint32_t end;

int main(uint32_t magic, struct multiboot_info *mb_info_addr) {
    initGdt();
    initIdt();
    init_kernel_memory(&end);
    init_paging();
    init_pit();
    init_keyboard();
    sleep_busy(1000);
    
    return kernel_main();
}