#ifndef GDT_H
#define GDT_H

#include <libc/stdint.h>

/* ---- 8-byte GDT descriptor -------------------------------------------- */
struct gdt_entry {
    uint16_t limit_low;      /* bits 0-15  of limit  */
    uint16_t base_low;       /* bits 0-15  of base   */
    uint8_t  base_mid;       /* bits 16-23 of base   */
    uint8_t  access;         /* access flags         */
    uint8_t  granularity;    /* flags + limit 16-19  */
    uint8_t  base_high;      /* bits 24-31 of base   */
} __attribute__((packed));

/* ---- GDTR image -------------------------------------------------------- */
struct gdt_ptr {
    uint16_t limit;          /* size-1 of the table  */
    uint32_t base;           /* linear addr of table */
} __attribute__((packed));

/* Initialise GDT and enter flat protected mode */
void gdt_init(void);

#endif /* GDT_H */
