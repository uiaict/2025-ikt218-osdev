
#include "printf.h"
#include <stdint.h>
#include <stddef.h>
#include "libc/string.h"
#include <string.h>

void shell_prompt() {
    printf("UiAOS> ");
}

void shell_handle_input(const char* input) {
    if (strcmp(input, "help") == 0) {
        printf("Tilgjengelige kommandoer:\n");
        printf(" - help\n - clear\n - echo [tekst]\n");
    } else if (strncmp(input, "echo ", 5) == 0) {
        printf("%s\n", input + 5);
    } else if (strcmp(input, "clear") == 0) {
        for (int i = 0; i < 80 * 25; i++)
            ((uint16_t*)0xB8000)[i] = 0x0720;
    } else {
        printf("Ukjent kommando: %s\n", input);
    }
}