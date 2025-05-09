#include "libc/stdint.h"
void initKeyboard();
void keyboardHandler(struct InterruptRegisters *regs);
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void update_cursor(int x, int y);
char getScanCode();
