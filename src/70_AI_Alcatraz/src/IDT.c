#include "IDT.h"
#include "printf.h"
#include "libc/stdbool.h"  // Added this include for bool type
#include "libc/stdint.h"

// Declare IDT entries array and pointer
struct idt_entry idt_entries[256];
struct idt_ptr idtp;

// Array of function pointers for interrupt handlers
isr_t interrupt_handlers[256];

// (Interrupt 0)
void divide_by_zero_handler(registers_t* regs) {
    printf("Divide by zero exception (#DE) occurred!\n");
    printf("Error code: %d\n", regs->err_code);
}

// (Interrupt 3)
void breakpoint_handler(registers_t* regs) {
    printf("Breakpoint exception (#BP) occurred!\n");
    printf("Error code: %d\n", regs->err_code);
}

//  (Interrupt 13)
void general_protection_fault_handler(registers_t* regs) {
    printf("General Protection Fault (#GP) occurred!\n");
    printf("Error code: %d\n", regs->err_code);
}

// PIC ports
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

// PIC commands
#define PIC_EOI      0x20
#define ICW1_INIT    0x11
#define ICW4_8086    0x01

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Remap the PIC to avoid conflicts with CPU exceptions
void irq_remap() {
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT);
    outb(PIC2_COMMAND, ICW1_INIT);
    outb(PIC1_DATA, 32);      
    outb(PIC2_DATA, 40);      
    outb(PIC1_DATA, 4);      
    outb(PIC2_DATA, 2);       
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}


void timer_handler(registers_t* regs) {
    static uint32_t tick = 0;
    tick++;
    if (tick % 100 == 0) {  
        printf("Timer tick: %d\n", tick);
    }
}


#define KEYBOARD_BUFFER_SIZE 64
char keyboard_buffer[KEYBOARD_BUFFER_SIZE]; 
int buffer_position = 0;                    
bool caps_lock_on = false;                  


static const char scancode_to_ascii_low[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


static const char scancode_to_ascii_high[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


#define SCANCODE_SHIFT_LEFT   0x2A
#define SCANCODE_SHIFT_RIGHT  0x36
#define SCANCODE_CAPS_LOCK    0x3A
#define SCANCODE_RELEASE      0x80

// Function to add a character to the keyboard buffer
void add_to_buffer(char c) {
    if (buffer_position < KEYBOARD_BUFFER_SIZE - 1) {
        keyboard_buffer[buffer_position++] = c;
        keyboard_buffer[buffer_position] = '\0';
        putchar(c); 
    }
}

// Function to handle backspace
void handle_backspace() {
    if (buffer_position > 0) {
        if (cursor_x > 0) {
            cursor_x--;
        } else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
        }
        
        terminal_buffer[cursor_y * VGA_WIDTH + cursor_x] = (WHITE_ON_BLACK << 8) | ' ';
        
        buffer_position--;
        keyboard_buffer[buffer_position] = '\0';
        
        move_cursor();
    }
}

// (IRQ1)
void keyboard_handler(registers_t* regs) {
    uint8_t scancode = inb(0x60);
    
    if (scancode & SCANCODE_RELEASE) {
        uint8_t released_key = scancode & ~SCANCODE_RELEASE;
        
        if (released_key == SCANCODE_SHIFT_LEFT || released_key == SCANCODE_SHIFT_RIGHT) {
            shift_pressed = false;
        }
        
        return; 
    }
    
    switch (scancode) {
        case SCANCODE_SHIFT_LEFT:
        case SCANCODE_SHIFT_RIGHT:
            shift_pressed = true;
            break;
            
        case SCANCODE_CAPS_LOCK:
            caps_lock_on = !caps_lock_on; 
            break;
        
        case 0x0E: 
            handle_backspace();
            break;
            
        default:
            if (scancode < sizeof(scancode_to_ascii_low)) {
    
                bool use_upper = shift_pressed ^ caps_lock_on;
                
                char c;
                if (use_upper) {
                    c = scancode_to_ascii_high[scancode];
                } else {
                    c = scancode_to_ascii_low[scancode];
                }
                
                if (c != 0) {
                    add_to_buffer(c);
                }
            }
            break;
    }
    

}

// Send EOI (End of Interrupt) signal to PICs
void irq_handler(registers_t regs) {
    if (regs.int_no >= 40) {
        outb(PIC2_COMMAND, PIC_EOI);  
    }
    outb(PIC1_COMMAND, PIC_EOI);      

    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(&regs);
    } else {
        printf("Unhandled IRQ: %d\n", regs.int_no - 32);
    }
}

// Set an IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].selector = sel;
    idt_entries[num].zero = 0;
    idt_entries[num].flags = flags;
}

// Initialize the IRQ system
void irq_init() {
    irq_remap();

    // Register IRQ gates in the IDT
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    
    register_interrupt_handler(32, timer_handler);    
    register_interrupt_handler(33, keyboard_handler); 

    
    asm volatile("sti");
}

// Initialize the IDT
void idt_init() {
    
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t)&idt_entries;

    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
        interrupt_handlers[i] = 0;
    }

    // Set up the IDT entries for ISRs
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);   
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);   
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);   
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);   
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);   
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);   
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);   
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);  
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);  
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);   
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E); 
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E); 
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E); 
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E); 
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E); 
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E); 
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E); 
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E); 
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E); 
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E); 
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E); 
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E); 
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E); 
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E); 
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E); 
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E); 
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E); 
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E); 
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E); 
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E); 
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E); 
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E); 

    // Load the IDT
    idt_flush((uint32_t)&idtp);
    
    
    register_interrupt_handler(0, divide_by_zero_handler);  
    register_interrupt_handler(3, breakpoint_handler);      
    register_interrupt_handler(13, general_protection_fault_handler); 
    
   
    irq_init();
}


void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}


void isr_handler(registers_t regs) {
    
    printf("Received interrupt: %d\n", regs.int_no);

    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(&regs);
    }
}
