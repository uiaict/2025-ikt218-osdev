#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdio.h"    

#include "libc/system.h"
#include "memory/memory.h"
// Constants for memory layout
#define KERNEL_PAGE_DIR_START 0x400000     // 4MB
#define PAGE_TABLE_START      0x404000     // 4MB + 4KB
#define PAGE_SIZE             4096         // 4KB per page
#define ENTRIES_PER_TABLE     1024

// Page flags
#define PAGE_PRESENT_RW 0x3    // Present + Writable
#define PAGE_RW         0x2    // Writable only

// Page table structures
static uint32_t* kernel_page_directory = 0;
static uint32_t* next_free_page_table = 0;
static uint32_t page_directory_phys = 0;

// Map a full 4MB of virtual memory to 4MB physical memory
void paging_map_region(uint32_t virtual_addr, uint32_t physical_addr) {
    uint32_t dir_index = virtual_addr >> 22;  // Use top 10 bits for directory index

    for (int i = 0; i < ENTRIES_PER_TABLE; i++) {
        next_free_page_table[i] = (physical_addr | PAGE_PRESENT_RW);
        physical_addr += PAGE_SIZE;
    }

    kernel_page_directory[dir_index] = ((uint32_t)next_free_page_table) | PAGE_PRESENT_RW;
    next_free_page_table = (uint32_t*)((uint32_t)next_free_page_table + PAGE_SIZE);
}

// Enable paging using inline assembly
void paging_enable() {
    asm volatile("mov %0, %%cr3" : : "r"(page_directory_phys)); // Set page directory
    asm volatile("mov %%cr0, %%eax\n orl $0x80000000, %%eax\n mov %%eax, %%cr0" ::: "eax"); // Enable paging
}

// Main paging setup function
void init_paging() {
    terminal_printf("Initializing kernel paging...\n");

    kernel_page_directory = (uint32_t*)KERNEL_PAGE_DIR_START;
    page_directory_phys = (uint32_t)kernel_page_directory;
    next_free_page_table = (uint32_t*)PAGE_TABLE_START;

    // Clear all directory entries
    for (int i = 0; i < ENTRIES_PER_TABLE; i++) {
        kernel_page_directory[i] = PAGE_RW;  // Mark as not present but writable
    }

    // Map 0–4MB and 4MB–8MB (basic kernel + heap)
    paging_map_region(0x00000000, 0x00000000);       // First 4MB
    paging_map_region(0x00400000, 0x00400000);       // Next 4MB

    paging_enable();

    terminal_printf("Paging is enabled and 0-8MB mapped!\n");
}
