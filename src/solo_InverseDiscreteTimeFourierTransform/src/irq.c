#include "interrupts.h"
#include "libc/stddef.h"
#include "libc/io.h"


#define IRQ_COUNT   16


#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_INIT    0x11
#define ICW4_8086    0x01



extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);



extern void isr_common(int int_no);



typedef void (*isr_t)(int, void*);
static struct {


    isr_t  handler;
    void  *ctx;
    
} irq_handlers[IRQ_COUNT];



static void pic_remap(void) {


    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    // Start init sequence in cascade mode
    outb(PIC1_COMMAND, ICW1_INIT);
    outb(PIC2_COMMAND, ICW1_INIT);

    // Remap offsets: master→0x20, slave→0x28
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);

    // Tell master/slave their cascade identities
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // Set 8086/88 mode
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // Restore saved masks
    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

extern struct idt_entry_t idt_entries[];
static void idt_set_gate(uint8_t n, uint32_t handler_addr) {
    idt_entries[n].offset_low  = handler_addr & 0xFFFF;
    idt_entries[n].selector    = 0x08;
    idt_entries[n].zero        = 0;
    idt_entries[n].type_attr   = 0x8E;
    idt_entries[n].offset_high = (handler_addr >> 16) & 0xFFFF;
}

void init_irq(void) {


    // 1) Remap the PIC
    pic_remap();

    // 2) Clear any registered callbacks
    for (int i = 0; i < IRQ_COUNT; i++) {
        irq_handlers[i].handler = NULL;
        irq_handlers[i].ctx     = NULL;
    }

    // 3) Install the IRQ stubs into the IDT
    idt_set_gate(IRQ0,  (uint32_t)irq0);
    idt_set_gate(IRQ1,  (uint32_t)irq1);
    idt_set_gate(IRQ2,  (uint32_t)irq2);
    idt_set_gate(IRQ3,  (uint32_t)irq3);
    idt_set_gate(IRQ4,  (uint32_t)irq4);
    idt_set_gate(IRQ5,  (uint32_t)irq5);
    idt_set_gate(IRQ6,  (uint32_t)irq6);
    idt_set_gate(IRQ7,  (uint32_t)irq7);
    idt_set_gate(IRQ8,  (uint32_t)irq8);
    idt_set_gate(IRQ9,  (uint32_t)irq9);
    idt_set_gate(IRQ10, (uint32_t)irq10);
    idt_set_gate(IRQ11, (uint32_t)irq11);
    idt_set_gate(IRQ12, (uint32_t)irq12);
    idt_set_gate(IRQ13, (uint32_t)irq13);
    idt_set_gate(IRQ14, (uint32_t)irq14);
    idt_set_gate(IRQ15, (uint32_t)irq15);

    // 4) Reload  IDT so the new gates take effect
    extern void idt_load(uint32_t);
    extern struct idt_ptr_t idt_ptr;
    idt_load((uint32_t)&idt_ptr);
}

void register_irq_handler(int irq, isr_t handler, void* ctx) {


    if (irq < 0 || irq >= IRQ_COUNT) return;
    irq_handlers[irq].handler = handler;
    irq_handlers[irq].ctx     = ctx;
}

void irq_common(int int_no) {


    // Send EOI to slave if needed
    if (int_no >= IRQ8) {
        outb(PIC2_COMMAND, 0x20);
    }


    // Always send EOI to master
    outb(PIC1_COMMAND, 0x20);

    int irq = int_no - IRQ0;
    if (irq < 0 || irq >= IRQ_COUNT) return;

    if (irq_handlers[irq].handler) {
        irq_handlers[irq].handler(int_no, irq_handlers[irq].ctx);
    }
}
