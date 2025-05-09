#include "libc/stdint.h"
#include "interruptHandler.h"
#include "descriptorTables.h"
#include "miscFuncs.h"
#include "display.h"

// Ekstern timer handler
extern void timer_handler(void);

// Tastatur buffer - størrelse må være 2^n for rask modulo med bitmasking
#define KEYBOARD_BUFFER_SIZE 64
#define KEYBOARD_BUFFER_MASK (KEYBOARD_BUFFER_SIZE - 1)

// Tastatur buffer implementasjon
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static volatile uint8_t buffer_read_index = 0;
static volatile uint8_t buffer_write_index = 0;

// Holder styr på om SHIFT tasten er trykket
static volatile uint8_t shift_pressed = 0;

// Scancode til ASCII mapping for US tastatur (små bokstaver)
static const char scancode_map_lower[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Scancode mapping for store bokstaver og spesialtegn
static const char scancode_map_upper[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Scancode verdier for spesialtaster
#define SCANCODE_LEFT_SHIFT  0x2A
#define SCANCODE_RIGHT_SHIFT 0x36
#define SCANCODE_CAPS_LOCK   0x3A

// Rask I/O funksjoner
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

// Rask I/O forsinkelse
void io_wait(void) {
    outb(0x80, 0);
}

// Konverterer scancode til ASCII
char scancode_to_ascii(uint8_t scancode) {
    uint8_t raw_scancode = scancode & 0x7F;
    
    if (raw_scancode >= 128) {
        return 0;
    }
    
    if (shift_pressed) {
        return scancode_map_upper[raw_scancode];
    } else {
        return scancode_map_lower[raw_scancode];
    }
}

// Initialiserer PIC
void pic_initialize(void) {
    // ICW1: Start initialiseringssekvens
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    io_wait();
    
    // ICW2: Sett vektor offset
    outb(PIC1_DATA, 0x20); // IRQ 0-7 bruker interrupts 0x20-0x27
    outb(PIC2_DATA, 0x28); // IRQ 8-15 bruker interrupts 0x28-0x2F
    io_wait();
    
    // ICW3: Sett opp kaskadering
    outb(PIC1_DATA, 0x04); // Fortell master PIC at slave er på IRQ2
    outb(PIC2_DATA, 0x02); // Fortell slave PIC sin kaskade identitet
    io_wait();
    
    // ICW4: Sett 8086 modus
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    io_wait();
    
    // OCW1: Sett interrupt masker - aktiver timer (IRQ0) og tastatur (IRQ1)
    outb(PIC1_DATA, 0xFC); // Aktiver IRQ0 (timer) og IRQ1 (tastatur)
    outb(PIC2_DATA, 0xFF); // Deaktiver alle slave interrupts
}

// Initialiserer interrupt system
void interrupt_initialize(void) {
    pic_initialize();
    
    // Nullstill tastatur buffer og tilstand
    buffer_read_index = buffer_write_index = 0;
    shift_pressed = 0;
    
    // Tøm eventuelle ventende tastatur data
    while (inb(KEYBOARD_STATUS) & 0x01)
        inb(KEYBOARD_DATA);
    
    // Aktiver interrupts
    __asm__ volatile("sti");
    
    display_write_color("Tastatur initialisert og klar\n", COLOR_LIGHT_GREEN);
}

// Sjekker om tastatur buffer har data
int keyboard_data_available(void) {
    return buffer_read_index != buffer_write_index;
}

// Henter et tegn fra tastatur buffer
char keyboard_getchar(void) {
    if (buffer_read_index == buffer_write_index)
        return 0;
    
    char c = keyboard_buffer[buffer_read_index];
    buffer_read_index = (buffer_read_index + 1) & KEYBOARD_BUFFER_MASK;
    
    return c;
}

// Sjekker om interrupts er aktivert
int interrupts_enabled(void) {
    uint32_t flags;
    __asm__ volatile("pushf; pop %0" : "=r"(flags));
    return (flags & 0x200) != 0;
}

// Enkel CPU unntak handler
void isr_handler(uint32_t esp) {
    (void)esp;
}

// Optimalisert IRQ handler
void irq_handler(uint32_t esp) {
    uint8_t irq = *((uint8_t*)(esp + 36)) - 32;
    
    if (irq == 0) {
        timer_handler();
    } 
    else if (irq == 1) {
        uint8_t scancode = inb(KEYBOARD_DATA);
        
        if (scancode == SCANCODE_LEFT_SHIFT || scancode == SCANCODE_RIGHT_SHIFT) {
            shift_pressed = 1;
        } else if (scancode == (SCANCODE_LEFT_SHIFT | 0x80) || scancode == (SCANCODE_RIGHT_SHIFT | 0x80)) {
            shift_pressed = 0;
        }
        else if (!(scancode & 0x80)) {
            char c = scancode_to_ascii(scancode);
            
            if (c) {
                uint8_t next = (buffer_write_index + 1) & KEYBOARD_BUFFER_MASK;
                
                if (next != buffer_read_index) {
                    keyboard_buffer[buffer_write_index] = c;
                    buffer_write_index = next;
                }
            }
        }
    }
    
    // Send End-of-Interrupt signal
    if (irq >= 8)
        outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);
} 