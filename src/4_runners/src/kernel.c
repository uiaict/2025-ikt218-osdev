#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "terminal.h"
#include "idt.h"
#include "gdt.h"
#include "irq.h"
#include <multiboot2.h>


void terminal_write(const char* str);
void terminal_put_char(char c);
void initkeyboard(void);
extern char keyboard_getchar(void);

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

#include "terminal.h"

int kernel_main() {
    // Display initial messages
    terminal_write("Hello, World!\n");

    // Trigger interrupts 0, 1, and 2
    asm volatile ("int $0");
    asm volatile ("int $1");
    asm volatile ("int $2");

    // Initialize the keyboard
    terminal_write("Initializing keyboard...\n");
    initkeyboard();
    terminal_write("Keyboard initialized.\n");

    // Move the cursor to the next line explicitly
    int current_row, current_col;
    terminal_get_cursor(&current_row, &current_col); // Get the current cursor position
    terminal_set_cursor(current_row + 1, 0);        // Move to the next line

    asm volatile ("sti"); // Enable interrupts

    while (1) {
        asm volatile ("hlt");
    }

    return 0;
}

int compute(int a, int b) {
    return a + b;
}

typedef struct {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e[6];
} myStruct;


int main(uint32_t structAddr, uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize core components
    gdt_init();
    idt_init();
    irq_init();

    // Write initial message
    terminal_write("System initialized\n");

    // Example struct usage
    myStruct* myStructPtr = (myStruct*)structAddr;

    // Example computation
    int res = compute(1, 2);

    // Enable interrupts and enter kernel main loop
    return kernel_main();
}