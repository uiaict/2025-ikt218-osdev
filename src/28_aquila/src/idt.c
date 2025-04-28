
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include "libc/string.h"

#include "printf.h" 
#include "kernel/pit.h"
#include <multiboot2.h>
#include "buffer.h"


bool left_shift_pressed = false;
bool right_shift_pressed = false;
bool caps_lock_on = false;
bool left_ctrl_pressed = false;


 

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

extern int input_cursor;
extern int input_len;
extern int in_nano; // 0 = aquila, 1 = program mode

extern volatile uint32_t tick;

void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

// NORDIC ISO Norsk øæå (Unshifted) - CP437
static const unsigned char scancode_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', // 0x00 - 0x09 
  '9', '0', '+', '\\', '\b', '\t', 'q', 'w', 'e', 'r', // 0x0A - 0x13 
  't', 'y', 'u', 'i', 'o', 'p', 0x86, '^', '\n',   0, // 0x14 - 0x1D --- 0x1A -> 0x86 = CP437 'å' 
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0xF8, // 0x1E - 0x27 --- 0x27 -> 0xF8 = CP437 '°' (placeholder for ø) 
 0x91, '|',   0, '\'', 'z', 'x', 'c', 'v', 'b', 'n', // 0x28 - 0x31 --- 0x28 -> 0x91 = CP437 'æ' 
  'm', ',', '.', '-',   0, '*',   0, ' ',   0,   0, // 0x32 - 0x3B 
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x3C - 0x45 
    0,   0, '7', '8', '9', '-', '4', '5', '6', '+', // 0x46 - 0x4F Numpad
  '1', '2', '3', '0', '.',   0,   0, '<',   0,   0, // 0x50 - 0x59 
  // Rest are 0
};

// NORDIC ISO Norsk ØÆÅ (Shifted) - CP437
static const unsigned char scancode_ascii_shifted[128] = {
    0,  27, '!', '"', '#', '$', '%', '&', '/', '(', // 0x00 - 0x09
  ')', '=', '?', '`', '\b', '\t', 'Q', 'W', 'E', 'R', // 0x0A - 0x13
  'T', 'Y', 'U', 'I', 'O', 'P', 0x8F, '*', '\n',   0, // 0x14 - 0x1D --- 0x1A -> 0x8F = CP437 'Å'
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0xF8, // 0x1E - 0x27 --- 0x27 -> 0xF8 = CP437 '°' (placeholder for Ø)
 0x92, '§',   0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', // 0x28 - 0x31 --- 0x28 -> 0x92 = CP437 'Æ'
  'M', ';', ':', '_',   0, '*',   0, ' ',   0,   0, // 0x32 - 0x3B
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x3C - 0x45 
    0,   0, '7', '8', '9', '-', '4', '5', '6', '+', // 0x46 - 0x4F Numpad
  '1', '2', '3', '0', '.',   0,   0, '>',   0,   0, // 0x50 - 0x59 
   // Rest are 0
};

typedef struct registers {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags; 
} registers_t;

extern int input_start = 0;




void irq_handler(registers_t *regs) {

    if (regs->int_no == 33) { // Keyboard interrupt IRQ 1
        uint8_t scancode = inb(0x60);

        // Handle modifier key presses/releases
        if (scancode == 0x2A) { left_shift_pressed = true; }         // pressed=true
        else if (scancode == 0xAA) { left_shift_pressed = false; }   // relased=false
        else if (scancode == 0x36) { right_shift_pressed = true; }
        else if (scancode == 0xB6) { right_shift_pressed = false; }
        else if (scancode == 0x1D) { left_ctrl_pressed = true; }       // Left Ctrl down
        else if (scancode == 0x9D) { left_ctrl_pressed = false; } // Left Ctrl up
        else if (scancode == 0x3A) { // caps lock
            caps_lock_on = !caps_lock_on;
        }
        else if (!(scancode & 0x80)) {
            if (scancode == 0x2D && (left_ctrl_pressed)) {
                in_nano = 0;
                close_nano(); // Close nano editor
            } 
            else if (scancode < 128) { // Ensure scancode is within bounds

                bool shift_held = left_shift_pressed || right_shift_pressed;
                unsigned char ascii_raw = scancode_ascii[scancode];
                unsigned char ascii = 0;

                bool effective_shift = shift_held;
                if ((ascii_raw >= 'a' && ascii_raw <= 'z') || 
                 ascii_raw == 0x86 ||                     // CP437 å
                 ascii_raw == 0x91)                       // CP437 æ
                 {
                    effective_shift = shift_held ^ caps_lock_on;
                }

                // Select shift or not ascii table
                if (effective_shift) {
                    ascii = scancode_ascii_shifted[scancode];
                } else {
                    ascii = ascii_raw;
                }

                // Enter handling, with prefix aquila, terminal look
                if (scancode == 0x1C) { // Enter key
                    printf("\n");
                    buffer_handler(4, 0); // Enter key pressed
                }

                

                if (scancode == 0x53) { // Delete key
                    int row = cursor / VGA_WIDTH;
                    int col = cursor % VGA_WIDTH;
                
                    // Only delete if there are characters after the cursor on the line
                    if (col < VGA_WIDTH - 1) {
                        for (int x = col; x < VGA_WIDTH - 1; x++) {
                            int current = (row * VGA_WIDTH + x) * 2;
                            int next = (row * VGA_WIDTH + (x + 1)) * 2;
                            vga[current] = vga[next];
                            vga[current + 1] = vga[next + 1];
                        }
                
                        // Clear last character on line
                        int last = (row * VGA_WIDTH + (VGA_WIDTH - 1)) * 2;
                        vga[last] = ' ';
                        vga[last + 1] = 0x07;
                        
                
                        update_cursor(col, row); // Keep cursor in same place
                        if (input_cursor < input_len) {
                            buffer_handler(3, 0); // move buffer cursor to right
                            buffer_handler(1, 0); // Remove character from buffer
                        }
                    }
                } else if (scancode == 0x4B) { // Left arrow
                    if (cursor > input_start) {
                        cursor--;
                        int x = cursor % 80;
                        int y = cursor / 80;
                        update_cursor(x, y);
                        buffer_handler(2, 0); // Move buffer cursor to left
                    }
                } else if (scancode == 0x4D) { // Right arrow
                    if (cursor < VGA_WIDTH * VGA_HEIGHT) {
                        cursor++;
                        int x = cursor % 80;
                        int y = cursor / 80;
                        update_cursor(x, y);
                        buffer_handler(3, 0); // Move buffer cursor to right
                    }
                } else

                if (scancode == 0x0E) { // Backspace
                    if (cursor > input_start) {
                        cursor--; // Move left to the character to delete
                
                        int row = cursor / VGA_WIDTH;
                        int col = cursor % VGA_WIDTH;
                
                        // Shift characters on the line one left from the cursor position
                        for (int x = col; x < VGA_WIDTH - 1; x++) {
                            int curr = (row * VGA_WIDTH + x) * 2;
                            int next = (row * VGA_WIDTH + (x + 1)) * 2;
                            vga[curr] = vga[next];
                            vga[curr + 1] = vga[next + 1];
                        }
                
                        // Clear the last character in the line
                        int last = (row * VGA_WIDTH + (VGA_WIDTH - 1)) * 2;
                        vga[last] = ' ';
                        vga[last + 1] = 0x07;
                
                        // Update cursor
                        update_cursor(col, row);
                        buffer_handler(1, 0); // Remove character from buffer
                    }
                }
                else if (ascii != 0) {
                    if (ascii >= ' ' || ascii >= 0x80) { // ext. chars øæå
                        int row = cursor / VGA_WIDTH;
                        int col = cursor % VGA_WIDTH;
                
                        // Shift all characters one position to the right on the same line
                        for (int x = VGA_WIDTH - 2; x >= col; x--) { // -2 to leave space at end
                            int curr = (row * VGA_WIDTH + (x + 1)) * 2;
                            int prev = (row * VGA_WIDTH + x) * 2;
                            vga[curr] = vga[prev];
                            vga[curr + 1] = vga[prev + 1];
                        }
                    
                        
                        char msg[2] = {(char)ascii, '\0'};
                        printf(msg);
                        buffer_handler(0, ascii); // Update buffer
                                        
                        // Update hardware cursor
                        update_cursor(cursor % VGA_WIDTH, cursor / VGA_WIDTH);
                    }
                }
                
            }
        }
    }
    else if (regs->int_no == 32) {
        tick++;
    } 

    if (regs->int_no >= 40) { // If IRQ involved the slave PIC (IRQ 8-15)
        outb(0xA0, 0x20); // Slave PIC EOI
    }
    outb(0x20, 0x20); // Master PIC EOI
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