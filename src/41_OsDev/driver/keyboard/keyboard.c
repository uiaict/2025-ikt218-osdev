#include "keyboard.h"
#include "port_io.h"
#include <libc/stdint.h>
#include <libc/stdbool.h>

////////////////////////////////////////
// PS/2 Controller Configuration
////////////////////////////////////////

#define PS2_DATA_PORT   0x60
#define PS2_STATUS_PORT 0x64

////////////////////////////////////////
// Keyboard Buffer (Ring Buffer)
////////////////////////////////////////

#define KB_BUFFER_SIZE 32
static KeyCode kb_buffer[KB_BUFFER_SIZE];
static uint8_t kb_head = 0, kb_tail = 0;

// Check if buffer is empty
static inline bool buf_empty(void) {
    return kb_head == kb_tail;
}

// Check if buffer is full
static inline bool buf_full(void) {
    return ((kb_head + 1) % KB_BUFFER_SIZE) == kb_tail;
}

// Add a key to the buffer if space is available
static void enqueue(KeyCode c) {
    if (!buf_full()) {
        kb_buffer[kb_head] = c;
        kb_head = (kb_head + 1) % KB_BUFFER_SIZE;
    }
}

////////////////////////////////////////
// Scan Code Mappings
////////////////////////////////////////

static const KeyCode scan_map[128] = {
    [0x01] = KEY_ESC,
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
    [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8',
    [0x0A] = '9', [0x0B] = '0', [0x0C] = '+', [0x0D] = '\'',
    [0x0E] = '\b',
    [0x0F] = '\t',
    [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r',
    [0x14] = 't', [0x15] = 'y', [0x16] = 'u', [0x17] = 'i',
    [0x18] = 'o', [0x19] = 'p', [0x1A] = 'a', [0x1B] = '^',
    [0x1C] = KEY_ENTER,
    [0x1E] = 's', [0x1F] = 'd', [0x20] = 'f', [0x21] = 'g',
    [0x22] = 'h', [0x23] = 'j', [0x24] = 'k', [0x25] = 'l',
    [0x26] = 'o', [0x28] = 'z', [0x29] = '`',
    [0x2B] = '\\',
    [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v',
    [0x30] = 'b', [0x31] = 'n', [0x32] = 'm',
    [0x33] = ',', [0x34] = '.', [0x35] = '-',
    [0x37] = '*',
    [0x39] = KEY_SPACE,
};

static const KeyCode ext_map[128] = {
    [0x48] = KEY_UP,
    [0x50] = KEY_DOWN,
    [0x4B] = KEY_LEFT,
    [0x4D] = KEY_RIGHT,
    [0x1C] = KEY_ENTER,
    [0x01] = KEY_ESC
};

////////////////////////////////////////
// Scancode Processing
////////////////////////////////////////

// Process a single scancode from the PS/2 data port
static void process_scancode(void) {
    if (!(inb(PS2_STATUS_PORT) & 0x01)) {
        return;
    }

    uint8_t sc = inb(PS2_DATA_PORT);

    if (sc == 0xE0) {
        while (!(inb(PS2_STATUS_PORT) & 0x01));
        uint8_t sc2 = inb(PS2_DATA_PORT);
        if (!(sc2 & 0x80) && ext_map[sc2]) {
            enqueue(ext_map[sc2]);
        }
    } else {
        if (sc & 0x80) return;
        if (scan_map[sc]) {
            enqueue(scan_map[sc]);
        }
    }
}

////////////////////////////////////////
// Interrupt Handler Entry Point
////////////////////////////////////////

// ISR entry point for keyboard interrupts
void keyboard_handler(void) {
    process_scancode();
}

////////////////////////////////////////
// Public API
////////////////////////////////////////

// Initialize keyboard driver and flush input buffer
void keyboard_initialize(void) {
    kb_head = kb_tail = 0;
    while (inb(PS2_STATUS_PORT) & 0x01) {
        inb(PS2_DATA_PORT);
    }
}

// Blocking read from keyboard buffer
KeyCode keyboard_get_key(void) {
    while (buf_empty()) {
        process_scancode();
    }
    KeyCode c = kb_buffer[kb_tail];
    kb_tail = (kb_tail + 1) % KB_BUFFER_SIZE;
    return c;
}

// Check if the keyboard buffer is empty
bool keyboard_buffer_empty(void) {
    return buf_empty();
}

// Non-blocking read from keyboard buffer
KeyCode keyboard_buffer_dequeue(void) {
    if (buf_empty()) {
        return 0;
    }
    KeyCode c = kb_buffer[kb_tail];
    kb_tail = (kb_tail + 1) % KB_BUFFER_SIZE;
    return c;
}
