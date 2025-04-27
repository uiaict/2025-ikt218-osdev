// keyboard.c

#include "libc/stdint.h"
#include "libc/terminal.h"
#include "idt.h"
#include "common.h"
#include "input.h"

#define KBD_BUF_SIZE 256

static char kbd_buffer[KBD_BUF_SIZE];
static volatile uint16_t kbd_head = 0;
static volatile uint16_t kbd_tail = 0;

// Scancode → ASCII table (press codes only)
static const char scancode_ascii[128] = {
    /* 0x00–0x07 */ 0, 0, '1', '2', '3', '4', '5', '6',
    /* 0x08–0x0F */ '7', '8', '9', '0', '-', '=', '\b', '\t',
    /* 0x10–0x17 */ 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    /* 0x18–0x1F */ 'o', 'p', '[', ']', '\n', 0, 'a', 's',
    /* 0x20–0x27 */ 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    /* 0x28–0x2F */ '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    /* 0x30–0x37 */ 'b', 'n', 'm', ',', '.', '/', 0, '*',
    /* 0x38–0x3F */ 0, ' ', 0, 0, 0, 0, 0, 0,
    /* fill rest with zeros */
};

// IRQ1 callback, runs on every key press
static void keyboard_callback(registers_t *regs, void *ctx)
{
    uint8_t scancode = inb(0x60);
    if (scancode & 0x80)
        return; // break codes

    char c = scancode_ascii[scancode];
    if (!c)
        return; // non-printables

    uint16_t next = (kbd_head + 1) % KBD_BUF_SIZE;
    if (next != kbd_tail)
    { // buffer not full
        kbd_buffer[kbd_head] = c;
        kbd_head = next;
    }
}

// Call this once after your PIC is remapped and STI is done
void init_keyboard()
{
    register_irq_handler(IRQ1, keyboard_callback, NULL);
}

// Blocking getChar: waits until a keystroke is available
char getChar(void)
{
    // Wait for data
    while (kbd_head == kbd_tail)
    {
        asm volatile("hlt"); // halt until next interrupt
    }
    char c = kbd_buffer[kbd_tail];
    kbd_tail = (kbd_tail + 1) % KBD_BUF_SIZE;
    return c;
}
