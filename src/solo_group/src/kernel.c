#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include <multiboot2.h>
#include "descriptorTables.h"
#include "monitor.h"
#include "libc/string.h"
#include "interupts.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    initGdt();
    initIdt();
    initIrq();
    monitorInitialize();
    
    printf("Hello World\n");

    return kernel_main();
}