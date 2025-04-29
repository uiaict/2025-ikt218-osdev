#include "paging.h"

#define PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_SIZE 4096

// Page directory and one page table
static uint32_t page_directory[PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(4096)));
static uint32_t first_page_table[PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));

void init_paging() {
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | 3;  // Present, R/W
    }

    page_directory[0] = ((uint32_t)first_page_table) | 3;

    // Last inn CR3 (page directory base register)
    __asm__ volatile ("mov %0, %%cr3" :: "r"(&page_directory));

    // Slå på paging (bit 31 i CR0)
    uint32_t cr0;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile ("mov %0, %%cr0" :: "r"(cr0));
}
