#include "libc/stdint.h"
#include "idt.h"
#include "irq.h"
#include "snake.h"
#include "terminal.h" // Added for terminal_set_cursor
#include "libc/stdio.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_BUFFER_SIZE 256
#define PIC1_COMMAND 0x20

// External IRQ stubs from irq_stubs.asm
extern void irq0_stub(void); // for PIT
extern void irq1_stub(void); // keyboard
extern void pit_handler(void);

// Debug function to print scancodes
void print_scancode(uint8_t scancode, bool is_extended, bool processed, bool is_release) {
    terminal_set_cursor(0, 50); // Right side of the screen
    printf("Scan:%d Rel: %d Proc: %d", scancode, is_extended, is_release, processed);
}

// Forward declaration of irq1_handler
void irq1_handler(void);

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

    // Unmask IRQ1 (keyboard)
    outb(0x21, inb(0x21) & ~0x02);
}

// game state control
static bool in_game_mode = false;

// function to control game mode
void set_game_mode(bool enabled) {
    in_game_mode = enabled;
}

void irq_handler(int irq) {
    switch (irq) {
        case 0: // PIT
            pit_handler();
            break;
        case 1: // Keyboard
            irq1_handler();
            break;
        default:
            // Handle other IRQs if needed
            break;
    }
    outb(PIC1_COMMAND, 0x20); // Send EOI to master PIC
    if (irq >= 8) {
        outb(0xA0, 0x20); // Send EOI to slave PIC if applicable
    }
}

void irq1_handler(void) {
    static bool is_extended = false;
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    bool processed = false;
    bool is_release = (scancode & 0x80) != 0;

    // Debug print to see scancodes
    print_scancode(scancode, is_extended, processed, is_release);

    // Check for extended key prefix (0xE0)
    if (scancode == 0xE0) {
        is_extended = true;
        outb(PIC1_COMMAND, 0x20);
        return;
    }

    // Handle key releases (scancodes >= 0x80)
    if (is_release) {
        scancode &= 0x7F; // Remove release bit for comparison
        is_extended = false;
        outb(PIC1_COMMAND, 0x20);
        return;
    }

    // Handle game mode differently
    if (in_game_mode) {
        // Process all game controls including direction keys
        if (is_extended) {
            // Handle extended keys (arrow keys)
            switch(scancode) {
                case 0x48: // Up arrow
                    snake_on_key(SCANCODE_UP);
                    processed = true;
                    break;
                case 0x50: // Down arrow
                    snake_on_key(SCANCODE_DOWN);
                    processed = true;
                    break;
                case 0x4B: // Left arrow
                    snake_on_key(SCANCODE_LEFT);
                    processed = true;
                    break;
                case 0x4D: // Right arrow
                    snake_on_key(SCANCODE_RIGHT);
                    processed = true;
                    break;
            }
            is_extended = false;
        } else {
            // Handle regular keys
            switch(scancode) {
                case SCANCODE_S:
                case SCANCODE_P:
                case SCANCODE_R:
                case SCANCODE_ESC:
                    snake_on_key(scancode);
                    processed = true;
                    break;
            }
        }
    } else {
        // Normal terminal mode
        if (!is_extended && scancode < sizeof(scancode_to_ascii)) {
            char ascii = scancode_to_ascii[scancode];
            if (ascii) {
                keyboard_buffer.buffer[keyboard_buffer.write_pos] = ascii;
                keyboard_buffer.write_pos = (keyboard_buffer.write_pos + 1) % KEYBOARD_BUFFER_SIZE;
                terminal_put_char(ascii);
                processed = true;
            }
        }
        is_extended = false;
    }

    // Update debug print to show if the scancode was processed
    print_scancode(scancode, is_extended, processed, is_release);

    outb(PIC1_COMMAND, 0x20);
}

uint8_t keyboard_getchar(void) {
    if (keyboard_buffer.read_pos == keyboard_buffer.write_pos) {
        return 0;  // Buffer is empty
    }
    
    uint8_t scancode = keyboard_buffer.buffer[keyboard_buffer.read_pos];
    keyboard_buffer.read_pos = (keyboard_buffer.read_pos + 1) % KEYBOARD_BUFFER_SIZE;
    return scancode;
}

bool get_game_mode(void) {
    return in_game_mode;
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

    // Unmask IRQ0 (PIT) and IRQ1 (keyboard)
    outb(0x21, 0xFC); // Mask all except IRQ0 and IRQ1
    outb(0xA1, 0xFF); // Mask all on slave
}

void irq_init(void) {
    pic_remap();
    set_idt_entry(32, (uint32_t)irq0_stub, 0x08, 0x8E); // IRQ0 = PIT
    set_idt_entry(33, (uint32_t)irq1_stub, 0x08, 0x8E); // IRQ1 = Keyboard
}