#ifndef PAGING_H
#define PAGING_H
#include "libc/stdint.h"

#define NUM_PAGES 1024    //hver page table og page directory har 1024 entries
#define PAGE_SIZE 0x1000    //4kb per side


typedef struct
{
    uint32_t present : 1; // om siden finnes i minne
    uint32_t rw : 1;      // lese/skrive
    uint32_t user : 1;    // brukertilgang
    uint32_t pwt : 1;     // Page Write-Through 
    uint32_t pcd : 1;     // Page Cache Disable 
    uint32_t accessed : 1; // om siden er aksessert
    uint32_t dirty : 1;   // om siden er modifisert
    uint32_t pat : 1;     // Page Attribute Table 
    uint32_t global : 1;  // Global Page 
    uint32_t avl : 3;     // tilgjengelig for brukeren
    
    uint32_t frame : 20;  // Fysisk rammeadresse
} __attribute__((packed)) page_table_entry_t;

typedef struct
{
    uint32_t present : 1; // om pagetabellen finnes i minne
    uint32_t rw : 1;      // lese/skrive
    uint32_t user : 1;    // brukertilgang
    uint32_t pwt : 1;     // Page Write-Through 
    uint32_t pcd : 1;     // Page Cache Disable 
    uint32_t accessed : 1; // om den er aksessert
    uint32_t avl : 1;     // tilgjengelig for brukeren
    uint32_t ps : 1;      // Page Size (0 = 4KB, 1 = 4MB) 
    uint32_t avl_2 : 4;    // tilgjengelig for brukeren
    uint32_t tableAddr : 20;   // adresse til page table
} __attribute__((packed)) page_directory_entry_t;

typedef struct
{
    page_directory_entry_t entries[NUM_PAGES];
} __attribute__((packed)) page_directory_t;

typedef struct
{
    page_table_entry_t entries[NUM_PAGES];
} __attribute__((packed)) page_table_t;

extern page_directory_t* kernelDirectory;
extern page_directory_t* currentDirectory;
//extern page_table_t* kernelPageTable;


void init_paging();
void enable_paging();
void map_virt_to_phys(uint32_t virtualAddr, uint32_t physicalAddr);

#endif