#include "libc/stdint.h"

//Function decalation for flush
//32 bit unasigneed integer as parameter
//Updates the CPU with the new GDT settings
extern void gdt_flush(uint32_t);

//Each entry in the GDT definesa a memory segment by specifying its base addess, size(limit) and access rights
struct gdt_entry {
    uint16_t limit_low;  //Lower 16 bits of the segment limit.
    uint16_t base_low;   //Lower 16 bits of the segment base address.
    uint8_t base_middle; //Next 8 bits of the segment base address.
    uint8_t access;      //Access flags define the segment's permissions and type.
    uint8_t granularity; //Contains flags (like granularity) and the high 4 bits of the limit.
    uint8_t base_high;   //Last 8 bits of the segment base address.
} __attribute__((packed));

//Tells the CPU where the GDT is and how big it is
struct gdt_ptr {
    uint16_t limit; //Size of the GDT table -1
    uint32_t base;  //Address of the first entry in GDT
} __attribute__((packed));

//Initializes the GDT
void init_gdt();

//Loads the GDT
void gdt_load(struct gdt_ptr *gdt_ptr);

//Populates a specific GDT entry with the parameters provided
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

