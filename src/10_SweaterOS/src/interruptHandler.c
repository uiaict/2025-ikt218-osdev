/**
 * Interrupt Handler Implementation
 * 
 * Dette er en sentral del av operativsystemet som håndterer både CPU exceptions
 * og hardware interrupts. Filen inneholder kode for å:
 * 
 * 1. Initialisere og konfigurere PIC (Programmable Interrupt Controller)
 * 2. Håndtere CPU exceptions (f.eks. division by zero, page fault)
 * 3. Håndtere hardware interrupts (f.eks. tastatur, timer)
 * 4. Implementere I/O-funksjoner for kommunikasjon med hardware
 * 5. Håndtere tastaturinput og konvertere scancodes til ASCII
 */

#include "libc/stdint.h"
#include "interruptHandler.h"
#include "descriptorTables.h"
#include "miscFuncs.h"
#include "display.h"

// External timer handler
extern void timer_handler(void);

// Keyboard buffer - power of 2 size for fast modulo with bit masking
#define KEYBOARD_BUFFER_SIZE 64
#define KEYBOARD_BUFFER_MASK (KEYBOARD_BUFFER_SIZE - 1)

// Keyboard buffer implementation
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static volatile uint8_t buffer_read_index = 0;
static volatile uint8_t buffer_write_index = 0;

// Add flag to track SHIFT key state
static volatile uint8_t shift_pressed = 0;

// Direct scancode to ASCII mapping table for US keyboard layout
// Index is the scancode, value is the ASCII character
static const char scancode_map_lower[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Shift key pressed mapping for uppercase and special characters
static const char scancode_map_upper[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Define scancode values for modifier keys
#define SCANCODE_LEFT_SHIFT  0x2A
#define SCANCODE_RIGHT_SHIFT 0x36
#define SCANCODE_CAPS_LOCK   0x3A

// Optimized I/O functions
void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

void outw(uint16_t port, uint16_t value) {
    __asm__ volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Fast I/O delay
void io_wait(void) {
    // Use port 0x80 (unused port) for a short delay
    outb(0x80, 0);
}

// Get ASCII character from scancode
char scancode_to_ascii(uint8_t scancode) {
    // Get raw scancode without the release bit
    uint8_t raw_scancode = scancode & 0x7F;
    
    // Check if it's a valid scancode
    if (raw_scancode >= 128) {
        return 0;
    }
    
    // Use appropriate mapping based on shift state
    if (shift_pressed) {
        return scancode_map_upper[raw_scancode];
    } else {
        return scancode_map_lower[raw_scancode];
    }
}

// Initialize the PIC
void pic_initialize(void) {
    // ICW1: Start initialization sequence
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    io_wait();
    
    // ICW2: Set vector offsets
    outb(PIC1_DATA, 0x20); // IRQs 0-7 use interrupts 0x20-0x27
    outb(PIC2_DATA, 0x28); // IRQs 8-15 use interrupts 0x28-0x2F
    io_wait();
    
    // ICW3: Set up cascading
    outb(PIC1_DATA, 0x04); // Tell master PIC that slave is at IRQ2
    outb(PIC2_DATA, 0x02); // Tell slave PIC its cascade identity
    io_wait();
    
    // ICW4: Set 8086 mode
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    io_wait();
    
    // OCW1: Set interrupt masks - enable timer (IRQ0) and keyboard (IRQ1)
    outb(PIC1_DATA, 0xFC); // Enable IRQ0 (timer) and IRQ1 (keyboard)
    outb(PIC2_DATA, 0xFF); // Disable all slave interrupts
}

// Initialize the interrupt system
void interrupt_initialize(void) {
    pic_initialize();
    
    // Reset keyboard buffer and state
    buffer_read_index = buffer_write_index = 0;
    shift_pressed = 0;
    
    // Clear any pending keyboard data
    while (inb(KEYBOARD_STATUS) & 0x01)
        inb(KEYBOARD_DATA);
    
    // Enable interrupts
    __asm__ volatile("sti");
    
    // Debug message to confirm initialization
    display_write_color("Keyboard initialized and ready\n", COLOR_LIGHT_GREEN);
}

// Check if keyboard buffer has data
int keyboard_data_available(void) {
    return buffer_read_index != buffer_write_index;
}

// Get a character from the keyboard buffer
char keyboard_getchar(void) {
    // Return 0 if buffer is empty
    if (buffer_read_index == buffer_write_index)
        return 0;
    
    // Get character and update read index
    char c = keyboard_buffer[buffer_read_index];
    buffer_read_index = (buffer_read_index + 1) & KEYBOARD_BUFFER_MASK;
    
    return c;
}

// Check if interrupts are enabled
int interrupts_enabled(void) {
    uint32_t flags;
    __asm__ volatile("pushf; pop %0" : "=r"(flags));
    return (flags & 0x200) != 0;
}

// Minimal CPU exception handler
void isr_handler(uint32_t esp) {
    (void)esp;  // Avoid unused parameter warning
}

// Optimized IRQ handler
void irq_handler(uint32_t esp) {
    // Get IRQ number using direct memory access for speed
    uint8_t irq = *((uint8_t*)(esp + 36)) - 32;
    
    // Handle common interrupts with if/else for better branch prediction
    if (irq == 0) {
        // Timer interrupt - most frequent
        timer_handler();
    } 
    else if (irq == 1) {
        // Keyboard interrupt - second most common
        uint8_t scancode = inb(KEYBOARD_DATA);
        
        // Check for shift key press/release
        if (scancode == SCANCODE_LEFT_SHIFT || scancode == SCANCODE_RIGHT_SHIFT) {
            shift_pressed = 1;
        } else if (scancode == (SCANCODE_LEFT_SHIFT | 0x80) || scancode == (SCANCODE_RIGHT_SHIFT | 0x80)) {
            shift_pressed = 0;
        }
        // Regular key press (bit 7 not set means key press, bit 7 set means key release)
        else if (!(scancode & 0x80)) {
            char c = scancode_to_ascii(scancode);
            
            if (c) {
                // Calculate next position with bit masking for fast modulo
                uint8_t next = (buffer_write_index + 1) & KEYBOARD_BUFFER_MASK;
                
                // Only add if buffer not full
                if (next != buffer_read_index) {
                    keyboard_buffer[buffer_write_index] = c;
                    buffer_write_index = next;
                }
            }
        }
    }
    
    // Send End-of-Interrupt signal
    if (irq >= 8)
        outb(PIC2_COMMAND, 0x20); // Send EOI to slave PIC
    outb(PIC1_COMMAND, 0x20);     // Send EOI to master PIC
} 