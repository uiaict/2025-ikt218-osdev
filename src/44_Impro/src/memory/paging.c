#include "libc/stdint.h"
#include "paging.h"




static uint32_t page_directory[1024] __attribute__((aligned(4096)));

void init_paging() {
    
    for (int i = 0; i < 1024; i++) {

        uint32_t address = i * 0x200000;
        page_directory[i] = address | 0x83;
    }

    load_page_directory(page_directory);

    enable_paging();
}
