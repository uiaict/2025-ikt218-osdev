#include "system.h"
#include "libc/scrn.h"

void shutdown() {
    printf("\nSlår av systemet...\n");
    outw(0x604, 0x2000);
    while (1) {
        __asm__ volatile ("hlt");
    }
}
