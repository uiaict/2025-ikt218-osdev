#include "isr.h"
#include "libc/string.h"

void terminal_write(const char* str); // Forward declaration

void isr_handler(int interrupt_number) {
    const char* messages[] = {
        "Received interrupt: 0",
        "Received interrupt: 1",
        "Received interrupt: 2"
    };

    if (interrupt_number >= 0 && interrupt_number < 3) {
        terminal_write(messages[interrupt_number]);
    } else {
        terminal_write("Unknown interrupt");
    }
}
