#include "paging.h"
#include "libc/stdint.h"
#include "../io/printf.h"

//Denne fila er hentet fra solution guide til Per Arne Andersen, med små endringer.

static uint32_t* page_directory = 0;
static uint32_t page_dir_loc = 0;      
static uint32_t* last_page = 0;       

//heap vil være fra 1mb til 4mb
//Paging vil være fra 4mb 


void map_virt_to_phys(uint32_t virt, uint32_t phys)
{
    uint16_t id = virt >> 22;       
    for(int i = 0; i < 1024; i++)   
    {
        last_page[i] = phys | 3;   
        phys += 4096;               
    }
    page_directory[id] = ((uint32_t)last_page) | 3;  
    last_page = (uint32_t *)(((uint32_t)last_page) + 4096); 
}

void enable_paging()
{
    asm volatile("mov %%eax, %%cr3": :"a"(page_dir_loc));  // laster pageDir addresse til eax, deretter cr3
    asm volatile("mov %cr0, %eax");                        // laster cr0 register til eax
    asm volatile("orl $0x80000000, %eax");                 // setter bit 31 i cr0 registeret
    asm volatile("mov %eax, %cr0");                        // laster eax (oppdatert cr0) tilbake til cr0 registeret
}

void init_paging()
{
    mafiaPrint("Setting up paging\n");
    page_directory = (uint32_t*)0x400000;      // Setter page directory til å starte på 4 MB
    page_dir_loc = (uint32_t)page_directory;  // Setter fysisk addresse til page directory
    last_page = (uint32_t *)0x404000;         // Setter last_page til å starte på   4 MB + 4 KB
    for(int i = 0; i < 1024; i++)             // Looper gjennom alle
    {
        page_directory[i] = 0 | 2;            // Setter alle entries i page table adressene til 0 og setter read/write bit. 
    }
    map_virt_to_phys(0, 0);         // Mapper de første 4MB av virtuell til fysisk minne
    map_virt_to_phys(0x400000, 0x400000); // Mapper de neste 4MB av virtuell til fysisk minne
    enable_paging();                         
    mafiaPrint("Paging was successfully enabled!\n");
}
