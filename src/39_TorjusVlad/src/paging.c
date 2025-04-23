#include "libc/paging.h" 


#define PAGE_SIZE 4096
#define PAGE_ENTRIES 1024
#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_USER 0x4

typedef uint32_t page_directory_entry_t;
typedef uint32_t page_table_entry_t;

static page_directory_entry_t page_directory[PAGE_ENTRIES] __attribute__((aligned(4096)));
static page_table_entry_t first_page_table[PAGE_ENTRIES] __attribute__((aligned(4096)));


void init_paging(){
    for(uint32_t i = 0; i<PAGE_ENTRIES; i++){
        first_page_table[i] = (i*PAGE_SIZE)|PAGE_PRESENT|PAGE_RW;

    }
    page_directory[0] = ((uint32_t)first_page_table)|PAGE_PRESENT|PAGE_RW;

    for(uint32_t i = 1; i< PAGE_ENTRIES; i++){
        page_directory[i] = 0;
    }
}

void enable_paging(){
    asm volatile("mov %0, %%cr3" :: "r"(page_directory));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |=0x80000000;
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}