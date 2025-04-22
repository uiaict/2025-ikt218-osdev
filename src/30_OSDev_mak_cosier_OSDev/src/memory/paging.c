#include "libc/stdio.h"     // For printf
#include "libc/memory.h"    // Memory functions
#include "libc/teminal.h"   // For kprint

// Use variables from memory.c for init_paging
extern uint32_t* page_dir;
extern uint32_t page_dir_physical;
extern uint32_t* next_page_table;

// External function declared in memory.c
extern void paging_enable(void);

// Function to initialize paging
void init_paging()
{
    kprint("Setting up paging\n");
    
    // Use paging functions from memory.c
    for(int i = 0; i < 1024; i++)
    {
        page_dir[i] = 0 | 2;  // Not present, but writable by supervisor
    }
    
    // Map first 8MB of physical memory
    paging_map_virtual_to_phys(0, 0);         // First 4MB
    paging_map_virtual_to_phys(0x400000, 0x400000); // Second 4MB
    
    paging_enable();
    kprint("Paging was successfully enabled!\n");
}
