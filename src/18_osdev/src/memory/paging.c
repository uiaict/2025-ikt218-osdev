#include "paging.h"
#include "libc/stdint.h"
#include "libc/string.h"



#define PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_SIZE 4096


__attribute__((aligned(4096))) uint32_t page_directory[PAGE_DIRECTORY_ENTRIES];
__attribute__((aligned(4096))) uint32_t first_page_table[PAGE_TABLE_ENTRIES];




// identity map the first 4MB of memory, aka setting virtual = physical
void init_paging() {
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        // calculates the physical adress, and sets permissions to read/write
        first_page_table[i] = (i * PAGE_SIZE) | 3; 
    }
    // set the first page table to be the first page directory entry
    page_directory[0] = ((uint32_t)first_page_table) | 3;

    // set the rest of the page directory to be empty
    for (int i = 1; i < PAGE_DIRECTORY_ENTRIES; i++) {
    page_directory[i] = 0;
    }

    // load the page directory into CR3
    asm volatile("mov %0, %%cr3" : : "r"(page_directory)); 


    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // set the paging bit
    asm volatile("mov %0, %%cr0" : : "r"(cr0)); // load the new CR0 value

    }

