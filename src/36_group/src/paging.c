#include "paging.h"
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>


// Page directory and page table entry flags
#define PAGE_PRESENT    0x1
#define PAGE_RW         0x2
#define PAGE_USER       0x4

// Page directory and page table sizes
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_SIZE 4096

// Page directory and page tables
uint32_t page_directory[PAGE_TABLE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
uint32_t first_page_table[PAGE_TABLE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

// Initialize paging
void init_paging() {
    // Clear the page directory and page table
    for (size_t i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        page_directory[i] = 0x0;
        first_page_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
    }

    // Map the first page table into the page directory
    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW;

    // Load the page directory into the CR3 register
    load_page_directory((uint32_t)page_directory);

    // Enable paging by setting the PG bit in CR0
    enable_paging();
}

// Assembly function to load the page directory
void load_page_directory(uint32_t page_directory_address) {
    asm volatile("mov %0, %%cr3" : : "r"(page_directory_address));
}

// Assembly function to enable paging
void enable_paging() {
    asm volatile(
        "mov %%cr0, %%eax\n"
        "or $0x80000000, %%eax\n"
        "mov %%eax, %%cr0\n"
        :
        :
        : "eax");
}