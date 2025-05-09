#ifndef GDT_H
#define GDT_H

#include "libc/stdint.h"   // Include standard integer types like uint16_t, uint32_t
#include "libc/stddef.h"   // Include standard definitions
#include "libc/stdbool.h"  // Include standard boolean definitions (true/false)

#define GDT_ENTRIES 5  // We are creating 5 entries (segments) in the GDT

// This structure represents one entry (segment) in the GDT
struct gdt_entry_t {
    uint16_t limit_low;    // The lower part of the segment limit (how much memory this segment can use)
    uint16_t base_low;     // The lower part of the base address (where this segment starts)
    uint8_t base_middle;   // The middle part of the base address
    uint8_t access;        // Defines who can access this segment and how (permissions)
    uint8_t granularity;   // Defines size and other settings for the segment
    uint8_t base_high;     // The upper part of the base address
} __attribute__((packed)); // Prevent the compiler from adding extra bytes (keep it tightly packed)

// This structure tells the CPU where the GDT is in memory and its size
struct gdt_ptr_t {
    uint16_t limit;  // The size of the GDT (in bytes)
    uint32_t base;   // The memory address where the GDT starts
} __attribute__((packed));  // Prevent extra padding bytes

// Function to initialize the GDT (set it up)
void init_gdt();

// Function to load the GDT into the CPU so it can start using it
void gdt_load();

// Function to set the details for one entry (segment) in the GDT
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

// The actual GDT array (5 segments)
static struct gdt_entry_t gdt[GDT_ENTRIES];

// Pointer to the GDT (used to tell the CPU where the GDT is)
static struct gdt_ptr_t gdt_ptr;

#endif  // End of the header file
