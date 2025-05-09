#ifndef GDT_H
#define GDT_H 

#include<libc/stdint.h>

struct GdtEntry
{
    uint16_t limit_low; 
    uint16_t base_low; 
    uint8_t base_middle;   
    uint8_t access; 
    uint8_t granularity; 
    
    #ifdef __x86_64__
    uint32_t base_high; 
    uint32_t reserved;  
    #else
    uint8_t  base_high; 
    #endif
}__attribute__((packed));

struct GdtPtr 
{
    uint16_t limit; 

    #ifdef __x86_64__
    uint64_t base;   
    #else
    uint32_t base;   
    #endif
}__attribute__((packed)); 

void init_gdt();

#ifdef __x86_64__
void gdt_flush(uint64_t gdt_ptr);
#else
void gdt_flush(uint32_t gdt_ptr);
#endif

#endif
