#include "keyboard.h"
#include "isr.h"
#include "monitor.h"

static char keyboard_map[128] =
{
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static char keyboard_map_shift[128] =
{
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static u8int shift_pressed = 0;
static u8int ctrl_pressed = 0;
static u8int alt_pressed = 0;
static u8int caps_lock = 0;

u8int keyboard_is_shift_pressed()
{
    return shift_pressed;
}

char keyboard_scancode_to_char(u8int scancode, u8int shift_state)
{
    if (scancode < 128) {
        if (shift_state) {
            return keyboard_map_shift[scancode];
        } else {
            return keyboard_map[scancode];
        }
    }
    return 0;
}

static void keyboard_callback(registers_t regs)
{
    u8int scancode = inb(0x60);
    
    if (scancode & 0x80) {
        scancode &= 0x7F;
        
        switch (scancode) {
            case KEY_LSHIFT:
            case KEY_RSHIFT:
                shift_pressed = 0;
                break;
            case KEY_CTRL:
                ctrl_pressed = 0;
                break;
            case KEY_ALT:
                alt_pressed = 0;
                break;
        }
    }
    else {
        switch (scancode) {
            case KEY_LSHIFT:
            case KEY_RSHIFT:
                shift_pressed = 1;
                return;
            case KEY_CTRL:
                ctrl_pressed = 1;
                return;
            case KEY_ALT:
                alt_pressed = 1;
                return;
            case KEY_CAPSLOCK:
                caps_lock = !caps_lock;
                return;
        }
        
        switch (scancode) {
            case KEY_ESCAPE:
                monitor_write("[ESC]");
                break;
            case KEY_BACKSPACE:
                monitor_put('\b');
                break;
            case KEY_TAB:
                monitor_put('\t');
                break;
            case KEY_ENTER:
                monitor_put('\n');
                break;
            case KEY_F1: case KEY_F2: case KEY_F3: case KEY_F4:
            case KEY_F5: case KEY_F6: case KEY_F7: case KEY_F8:
            case KEY_F9: case KEY_F10: case KEY_F11: case KEY_F12:
                monitor_write("[F");
                monitor_write_dec(scancode - KEY_F1 + 1);
                monitor_write("]");
                break;
            default:
                u8int use_shift = shift_pressed;
                
                if (caps_lock) {
                    if ((scancode >= 16 && scancode <= 25) ||
                        (scancode >= 30 && scancode <= 38) ||
                        (scancode >= 44 && scancode <= 50)) {
                        use_shift = !use_shift;
                    }
                }
                
                char c = keyboard_scancode_to_char(scancode, use_shift);
                if (c != 0) {
                    if (ctrl_pressed && c >= 'a' && c <= 'z') {
                        c = c - 'a' + 1;
                        monitor_write("[CTRL+");
                        monitor_put((c - 1) + 'A');
                        monitor_write("]");
                    } else {
                        monitor_put(c);
                    }
                }
                break;
        }
    }
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, &keyboard_callback);
    monitor_write("Keyboard initialized\n");
}