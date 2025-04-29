#include "libc/stdint.h"

#define GDT_ENTRIES 3   // Number of GDT entries, in this case null, code and data

// A GDT entry, describes a memory segment
struct gdtEntry {
    uint16_t limitLow;      // The lower 16 bits of the segment limit
    uint16_t baseLow;       // The lower 16 bits of the base address
    uint8_t baseMiddle;     // The next 8 bits of the base address
    uint8_t access;         // Flag for access/ring
    uint8_t granularity;    // Flags: limit high 4 bits, granularity, size
    uint8_t baseHigh;       // The upper 8 bits of the base address
} __attribute__((packed));  // packed = No padding

// A pointer to the GDT passed to lgdt
struct gdtPtr {
    uint16_t limit; // Size of the GDT
    uint32_t base;  // Address of the first GDT entry
} __attribute__((packed));

void initGdt(); // Initializes and loads the GDT
void gdtLoad(struct gdtPtr *gp);     // Load the GDT into GDTR using `lgdt`
void gdtSetGate(int32_t num, uint32_t base, uint32_t limit, 
    uint8_t access, uint8_t gran);  // Fills a GDT entry
void gdtFlush(uint32_t gp); // Reloades segment register with the new GDT