#include "memory/paging.h"
#include "libc/stdio.h"

#define PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_TABLE_ENTRIES     1024
#define PAGE_SIZE              4096

#define PRESENT 0x1
#define WRITE   0x2

static uint32_t _page_directory[1024] __attribute__((aligned(4096)));
static uint32_t _first_page_table[1024] __attribute__((aligned(4096)));

static uint32_t* page_directory = 0;
static uint32_t* first_page_table = 0;

// Maps one virtual address to one physical address
void paging_map(uint32_t virt_addr, uint32_t phys_addr) {
    uint32_t dir_index = virt_addr >> 22;
    uint32_t table_index = (virt_addr >> 12) & 0x03FF;

    uint32_t* page_table = (uint32_t*)(page_directory[dir_index] & ~0xFFF);
    if (!page_table) return;

    page_table[table_index] = (phys_addr & ~0xFFF) | PRESENT | WRITE;
}

// Enables paging via CR0/CR3
void paging_enable() {
    asm volatile("mov %0, %%cr3" :: "r"(page_directory));
    asm volatile(
        "mov %%cr0, %%eax\n\t"
        "or $0x80000000, %%eax\n\t"
        "mov %%eax, %%cr0"
        ::: "eax"
    );
}

// Initializes basic paging (identity map 0–4MB)
void init_paging() {
    printf("[PAGING] Initializing paging...\n");

    page_directory = _page_directory;
    first_page_table = _first_page_table;

    // Clear and set up page directory and table
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0 | WRITE;
        first_page_table[i] = (i * PAGE_SIZE) | PRESENT | WRITE;
    }

    // Map first 4MB (identity mapping)
    page_directory[0] = ((uint32_t)first_page_table) | PRESENT | WRITE;

    paging_enable();

    printf("[PAGING] Paging enabled. 0–4MB identity-mapped.\n");

    // Simple test
    uint32_t* test = (uint32_t*)0x00100000;
    *test = 0xCAFEBABE;
    printf("[PAGING TEST] Wrote 0x%x\n", *test);
}
