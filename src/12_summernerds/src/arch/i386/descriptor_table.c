#include <stdint.h>

#define GDT_ENTRIES 5
#define IDT_ENTRIES 256

// Define the GDT entry structure
///////////////////////////////////////////////////////////////////////////ikke ferdig
// Define the IDT entry structure
struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));




//Under we define pointers for gdt and idt:
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


void init_idt();
void idt_load(struct idt_ptr *idt_ptr);
void interrupt_handler();int_handlers