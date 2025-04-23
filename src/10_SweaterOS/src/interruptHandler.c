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

// External variables from display.c
extern size_t terminal_column;
extern size_t terminal_row;

// External timer handler from programmableIntervalTimer.c
extern void timer_handler(void);

// PIC ports
#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1

// Keyboard ports
#define KEYBOARD_DATA   0x60
#define KEYBOARD_STATUS 0x64

// Keyboard buffer
#define KEYBOARD_BUFFER_SIZE 64
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static volatile int buffer_head = 0;
static volatile int buffer_tail = 0;

// Write a byte to a port
void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Write a word (16-bit) to a port
void outw(uint16_t port, uint16_t value) {
    __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

// Read a byte from a port
uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Read a word (16-bit) from a port
uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Small delay for I/O operations
void io_wait(void) {
    outb(0x80, 0);
}

// Define SCANCODE_ESC
#define SCANCODE_ESC 0x01

/**
 * Initializes the Programmable Interrupt Controller (PIC)
 * 
 * This function initializes the master and slave PICs, remapping IRQs 0-15
 * to interrupts 32-47 to avoid conflicts with CPU exceptions (0-31).
 * 
 * The initialization sequence consists of:
 *   1. Send ICW1 (Initialize + ICW4 needed) to both PICs
 *   2. Send ICW2 (Interrupt Vector Offset) to both PICs
 *   3. Send ICW3 (Master/Slave Wiring Info) to both PICs
 *   4. Send ICW4 (8086 Mode) to both PICs
 *   5. Mask unwanted interrupts
 */
void pic_initialize(void) {
    // Save masks
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    // Start initialization sequence
    outb(PIC1_COMMAND, 0x11);
    io_wait();
    outb(PIC2_COMMAND, 0x11);
    io_wait();

    // Set vector offsets
    outb(PIC1_DATA, 32);    // IRQ 0-7: 32-39
    io_wait();
    outb(PIC2_DATA, 40);    // IRQ 8-15: 40-47
    io_wait();

    // Set up cascading
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    // Set 8086 mode
    outb(PIC1_DATA, 0x01);
    io_wait();
    outb(PIC2_DATA, 0x01);
    io_wait();

    // Restore masks
    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

/**
 * Sends End-of-Interrupt (EOI) signal to PIC
 * 
 * This function sends an EOI signal to the appropriate PIC(s) based on the IRQ number.
 * For IRQs 0-7, only the master PIC needs an EOI.
 * For IRQs 8-15, both the slave and master PICs need an EOI.
 * 
 * @param irq The IRQ number (0-15, NOT the interrupt number 32-47)
 */
void pic_send_eoi(uint8_t irq) {
    if (irq >= 8)
        outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);
}

/**
 * Convert scancode to ASCII (simplified)
 * 
 * This function converts a keyboard scancode to an ASCII character,
 * taking into account the Shift key state for uppercase letters and symbols.
 * 
 * @param scancode The keyboard scancode
 * @return The ASCII character, or 0 if no mapping exists
 */
char scancode_to_ascii(uint8_t scancode) {
    // Handle special keys first
    if (scancode == SCANCODE_ESC) {
        return 27;  // ESC ASCII code
    }

    static const char ascii_table[] = {
        0,    27,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=',  '\b',
        '\t', 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '[',  ']',  '\n',
        0,    'a',  's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '\'', '`',
        0,    '\\', 'z',  'x',  'c',  'v',  'b',  'n',  'm',  ',',  '.',  '/',  0,
        '*',  0,    ' '
    };

    // Only convert if the scancode is within our table range
    if (scancode < sizeof(ascii_table)) {
        return ascii_table[scancode];
    }
    return 0;
}

/**
 * Handle keyboard input
 * 
 * This function is called when a keyboard interrupt occurs.
 * It reads the scancode from the keyboard data port, converts
 * it to an ASCII character if possible, and adds it to the 
 * keyboard buffer for later retrieval.
 */
void keyboard_handler(void) {
    uint8_t scancode = inb(KEYBOARD_DATA);
    
    // Only handle key press events (not releases)
    if (!(scancode & 0x80)) {
        char c = scancode_to_ascii(scancode);
        if (c) {
            int next = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
            if (next != buffer_tail) {
                keyboard_buffer[buffer_head] = c;
                buffer_head = next;
                
                // Echo the character to the screen for immediate feedback
                if (c >= 32 || c == '\n' || c == '\t' || c == '\b') {
                    display_write_char(c);
                }
            }
        }
    }
}

/**
 * Check if keyboard data is available
 * 
 * This function checks if there is data available in the keyboard buffer.
 * 
 * @return 1 if data is available, 0 if empty
 */
int keyboard_data_available(void) {
    return buffer_head != buffer_tail;
}

/**
 * Get a character from the keyboard buffer
 * 
 * This function returns the next character from the keyboard buffer,
 * or 0 if the buffer is empty.
 * 
 * @return The next character from the keyboard buffer, or 0 if empty
 */
char keyboard_getchar(void) {
    if (buffer_head == buffer_tail)
        return 0;
    
    char c = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

/**
 * Initialize interrupts
 * 
 * This function sets up the interrupt system by:
 * 1. Initializing the Interrupt Descriptor Table (IDT)
 * 2. Initializing the Programmable Interrupt Controller (PIC)
 * 3. Initializing the keyboard controller
 * 4. Enabling interrupts
 */
void interrupt_initialize(void) {
    pic_initialize();
    
    // Enable keyboard and timer interrupts
    outb(PIC1_DATA, 0xFC);  // Enable IRQ0 (timer) and IRQ1 (keyboard)
    
    // Enable interrupts
    __asm__ volatile("sti");
}

// Initialize keyboard
void keyboard_initialize(void) {
    buffer_head = 0;
    buffer_tail = 0;
    
    // Clear keyboard buffer
    for (int i = 0; i < KEYBOARD_BUFFER_SIZE; i++) {
        keyboard_buffer[i] = 0;
    }
    
    // Sørg for at tastaturavbrudd er aktivert i PIC
    uint8_t mask = inb(PIC1_DATA);
    mask &= ~(1 << 1);  // Clear bit 1 to enable keyboard interrupt (IRQ1)
    outb(PIC1_DATA, mask);
    io_wait();
    
    // Tøm eventuelle ventende tastaturdataer
    while (inb(KEYBOARD_STATUS) & 0x01) {
        inb(KEYBOARD_DATA);
    }
}

// Check if interrupts are enabled
int interrupts_enabled(void) {
    uint32_t flags;
    __asm__ volatile("pushf; pop %0" : "=r"(flags));
    return (flags & 0x200) != 0;  // Check IF bit
}

// Handle CPU exceptions
void isr_handler(uint32_t esp) {
    // We don't need to handle exceptions for this minimal OS
    (void)esp;  // Avoid unused parameter warning
}

// Handle hardware interrupts
void irq_handler(uint32_t esp) {
    // Get the interrupt number
    uint32_t int_no = *((uint32_t*)(esp + 36));
    uint8_t irq = int_no - 32;
    
    // Handle specific IRQs
    switch (irq) {
        case 0:  // Timer
            timer_handler();
            break;
        case 1:  // Keyboard
            keyboard_handler();
            break;
    }
    
    // Send EOI
    pic_send_eoi(irq);
} 