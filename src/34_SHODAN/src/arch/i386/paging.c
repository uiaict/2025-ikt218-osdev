#include "paging.h"
#include "port_io.h"
#include <stdint.h>

#define PAGE_SIZE 4096
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_DIRECTORY_ENTRIES 1024

// Page-aligned page directory and page table
static uint32_t page_directory[PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
static uint32_t first_page_table[PAGE_TABLE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

void init_paging() {
    for (uint32_t i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | 3;
    }

    // 🛡️ Re-explicitly map VGA text memory
    first_page_table[0xB8] = 0xB8000 | 3;

    page_directory[0] = ((uint32_t)first_page_table) | 3;
    for (uint32_t i = 1; i < PAGE_DIRECTORY_ENTRIES; i++) {
        page_directory[i] = 0;
    }

    __asm__ __volatile__("mov %0, %%cr3" :: "r"(page_directory));

    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0" :: "r"(cr0));
}
