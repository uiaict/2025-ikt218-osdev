// Source file for memory.c based on Per-Arne Andersen's implementation found at https://perara.notion.site/IKT218-Advanced-Operating-Systems-2024-bfa639380abd46389b5d69dcffda597a
#include "memory.h"
#include <libc/stddef.h>
#include <libc/stdint.h>
#include <libc/limits.h>
#include <libc/stdio.h>
#include <libc/string.h>
#include <isr.h>
#include <kernelstuff.h>


static uint32_t* pageDir = 0;                               
static uint32_t pageDirLoc = 0;                     
static uint32_t* lastPage = 0;                             

void pagingMap(uint32_t virt, uint32_t phys) {
    uint16_t id = virt >> 22;                               

    for (int i = 0; i < 1024; i++){                        
        lastPage[i] = phys | 3;                           
        phys += 4096;                                       
    } 

    pageDir[id] = ((uint32_t)lastPage) | 3;                 
    lastPage = (uint32_t*)(((uint32_t)lastPage) + 4096);    
}

void enablePaging() {
    asm volatile("mov %%eax, %%cr3": :"a"(pageDirLoc));    
    asm volatile("mov %cr0, %eax");                         
    asm volatile("orl $0x80000000, %eax");                 
    asm volatile("mov %eax, %cr0");                         
}


void initPaging() {
    printf("Initializing paging\n");                       
    registerInterruptHandler(14, &pageFaultHandler);        
    pageDir = (uint32_t*)0x400000;                          
    pageDirLoc = (uint32_t)pageDir;                         
    lastPage = (uint32_t*)0x401000;                        

    for (int i = 0; i < 1024; i++){                         
        pageDir[i] = 0 | 2;                                 
    }      
        
    pagingMap(0, 0);                           
    pagingMap(0x400000, 0x400000);             
    enablePaging();                                        
    printf("Paging initialized\n");                         
}

// Page fault handler based on James Molloy's implementation found at https://archive.is/8MXkb#selection-3583.1-3621.4
void pageFaultHandler(registers_t regs)
{
   
   uint32_t faultAddress;
   asm volatile("mov %%cr2, %0" : "=r" (faultAddress));   
   int present  = !(regs.errCode & 0x1);                    
   int rw = regs.errCode & 0x2;                            
   int us = regs.errCode & 0x4;                             
   int reserved = regs.errCode & 0x8;                      
   int id = regs.errCode & 0x10;                           

   printf("Page fault! ( ");                              
   if (present) {printf("present ");}                      
   if (rw) {printf("read-only ");}                        
   if (us) {printf("user-mode ");}                          
   if (reserved) {printf("reserved ");}                    
   printf(") at 0x");
   printf("%x", faultAddress);                             
   printf("\n");
   panic("Page fault");                                 
}