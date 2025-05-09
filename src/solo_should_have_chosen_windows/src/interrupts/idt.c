#include "interrupts/isr.h"
#include "interrupts/pic.h"
#include "interrupts/idt_structs.h"
#include "interrupts/idt_function.h"
#include "terminal/print.h"
#include "libc/string.h"
#include "libc/io.h"

#define IDT_ENTRIES 256

static idt_entry_t idt[IDT_ENTRIES];
static idt_ptr_t idt_ptr;

// Function to set an IDT entry
void set_idt_entry(int index, uint32_t isr_function, uint16_t selector, uint8_t type_attr) {
    idt[index].offset_low = isr_function & 0xFFFF;          
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].type_attr = type_attr;
    idt[index].offset_high = (isr_function >> 16) & 0xFFFF;
}


void idt_init() {

    memset(&idt, 0, sizeof(idt)); // Clear IDT memory

    // Define the IDT by using the pointer
    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_ENTRIES) - 1;
    idt_ptr.base = (uint32_t)&idt;

    // Remap the PIC
    pic_remap(0x20, 0x28);

    for (int i = 0; i < 256; ++i) {
        set_idt_entry(i, (uint32_t)isr_stubs[i], 0x08, 0x8E);
    }

    // Load the IDT
    __asm__ volatile ("lidt %0" : : "m" (idt_ptr));

}

void enable_interrupts() {
    __asm__ volatile ("sti");
}

void keyboard_flush_buffer() {
    while (inb(0x64) & 0x01) {
        inb(0x60);
    }
}