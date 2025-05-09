#include "paging.h"
#include "terminal.h"
#include "port_io.h"
#include <stdint.h>

#define PAGE_SIZE 4096

// Define page directory and page tables
static uint32_t page_directory[1024] __attribute__((aligned(PAGE_SIZE)));
static uint32_t first_page_table[1024] __attribute__((aligned(PAGE_SIZE)));

// Initialize simple paging
void init_paging(void) {
    // Map first 4MB (identity mapping)
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | 3; // Present and writable
    }

    page_directory[0] = ((uint32_t)first_page_table) | 3; // Present and writable
    for (int i = 1; i < 1024; i++) {
        page_directory[i] = 0; // Not present
    }

    // Load page directory
    asm volatile ("mov %0, %%cr3" :: "r"(page_directory));

    // Enable paging
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Set paging bit
    asm volatile ("mov %0, %%cr0" :: "r"(cr0));

    terminal_printf("[OK] Paging enabled.\n");
}
