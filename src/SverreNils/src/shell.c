#include "printf.h"
#include <stdint.h>
#include <stddef.h>
#include "libc/string.h"
#include <string.h>

void shell_prompt() {
    printf("UiAOS> ");
}

// Skriver til I/O-port (brukes for QEMU shutdown)
static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

// Kaller QEMU ACPI shutdown
void shutdown() {
    outw(0x604, 0x2000);
}

void shell_handle_input(const char* input) {
    if (strcmp(input, "help") == 0) {
        printf("Tilgjengelige kommandoer:\n");
        printf(" - help\n - clear\n - echo [tekst]\n - shutdown\n");
    } else if (strncmp(input, "echo ", 5) == 0) {
        printf("%s\n", input + 5);
    } else if (strcmp(input, "clear") == 0) {
        for (int i = 0; i < 80 * 25; i++)
            ((uint16_t*)0xB8000)[i] = 0x0720;
    } else if (strcmp(input, "shutdown") == 0) {
        printf("Shutting down...\n");
        shutdown();
    } else {
        printf("Ukjent kommando: %s\n", input);
    }
}
