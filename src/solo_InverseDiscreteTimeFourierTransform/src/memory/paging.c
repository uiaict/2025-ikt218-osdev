#include "libc/system.h"
#include "memory.h"
#include "terminal.h"


#define PAGE_DIR_BASE_ADDR      0x400000 // Physical (and virtual for kernel) address of Page Directory
#define FIRST_PAGE_TABLE_AREA   0x401000 // Start of area for Page Tables



#define PAGE_SIZE               4096     // Page size in bytes (4KB)
#define PT_ENTRY_COUNT          1024     // Number of entries in a Page Table
#define PD_ENTRY_COUNT          1024     // Number of entries in a Page Directory


#define PE_PRESENT              0x1      // Bit 0: Present
#define PE_READ_WRITE           0x2      // Bit 1: Read/Write
#define PE_USER_SUPERVISOR      0x4      // Bit 2: User/Supervisor


// Combinations
#define PE_KERNEL_RW            (PE_PRESENT | PE_READ_WRITE)
#define PE_KERNEL_RO            (PE_PRESENT)
#define PE_USER_RW              (PE_PRESENT | PE_READ_WRITE | PE_USER_SUPERVISOR)
#define PE_USER_RO              (PE_PRESENT | PE_USER_SUPERVISOR)


// Pointer to the Page Directory
static uint32_t *page_directory = NULL;

// Physical address of the Page Directory
static uint32_t page_dir_phys_addr = 0;

// Pointer to the next available memory location to place a new Page Table
static uint32_t *next_page_table_virt_addr = NULL;



// This function allocates and populates a PT.
void paging_map_4mb_region(uint32_t virt_start_addr, uint32_t phys_start_addr) {
    uint16_t pdi = virt_start_addr >> 22;

    // Get the virtual address of the PT we are about to fill
    // This PT maps the 1024 pages within the 4MB region
    uint32_t *current_page_table = next_page_table_virt_addr;
    uint32_t current_phys_addr = phys_start_addr;

    // Populate all 1024 Page Table Entries (PTEs) in this Page Table
    for (int i = 0; i < PT_ENTRY_COUNT; i++) {
        current_page_table[i] = current_phys_addr | PE_KERNEL_RW;
        current_phys_addr += PAGE_SIZE;
    }

    // Point the Page Directory Entry (PDE) to this newly populated Page Table
    page_directory[pdi] = (uint32_t)current_page_table | PE_KERNEL_RW;

    // Advance the pointer to where the *next* Page Table will be placed
    next_page_table_virt_addr = (uint32_t*)((uint32_t)current_page_table + PAGE_SIZE);
}

// Enables paging by loading CR3 and setting the PG bit in CR0
void paging_enable() {

    // Load CR3 with physical address of Page Directory
    asm volatile("mov %0, %%cr3" : : "r"(page_dir_phys_addr));

    uint32_t cr0_val;

    // Read current CR0 value
    asm volatile("mov %%cr0, %0" : "=r"(cr0_val));

    // Set Paging bit
    cr0_val |= 0x80000000;

    // Write modified value back to CR0
    asm volatile("mov %0, %%cr0" : : "r"(cr0_val));

}

// Initializes the paging system
void init_paging() {

    terminal_write("Setting up paging....\n");

    // Set Page Directory base virtual address
    page_directory = (uint32_t*)PAGE_DIR_BASE_ADDR;

    // Set Page Directory physical address
    page_dir_phys_addr = (uint32_t)page_directory;


    // Set the virtual address where the first Page Table will be created. Right after the Page Directory
    next_page_table_virt_addr = (uint32_t*)(PAGE_DIR_BASE_ADDR + PAGE_SIZE);

    

    for (int i = 0; i < PD_ENTRY_COUNT; i++) {
        page_directory[i] = 0 | PE_READ_WRITE;
    }


    // Map first 4MB of virtual memory to the first 4MB of physical memory (identity map). This covers kernel code, data, BSS, and initial heap
    paging_map_4mb_region(0x00000000, 0x00000000);



    // Map the next 4MB of virtual memory (4MB-8MB) to physical memory (4MB-8MB)
    paging_map_4mb_region(0x00400000, 0x00400000);


    paging_enable();

    terminal_write("Paging successfully enabled.\n");
}