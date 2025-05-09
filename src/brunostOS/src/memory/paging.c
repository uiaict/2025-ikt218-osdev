#include "memory/paging.h"
#include "memory/memutils.h"

#define DIRECTORY_SIZE 1024            // 4*uint8_t = uint32_t, 4*1024 = 4kb
#define PAGE_SIZE 4096

static uint32_t* page_directory = 0;   // each entry points to a page table
static uint32_t page_dir_loc = 0;      // location of page_directory
static uint32_t* last_page = 0;        // last page allocated

// Paging now will be really simple
// we reserve 0-8MB for kernel stuff
// heap will be from approx 1mb to 4mb
// and paging stuff will be from 4mb

void init_paging(){

    page_directory = (uint32_t*)0x400000;      // page directory start at end of pheap
    page_dir_loc = (uint32_t)page_directory;   // store location of page_directory
    last_page = (uint32_t *)0x404000;          // gives page_directory 4kb of space

    for(int i = 0; i < DIRECTORY_SIZE; i++){   // 4*uint8_t = uint32_t, 4*1024 = 4kb          
        page_directory[i] = 0 | 2;             // 0b...010, supervisor, readwrite, not present
    }

    paging_map_virtual_to_phys(0, 0);               // Map the first 4 MB of virtual memory to the first 4 MB of physical memory
    paging_map_virtual_to_phys(0x400000, 0x400000); // Map the next 4 MB of virtual memory to the next 4 MB of physical memory

    // Enable paging
    asm volatile("mov %%eax, %%cr3": :"a"(page_dir_loc));   // load CR3 register with location of page directory
    asm volatile("mov %cr0, %eax");                         // mov CR0 to EAX
    asm volatile("orl $0x80000000, %eax");                  // "orl" set "paging enabled" bit for EAX (EAX = CR0)  
    asm volatile("mov %eax, %cr0");                         // move content of EAX back to CR0
}



void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys){

    uint16_t id = virt >> 22;        // Get the upper 10 bits of the virtual address to use as index in the page directory

    for(int i = 0; i < DIRECTORY_SIZE; i++){   // Loop through all 1024 page table entries
    
        last_page[i] = phys | 3;    // 0b..phys..11, read/write, present
        phys += PAGE_SIZE;          // Increment by page size
    }
    
    page_directory[id] = ((uint32_t)last_page) | 3;  // Set the page table in the correct place in page directory
    last_page = (uint32_t *)(((uint32_t)last_page) + PAGE_SIZE); // increment last page
}