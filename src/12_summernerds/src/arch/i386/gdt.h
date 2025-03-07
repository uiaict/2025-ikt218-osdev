#include "stdint.h"

struct GDTEntry {
    uint16_t limit_low;   // The lower 16 bits of the limit (lagrer de første minste 16 bitene av limit)
    uint16_t base_low;    // The lower 16 bits of the base (the 16 first bits of the base adress)
    uint8_t base_middle;  // The next 8 bits of the base (the 8 nest bits of the base adress)
    uint8_t access;       // It bestemmer type of segment (example; kode, data, ring-level)
    uint8_t granularity;  // The last 4 bits of the limit in the memory
    uint8_t base_high;    // the highest 8 bits of the base (the last 8 bits of the base adress)
} __attribute__((packed)); //sikrer at kompilatoren ikke legger til ekstra bytes
 typedef struct struct GDTEntry gdt_Ent_t; 


struct GDTPointer { //a pointer to our gdt
    uint16_t limit;   // The limit is the table -1(Forteller hvor stor tabellen er (antall bytes - 1)); så (* 5 -1) 
    uint32_t base;    // Holder minneaddressen til starten av GDT tabellen 
}__attribute__((packed));
typedef struct struct GDTPointer gdt_Ptr_t;

void init_GDT ();
void*set_GDT_Gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

