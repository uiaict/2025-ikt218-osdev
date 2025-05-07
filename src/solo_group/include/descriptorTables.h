#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

#include "libc/stdint.h"

#define GDT_ENTRIES 3   // Number of GDT entries, in this case null, code and data
#define IDT_ENTRIES 256

// A GDT entry, describes a memory segment
struct gdtEntry {
    uint16_t limitLow;      // The lower 16 bits of the segment limit
    uint16_t baseLow;       // The lower 16 bits of the base address
    uint8_t baseMiddle;     // The next 8 bits of the base address
    uint8_t access;         // Flag for access/ring
    uint8_t granularity;    // Flags: limit high 4 bits, granularity, size
    uint8_t baseHigh;       // The upper 8 bits of the base address
} __attribute__((packed));  // packed = No padding

struct idtEntry {
	uint16_t    baseLow;      // The lower 16 bits of the ISR's address
	uint16_t    selector;    // The Idt segment selector that the CPU will load into CS before calling the ISR
	uint8_t     zero;     // Set to zero
	uint8_t     flags;   // Type and attributes; see the IDT page
	uint16_t    baseHigh;     // The higher 16 bits of the ISR's address
} __attribute__((packed));

// A pointer to the GDT passed to lgdt
struct gdtPtr {
    uint16_t limit; // Size of the GDT
    uint32_t base;  // Address of the first GDT entry
} __attribute__((packed));


// A pointer to the --- passed to ---
struct idtPtr {
    uint16_t limit; // Size of the ---
    uint32_t base;  // Address of the first --- entry
} __attribute__((packed));




void initGdt(); // Initializes and loads the GDT
void initIdt(); // Initializes and loads the Idt


void gdtLoad(struct gdtPtr *gp);     // Load the GDT into GDTR using `lgdt`
void idtLoad(struct idtPtr *gp);     // Load the Idt into IdtR using `lIdt`



void gdtSetGate(int32_t num, uint32_t base, uint32_t limit, 
    uint8_t access, uint8_t gran);  // Fills a GDT entry
void idtSetGate(int32_t num, uint32_t base, uint8_t flags); // Fills a Idt entry


void gdtFlush(uint32_t gp); // Reloades segment register with the new GDT
void idtFlush(uint32_t ip); // Reloades segment register with the new Idt

#endif