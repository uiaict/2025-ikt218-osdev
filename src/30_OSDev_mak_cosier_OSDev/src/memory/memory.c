// memory.c
#include "libc/memory.h"
#include "libc/teminal.h"  // Changed from stdio.h to teminal.h for kprint

// Export these variables for use in paging.c
uint32_t* page_dir = 0;
uint32_t page_dir_physical = 0;
uint32_t* next_page_table = 0;

void paging_map_virtual_to_phys(uint32_t virt_addr, uint32_t phys_addr)
{
    uint16_t directory_index = virt_addr >> 22;

    for (int i = 0; i < 1024; i++)
    {
        next_page_table[i] = phys_addr | 3; // Present + Read/Write
        phys_addr += 4096;
    }

    page_dir[directory_index] = ((uint32_t)next_page_table) | 3;
    next_page_table = (uint32_t *)(((uint32_t)next_page_table) + 4096);

    kprint("Mapped virtual 0x%x (dir index: %d) to physical 0x%x\n", virt_addr, directory_index, phys_addr);
}

void paging_enable()
{
    asm volatile("mov %%eax, %%cr3" :: "a"(page_dir_physical));	
    asm volatile("mov %cr0, %eax");
    asm volatile("orl $0x80000000, %eax");
    asm volatile("mov %eax, %cr0");
}

void paging_init()
{
    kprint("Initializing paging...\n");

    // Use memory starting at 0x400000 for the page directory
    page_dir = (uint32_t*)0x400000;
    page_dir_physical = (uint32_t)page_dir;
    next_page_table = (uint32_t*)0x404000;

    // Clear directory
    for (int i = 0; i < 1024; i++)
    {
        page_dir[i] = 0x00000002; // Supervisor, Read/Write, not present
    }

    // Identity map kernel space (first 8MB)
    paging_map_virtual_to_phys(0x00000000, 0x00000000);
    paging_map_virtual_to_phys(0x00400000, 0x00400000);

    paging_enable();
    kprint("Paging enabled successfully.\n");
}