#ifndef IDT_H
#define IDT_H

#include <libc/stdint.h>
#include <libc/stddef.h> // for NULL

//extern struct idt_ptr idt_ptr;


// 👇 Legg til denne strukturen, den trengs for IDT-håndtering
struct int_handler {
    int num;
    void (*handler)(void *data);
    void *data;
};

#define IDT_ENTRIES 256

// Struktur for én IDT-entry
struct idt_entry {
    uint16_t base_low;
    uint16_t segment;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

// Struktur for IDT peker
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Funksjoner
void init_idt();
void set_idt_gate(int num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_load(struct idt_ptr* idt_ptr);
void register_int_handler(int num, void (*handler)(void *data), void *data);

#endif
