#include "libc/keyboard.h"
#include "libc/terminal.h"
#include "libc/printf.h"
#include "libc/stdint.h"

typedef enum {
    MODE_FRONT_PAGE,
    MODE_MATRIX,
    MODE_MUSIC,
    MODE_MEMORY,
    MODE_TERMINAL
} InputMode;

static InputMode current_mode = MODE_FRONT_PAGE;

// Simplified US QWERTY scancode to ASCII table
static const char scancode_table[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) return; // Ignore key releases

    char key = scancode_table[scancode];
    if (!key) return;

    if (current_mode == MODE_FRONT_PAGE) {
        switch (key) {
            case '1':
                current_mode = MODE_MATRIX;
                terminal_clear();
                //draw_matrix();
                return;
            case '2':
                current_mode = MODE_MUSIC;
                terminal_clear();
                draw_music_selection();
                return;
            case '3':
                current_mode = MODE_MEMORY;
                terminal_clear();
                print_memory_layout();
                printf("\nPress escape to return to main menu");
                return;
            case '4':
                current_mode = MODE_TERMINAL;
                terminal_clear();
            case 'q':
            case 'Q':
                //shutdown();
                return;
        }
    }
    

    // If you're in TERMINAL mode, allow normal typing
    if (current_mode == MODE_TERMINAL) {
        enable_cursor(14, 15);
        printf("%c", key);
    }

    // Optional: ESC key returns to front page
    if (key == 27) {
        current_mode = MODE_FRONT_PAGE;
        disable_cursor();
        terminal_clear();
        draw_front_page();
    }
}
