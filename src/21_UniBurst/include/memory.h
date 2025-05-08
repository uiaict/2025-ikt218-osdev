// Header file for memory functions based on Per-Arne Andersen's implementation found at https://perara.notion.site/IKT218-Advanced-Operating-Systems-2024-bfa639380abd46389b5d69dcffda597a
#ifndef MEMORY_H
#define MEMORY_H

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "isr.h"

// memory allocation struct
typedef struct {
    uint8_t status;
    uint32_t size;
} alloc_t;


void initKernelMemory(uint32_t* kernelEnd);                             

extern void initPaging();                                               
extern void pagingMap(uint32_t virt, uint32_t phys);       

extern char* pmalloc(size_t size);                                     
extern void* malloc(size_t size);                                  
extern void free(void *memory);                                     



extern void* memcpy(void* dest, const void* src, size_t num );         
extern void* memset (void * ptr, int value, size_t num );              
extern void* memset16 (void *ptr, uint16_t value, size_t num);         


void printMemory();                                              

void pageFaultHandler(registers_t reg);                                

#endif