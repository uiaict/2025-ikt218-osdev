#include "libc/stdint.h"


struct GDT{

    uint16_t limit; 
    uint16_t lower_base;
    uint8_t middle_base;
    uint8_t access; 
    uint8_t flags;
    uint8_t high_base;
    }__attribute__((packed));

    struct GDT_ptr
{
    uint16_t limit;
    unsigned base;
}__attribute__((packed));

void initGdt();
void setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
extern void gdt_flush(struct GDT_ptr*);