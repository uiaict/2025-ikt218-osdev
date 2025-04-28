#include "libc/terminal.h"

void panic(const char* msg) {
    terminal_write("KERNEL PANIC: ");
    terminal_write(msg);
    while (1) {
        __asm__ volatile ("cli; hlt");
    }
}
