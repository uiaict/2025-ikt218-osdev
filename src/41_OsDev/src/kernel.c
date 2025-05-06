#include "../libc/stdint.h"
#include "../libc/stddef.h"
#include "../libc/stdbool.h"
#include <multiboot2.h>
#include "../terminal.h"
#include "../gdt.h"

void main(void) {
    gdt_install();
    terminal_initialize();
    terminal_write("Hello World\n");
}
