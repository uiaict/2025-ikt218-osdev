#include <ui/ui_common.h>

////////////////////////////////////////
// UI Utility Functions
////////////////////////////////////////

// Clear the terminal screen
void clear_screen(void) {
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            terminal_putentryat(' ', 0x07, x, y);
        }
    }
    terminal_setcursor(0, 0);
}

// Get the length of a string
size_t terminal_strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}
