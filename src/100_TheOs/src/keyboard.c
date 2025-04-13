#include "interrupts.h"
#include "libc/system.h"
#include "libc/system.h"

extern void terminal_printf(const char* format, ...);

// Keyboard buffer
#define KEYBOARD_BUFFER_SIZE 256
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_pos = 0;


typedef enum {
    KEY_ESC = 1,
    KEY_1 = 2,
    KEY_2 = 3,
    KEY_3 = 4,
    KEY_4 = 5,
    KEY_5 = 6,
    KEY_6 = 7,
    KEY_7 = 8,
    KEY_8 = 9,
    KEY_9 = 10,
    KEY_0 = 11,
    KEY_DASH = 12,
    KEY_EQUALS = 13,
    KEY_TAB = 15,
    KEY_Q = 16,
    KEY_W = 17,
    KEY_E = 18,
    KEY_R = 19,
    KEY_T = 20,
    KEY_Y = 21,
    KEY_U = 22,
    KEY_I = 23,
    KEY_O = 24,
    KEY_P = 25,
    KEY_LBRACKET = 26,
    KEY_RBRACKET = 27,
    KEY_ENTER = 28,
    KEY_CTRL = 29,
    KEY_A = 30,
    KEY_S = 31,
    KEY_D = 32,
    KEY_F = 33,
    KEY_G = 34,
    KEY_H = 35,
    KEY_J = 36,
    KEY_K = 37,
    KEY_L = 38,
    KEY_SIMICOLON = 39,
    KEY_THEN = 40,
    KEY_GRAVE = 41,
    KEY_LSHIFT = 42,
    KEY_BSLASH = 43,
    KEY_Z = 44,
    KEY_X = 45,
    KEY_C = 46,
    KEY_V = 47,
    KEY_B = 48,
    KEY_N = 49,
    KEY_M = 50,
    KEY_COMMA = 51,
    KEY_PERIOD = 52,
    KEY_FSLASH = 53,
    KEY_RSHIFT = 54,
    KEY_PRTSC = 55,
    KEY_ALT = 56,
    KEY_SPACE = 57,
    KEY_CAPS = 58,
    KEY_F1 = 59,
    KEY_F2 = 60,
    KEY_F3 = 61,
    KEY_F4 = 62,
    KEY_F5 = 63,
    KEY_F6 = 64,
    KEY_F7 = 65,
    KEY_F8 = 66,
    KEY_F9 = 67,
    KEY_F10 = 68,
    KEY_NUM = 69,
    KEY_SCROLL = 70,
    KEY_HOME = 71,
    KEY_UP = 72,
    KEY_PGUP = 73,
    KEY_MINUS = 74,
    KEY_LEFT = 75,
    KEY_CENTER = 76,
    KEY_RIGHT = 77,
    KEY_PLUS = 78,
    KEY_END = 79,
    KEY_DOWN = 80,
    KEY_PGDN = 81,
    KEY_INS = 82,
    KEY_DEL = 83,
} scan_code;

#define CHAR_NONE 0
#define CHAR_ENTER 2
#define CHAR_SPACE 3

static bool capsEnabled = false;
static bool shiftEnabled = false;
char scancode_to_ascii(unsigned char* scan_code) {
    unsigned char a = *scan_code;
    switch (a){
       case KEY_RSHIFT:
       case KEY_LSHIFT:
        // Using shift toggle
            shiftEnabled = !shiftEnabled;
            return CHAR_NONE;
            
       case KEY_CAPS:
       // Caps toggle
            capsEnabled = !capsEnabled;
            return CHAR_NONE;

       case KEY_ENTER:
            return CHAR_ENTER;

       case KEY_SPACE:
            return CHAR_SPACE;

       case KEY_END:
       case KEY_DOWN:
       case KEY_PGDN:
       case KEY_INS:
       case KEY_DEL:
       case KEY_LEFT:
       case KEY_CENTER:
       case KEY_RIGHT:
       case KEY_F1:
       case KEY_F2:
       case KEY_F3:
       case KEY_F4:
       case KEY_F5:
       case KEY_F6:
       case KEY_F7:
       case KEY_F8:
       case KEY_F9:
       case KEY_F10:
       case KEY_NUM:
       case KEY_SCROLL:
       case KEY_HOME:
       case KEY_UP:
       case KEY_PGUP:
       case KEY_ESC:
       case KEY_TAB:
       case KEY_CTRL:
       case KEY_PRTSC:
       case KEY_ALT:
            return CHAR_NONE;
       case KEY_1:
            return shiftEnabled ? '!' : '1';
       case KEY_2:
            return shiftEnabled ? '"' : '2';
       case KEY_3:
            return shiftEnabled ? '#' : '3';
       case KEY_4:
            return shiftEnabled ? '¤' : '4';
       case KEY_5:
            return shiftEnabled ? '%' : '5';
       case KEY_6:
            return shiftEnabled ? '&' : '6';
       case KEY_7:
            return shiftEnabled ? '/' : '7';
       case KEY_8:
            return shiftEnabled ? '(' : '8';
       case KEY_9:
            return shiftEnabled ? ')' : '9';
       case KEY_0:
            return shiftEnabled ? '=' : '0';
       case KEY_DASH:
            return shiftEnabled ? '_' : '-';
       case KEY_EQUALS:
            return '=';
       case KEY_Q:
            return capsEnabled || shiftEnabled ? 'Q' : 'q';
       case KEY_W:
            return capsEnabled || shiftEnabled ? 'W' : 'w';
       case KEY_E:
            return capsEnabled || shiftEnabled ? 'E' : 'e';
       case KEY_R:
            return capsEnabled || shiftEnabled ? 'R' : 'r';
       case KEY_T:
            return capsEnabled || shiftEnabled ? 'T' : 't';
       case KEY_Y:
            return capsEnabled || shiftEnabled ? 'Y' : 'y';
       case KEY_U:
            return capsEnabled || shiftEnabled ? 'U' : 'u';
       case KEY_I:
            return capsEnabled || shiftEnabled ? 'I' : 'i';
       case KEY_O:
            return capsEnabled || shiftEnabled ? 'O' : 'o';
       case KEY_P:
            return capsEnabled || shiftEnabled ? 'P' : 'p';
       case KEY_A:
            return capsEnabled || shiftEnabled ? 'A' : 'a';
       case KEY_S:
            return capsEnabled || shiftEnabled ? 'S' : 's';
       case KEY_D:
            return capsEnabled || shiftEnabled ? 'D' : 'd';
       case KEY_F:
            return capsEnabled || shiftEnabled ? 'F' : 'f';
       case KEY_G:
            return capsEnabled || shiftEnabled ? 'G' : 'g';
       case KEY_H:
            return capsEnabled || shiftEnabled ? 'H' : 'h';
       case KEY_J:
            return capsEnabled || shiftEnabled ? 'J' : 'j';
       case KEY_K:
            return capsEnabled || shiftEnabled ? 'K' : 'k';
       case KEY_L:
            return capsEnabled || shiftEnabled ? 'L' : 'l';
       case KEY_THEN:
            return shiftEnabled ? '>' : '<';
       case KEY_BSLASH:
            return shiftEnabled ? '\\' : '`';
       case KEY_Z:
            return capsEnabled || shiftEnabled ? 'Z' : 'z';
       case KEY_X:
            return capsEnabled || shiftEnabled ? 'X' : 'x';
       case KEY_C:
            return capsEnabled || shiftEnabled ? 'C' : 'c';
       case KEY_V:
            return capsEnabled || shiftEnabled ? 'V' : 'v';
       case KEY_B:
            return capsEnabled || shiftEnabled ? 'B' : 'b';
       case KEY_N:
            return capsEnabled || shiftEnabled ? 'N' : 'n';
       case KEY_M:
            return capsEnabled || shiftEnabled ? 'M' : 'm';
       case KEY_COMMA:
            return shiftEnabled ? ';' : ',';
       case KEY_PERIOD:
            return shiftEnabled ? ':' : '.';
       case KEY_FSLASH:
            return '/';
       case KEY_MINUS:
            return '-';
       case KEY_PLUS:
            return '+';
       default:
            return CHAR_NONE;
    }
}


// Key states
static bool shift_pressed = false;
static bool caps_pressed = false;

// Keyboard IRQ controller (IRQ1)
void keyboard_controller(registers_t* regs, void* context) {
    unsigned char scancode = inb(0x60); // Read scancode from port 0x60
    
    
    char ascii = scancode_to_ascii(&scancode);
    
    switch (ascii) {
        case CHAR_NONE:
            return;
        case CHAR_ENTER:
            terminal_printf("\n");
            break;
        case CHAR_SPACE:
            terminal_printf(" ");
            break;
        default:
            terminal_printf("%c", ascii);
    }
}

// Initialize keyboard controller
void start_keyboard() {
    // Initialize key states
    shift_pressed = false;
    caps_pressed = false;
    
    register_irq_controller(1, keyboard_controller, NULL);
    
    terminal_printf("Keyboard initialized. Start typing...\n");
}