#include "libc/stdint.h"
#include "idt.h"
#include "irq.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_BUFFER_SIZE 256
#define PIC1_COMMAND 0x20

// External IRQ stubs from irq_stubs.asm
extern void irq1_stub(void);

void terminal_write(const char* str);
void terminal_put_char(char c);

static struct {
    char buffer[KEYBOARD_BUFFER_SIZE];
    int write_pos;
    int read_pos;
} keyboard_buffer = {
    .write_pos = 0,
    .read_pos = 0
};

// I/O port functions
static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Simple US QWERTY scancode to ASCII mapping
static char scancode_to_ascii[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // Backspace
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',   // Enter
    0,   // Control
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/', 0, '*',
    0,  ' ', // Space
    // Rest are zero or extended keys
};

void initkeyboard() {
    uint8_t status;

    // Reset the keyboard controller
    while (inb(0x64) & 0x1) {
        inb(0x60);    // Clear keyboard buffer
    }

    // Enable keyboard interrupts
    outb(0x64, 0x20);    // Read command byte
    status = inb(0x60);
    status |= 1;         // Enable IRQ1
    outb(0x64, 0x60);    // Write command byte
    outb(0x60, status);

    // Unmask keyboard IRQ
    outb(0x21, inb(0x21) & ~0x02);
}

void irq1_handler(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    // Ignore key releases (scancodes >= 0x80)
    if (scancode & 0x80) {
        outb(PIC1_COMMAND, 0x20); // Send EOI
        return;
    }

    // Convert scancode to ASCII
    if (scancode < sizeof(scancode_to_ascii)) {
        char ascii = scancode_to_ascii[scancode];
        if (ascii) {
            // Store in buffer
            keyboard_buffer.buffer[keyboard_buffer.write_pos] = ascii;
            keyboard_buffer.write_pos = (keyboard_buffer.write_pos + 1) % KEYBOARD_BUFFER_SIZE;

            // Display the character
            terminal_put_char(ascii);
        }
    }

    // Send End Of Interrupt to PIC
    outb(PIC1_COMMAND, 0x20);
}

char keyboard_getchar(void) {
    if (keyboard_buffer.read_pos == keyboard_buffer.write_pos) {
        return 0; // Buffer is empty
    }
    
    char c = keyboard_buffer.buffer[keyboard_buffer.read_pos];
    keyboard_buffer.read_pos = (keyboard_buffer.read_pos + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

void pic_remap() {
    outb(0x20, 0x11); // Master command
    outb(0xA0, 0x11); // Slave command

    outb(0x21, 0x20); // Master offset 0x20
    outb(0xA1, 0x28); // Slave offset 0x28

    outb(0x21, 0x04); // Tell Master about Slave at IRQ2
    outb(0xA1, 0x02); // Tell Slave its cascade ID

    outb(0x21, 0x01); // 8086 mode
    outb(0xA1, 0x01); // 8086 mode

    // Unmask IRQ1 (keyboard) only
    outb(0x21, 0xFD); // Mask all except IRQ1
    outb(0xA1, 0xFF); // Mask all on slave
}

void irq_init(void) {
    pic_remap();
    set_idt_entry(33, (uint32_t)irq1_stub, 0x08, 0x8E);
}