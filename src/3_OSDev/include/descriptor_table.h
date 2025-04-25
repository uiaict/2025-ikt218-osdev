#ifndef DESCRIPTOR_TABLE_H
#define DESCRIPTOR_TABLE_H

#include <libc/stdint.h>
#include <libc/stdint.h>
#include <libc/stddef.h>

#define GDT_ENTRIES 5

extern void gdt_flush(uint32_t);

// Define the entry structures
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
  } __attribute__((packed));

  // Define the pointers
struct gdt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

void init_gdt();
void gdt_load(struct gdt_ptr *gdt_ptr);
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

static struct gdt_entry gdt[GDT_ENTRIES];
static struct gdt_ptr gdt_ptr;

// ---------------------------- IDT ---------------------------- //
#define IDT_ENTRIES 256

// Define the entry structure
struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct int_handler {
    int num;
    void (*handler)(void *data);
    void *data;
  };

void init_idt();
void idt_load(struct idt_ptr *idt_ptr);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);

void register_int_handler(int num, void (*handler)(void *data), void *data);
void default_int_handler(void *data);
void int_handler(int num);

static struct int_handler int_handlers[IDT_ENTRIES];
static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idt_ptr;

#endif /* DESCRIPTOR_TABLE_H */