#include "libc/system.h"
#include "libc/stdio.h"

[[noreturn]] void panic(const char* reason) {
    printf("KERNEL PANIC: %s\n", reason);
    asm volatile("cli; hlt");
    for(;;);
}