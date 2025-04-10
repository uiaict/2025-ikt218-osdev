
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include "printf.h" 
#include <multiboot2.h>

extern void isr0(); 
extern void isr1();
extern void isr14(); 

extern void irq0();  
extern void irq1();  
extern void irq2();  
extern void irq3();  
extern void irq4();  
extern void irq5();  
extern void irq6();  
extern void irq7();  
extern void irq8();  
extern void irq9();  
extern void irq10(); 
extern void irq11(); 
extern void irq12(); 
extern void irq13(); 
extern void irq14(); 
extern void irq15(); 

void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

// NORDIC ISO Norsk øæå
static const unsigned char scancode_ascii[128] = {
      0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 0x00 - 0x09 */
    '9', '0', '+', '\\', '\b', '\t', 'q', 'w', 'e', 'r', /* 0x0A - 0x13 */
    't', 'y', 'u', 'i', 'o', 'p', 0xE5, '^', '\n',   0, /* 0x14 - 0x1D --- 0xE5 = å */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0xF8, /* 0x1E - 0x27 --- 0xF8 = ø */
   0xE6, '|',   0, '\'', 'z', 'x', 'c', 'v', 'b', 'n', /* 0x28 - 0x31 --- 0xE6 = æ */
    'm', ',', '.', '-',   0, '*',   0, ' ',   0,   0, /* 0x32 - 0x3B */
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, /* 0x3C - 0x45 */
      0,   0, '7', '8', '9', '-', '4', '5', '6', '+', /* 0x46 - 0x4F */
    '1', '2', '3', '0', '.',   0,   0, '<',   0,   0, /* 0x50 - 0x59 */
    /* Rest are 0 */
};

typedef struct registers {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags; 
} registers_t;


void irq_handler(registers_t *regs) {
    if (regs->int_no == 33) {
        uint8_t scancode = inb(0x60); 
        if (!(scancode & 0x80)) {
            if (scancode < 128) { // Ensure scancode is within bounds

                unsigned char ascii = scancode_ascii[scancode];

                if (scancode == 0x0E) { // Backspace
                    if (cursor > 0) {
                        cursor--;
                        vga[cursor * 2] = ' ';     
                        vga[cursor * 2 + 1] = 0x07; 
                    }
                } else if (ascii != 0) {
                     if (ascii >= ' ' || ascii >= 0x80) { // ext. chars øæå
                       char msg[2] = {(char)ascii, '\0'};
                       printf(msg);
                    }
                }

            }
        } else {

        }
    }
    else if (regs->int_no == 32) {
    }

    if (regs->int_no >= 40) {
        outb(0xA0, 0x20); // Slave PIC
    }
    outb(0x20, 0x20); // Master PIC
}


void isr_handler(registers_t *regs) {

    printf("CPU EXCEPTION: ");
    switch (regs->int_no) {
        case 0: printf("Divide by zero"); 
        break;
        case 1: printf("Debug"); break;
        case 14: printf("Page Fault"); 
        break;

        default:
            printf("Unhandled Exception ");
            break;
    }

    printf("\nSystem Halted.\n");
    asm volatile ("cli; hlt");
    while(1); 
}

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1
#define PIC_EOI      0x20

#define ICW1_INIT    0x10 
#define ICW1_ICW4    0x01 
#define ICW4_8086    0x01 

void remap_pic() {

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    outb(PIC1_DATA, 0x20); 
    outb(PIC2_DATA, 0x28); 

    outb(PIC1_DATA, 0x04); 
    outb(PIC2_DATA, 0x02); 

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);
    outb(PIC1_DATA, 0b11111000); 
    outb(PIC2_DATA, 0b11111111); 
}

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;      
    uint8_t  always0;  
    uint8_t  flags;    
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256
struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

void idt_set_gate(int n, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[n].base_low  = handler & 0xFFFF;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
    idt[n].sel       = sel;
    idt[n].always0   = 0;
    idt[n].flags     = flags; 
}

void lidt(void* idtp_ptr) {
    asm volatile("lidt (%0)" : : "r"(idtp_ptr));
}

#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_PORT  0x43
#define PIT_FREQUENCY     1193182 


void init_pit(uint32_t frequency) {
    uint32_t divisor = PIT_FREQUENCY / frequency;
    if (frequency == 0) divisor = 65535; 
    if (divisor == 0) divisor = 1; 
    if (divisor > 65535) divisor = 65535; 
    outb(PIT_COMMAND_PORT, 0x36);
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));
}

void init_idt() {
    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;
    idt_set_gate(0,  (uint32_t)isr0,  0x08, 0x8E); 
    idt_set_gate(1,  (uint32_t)isr1,  0x08, 0x8E); 
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E); 
    idt_set_gate(32, (uint32_t)irq0,  0x08, 0x8E); 
    idt_set_gate(33, (uint32_t)irq1,  0x08, 0x8E); 
    idt_set_gate(34, (uint32_t)irq2,  0x08, 0x8E); 
    idt_set_gate(35, (uint32_t)irq3,  0x08, 0x8E); 
    idt_set_gate(36, (uint32_t)irq4,  0x08, 0x8E); 
    idt_set_gate(37, (uint32_t)irq5,  0x08, 0x8E); 
    idt_set_gate(38, (uint32_t)irq6,  0x08, 0x8E); 
    idt_set_gate(39, (uint32_t)irq7,  0x08, 0x8E); 
    idt_set_gate(40, (uint32_t)irq8,  0x08, 0x8E); 
    idt_set_gate(41, (uint32_t)irq9,  0x08, 0x8E); 
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E); 
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E); 

    lidt(&idtp);
    remap_pic();

}