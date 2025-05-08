// Source file for malloc.c based on Per-Arne Andersen's implementation found at https://perara.notion.site/IKT218-Advanced-Operating-Systems-2024-bfa639380abd46389b5d69dcffda597a
#include "memory.h"
#include <libc/stddef.h>
#include <libc/stdint.h>
#include <libc/limits.h>
#include <libc/stdio.h>
#include <libc/string.h>
#include <kernelstuff.h>

#define MAX_PAGE_ALIGNED_ALLOCS 32                            

uint32_t lastAlloc = 0;                                         
uint32_t heapBegin = 0;                                     
uint32_t heapEnd = 0;                                      
uint32_t pheapBegin = 0;                                     
uint32_t pheapEnd = 0;                                     
uint8_t *pheapDesc = 0;                                   
uint32_t memoryUsed = 0;                                      

// Initializes the kernel memory
void initKernelMemory(uint32_t *kernelEnd) {

    lastAlloc = (uint32_t)kernelEnd + 0x1000;             
    heapBegin = lastAlloc;                                  
    pheapEnd = 0x400000;                                       
    pheapBegin = pheapEnd -(MAX_PAGE_ALIGNED_ALLOCS * 4096);    
    heapEnd = pheapBegin;                                       
    memset((char *)heapBegin, 0, heapEnd - heapBegin);          
    pheapDesc = (uint8_t *)malloc(MAX_PAGE_ALIGNED_ALLOCS);    
    printf("Kernel heap starts at 0x%x\n", lastAlloc);          

}


void printMemory() {
    printf("Memory used: %d bytes\n", memoryUsed);                      
    printf("Memory free: %d bytes\n", heapEnd - heapBegin - memoryUsed);
    printf("Heap size: %d bytes\n", heapEnd - heapBegin);
    printf("Heap start: 0x%x\n", heapBegin);
    printf("Heap end: 0x%x\n", heapEnd);
    printf("PHeap start: 0x%x\nPHeap end: 0x%x\n", pheapBegin, pheapEnd);
}



void free(void *memory) {
    alloc_t *alloc = (memory - sizeof(alloc_t));                
    memoryUsed -= alloc->size + sizeof(alloc_t);                

    alloc->status = 0;                                        
}


void pfree(void *memory) {
    if (memory < pheapBegin || memory > pheapEnd){
        return;                                                 
    }
    
    uint32_t ad = (uint32_t)memory;                            
    ad -= pheapBegin;                                          
    ad /= 4096;                                                

    
    pheapDesc[ad] = 0;
}


char* pmalloc(size_t size) {
    for (int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++)
    {
        
        if (pheapDesc[i]) {
            continue; 
        }
    
        pheapDesc[i] = 1;                                                                       
        printf("PAllocated from 0x%x to 0x%x\n", pheapBegin + i*4096, pheapBegin + (i+1)*4096); 
        return (char *)(pheapBegin + i*4096);                                                   
    }

    printf("pmalloc: FATAL: failure!\n"); 
    return 0;
}


void* malloc(size_t size) {
    if(!size){
        return 0;
    } 

    
    uint8_t *memory = (uint8_t *)heapBegin;

    while ((uint32_t)memory < lastAlloc)
    {
        
        alloc_t *a = (alloc_t *)memory;
        printf("mem=0x%x a={.status=%d, .size=%d}\n", memory, a->status, a->size);

        
        if (!a->size) {
            goto nalloc;
        }
            
        
        if (a->status) {
            memory += a->size;                   
            memory += sizeof(alloc_t);          
            memory += 4;                        
            continue;
        }

        
        if (a->size >= size)
        {
            a->status = 1; 
            printf("RE:Allocated %d bytes from 0x%x to 0x%x\n", size, memory + sizeof(alloc_t), memory + sizeof(alloc_t) + size); 
            memset(memory + sizeof(alloc_t), 0, size);                                                                            
            memoryUsed += size + sizeof(alloc_t);                                                                                 
            return (char *)(memory + sizeof(alloc_t));                                                                            
        }

        
        memory += a->size;
        memory += sizeof(alloc_t);
        memory += 4;
    }

    
    nalloc:;
    if (lastAlloc + size + sizeof(alloc_t) >= heapEnd)
    {
        panic("Cannot allocate bytes! Out of memory.\n");
    }


    alloc_t *alloc = (alloc_t *)lastAlloc; 
    alloc->status = 1;                     
    alloc->size = size;                    

    lastAlloc += size;                     
    lastAlloc += sizeof(alloc_t);          
    lastAlloc += 4;                        

    printf("Allocated %d bytes from 0x%x to 0x%x\n", size, (uint32_t)alloc + sizeof(alloc_t), lastAlloc); 

    memoryUsed += size + 4 + sizeof(alloc_t);                       
    memset((char *)((uint32_t)alloc + sizeof(alloc_t)), 0, size);   
    return (char *)((uint32_t)alloc + sizeof(alloc_t));             
}


