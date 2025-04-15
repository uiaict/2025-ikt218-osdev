#include <memory/memory.h>
#include <libc/stdint.h>

uint32_t *page_dir[1024] __attribute__((aligned(4096)));
uint32_t *page_table[1024] __attribute__((aligned(4096)));

// Sources:
    // https://wiki.osdev.org/Setting_Up_Paging
    // https://wiki.osdev.org/Paging
void init_paging() {
    // Set up the page directory and page tables
    for (int i = 0; i < 1024; i++) {
        page_dir[i] = 0x00000002; // Present and writable
    }
    for (unsigned int i = 0; i < 1024; i++) {
        page_table[i] = (i * 0x1000) | 3; // Present, writable, and user
    }

    // Put the page table in the page directory
    page_dir[0] = (unsigned int)page_table | 3; 

    // Enable paging
    load_page_dir(page_dir);
    enable_paging();
}

