#include "gdt.h"

// Tre segmentbeskrivelser: null, kjerne-kode, kjerne-data
static struct gdt_entry descriptors[3];
static struct gdt_ptr gdt_descriptor;

// Ekstern funksjon i assembly (se gdt.asm)
extern void gdt_flush(uint32_t gdt_ptr_address);

// Setter opp en enkelt GDT-beskrivelse (8 bytes)
static void encode_descriptor(int index, uint32_t base, uint32_t limit,
                              uint8_t access, uint8_t flags)
{
    descriptors[index].base_low    = base & 0xFFFF;
    descriptors[index].base_mid    = (base >> 16) & 0xFF;
    descriptors[index].base_high   = (base >> 24) & 0xFF;

    descriptors[index].limit_low   = limit & 0xFFFF;
    descriptors[index].granularity = ((limit >> 16) & 0x0F) | (flags & 0xF0);
    descriptors[index].access      = access;
}

// Initialiserer GDT og laster den inn
void gdt_init(void)
{
    // Nullsegmentet må være først
    encode_descriptor(0, 0, 0, 0, 0);

    // Segment for ring-0 kode, base 0, maks grense (4 GiB)
    encode_descriptor(1, 0, 0xFFFFF, 0x9A, 0xCF);

    // Segment for ring-0 data, base 0, maks grense (4 GiB)
    encode_descriptor(2, 0, 0xFFFFF, 0x92, 0xCF);

    gdt_descriptor.limit = sizeof(descriptors) - 1;
    gdt_descriptor.base  = (uint32_t)&descriptors;

    // Laster GDTR og oppdaterer segmentregister (via ASM)
    gdt_flush((uint32_t)&gdt_descriptor);
}
