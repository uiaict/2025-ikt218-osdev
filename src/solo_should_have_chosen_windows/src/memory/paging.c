#include "memory/paging.h"
#include "libc/stdint.h"

#define PAGE_SIZE 4096 // 4 KB
#define PAGE_TABLE_SIZE 1024 // Number of entries in a page table

static uint32_t page_directory[PAGE_TABLE_SIZE]__attribute__((aligned(4096)));
static uint32_t page_table_one[PAGE_TABLE_SIZE]__attribute__((aligned(4096)));

extern void load_page_directory(uint32_t* page_directory);
extern void enable_paging(void);

void paging_init(void) {
    // Identity map the first 4 MB of memory
    for (uint32_t i = 0; i < PAGE_TABLE_SIZE; i++) {
        page_table_one[i] = (i * 0x1000) | 0x3; 
    }
    page_directory[0] = ((uint32_t)page_table_one) | 0x3;

    load_page_directory(page_directory);
    enable_paging();
}