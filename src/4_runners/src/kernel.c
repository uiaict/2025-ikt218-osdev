#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "idt.h"
#include "gdt.h"
#include "irq.h"
#include <multiboot2.h>



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main() {
    // Placeholder implementation for kernel_main
    while (1) {
        // Halt the CPU to prevent unnecessary execution
        __asm__("hlt");
    }
    return 0; // This line will never be reached
}

//demonstaret eax regsiter with function
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

void terminal_write(const char* str) {
    static int row = 0;                     // Keep track of the current row
    const int col = 0;                      // Always start from first column
    volatile char* video_memory = (char*)0xb8000;

    size_t len = strlen(str);
    int offset = (row * 80 + col) * 2;      // 80 chars per row, 2 bytes per char

    for (size_t i = 0; i < len; i++) {
        video_memory[offset + i * 2] = str[i];
        video_memory[offset + i * 2 + 1] = 0x07; // Light gray on black
    }

    row++; // Move to next line for future calls
}

void terminal_put_char(char c) {
    static int row = 0;
    static int col = 0;
    volatile char* video_memory = (char*)0xb8000;

    if (c == '\n') {
        row++;
        col = 0;
        return;
    }

    int offset = (row * 80 + col) * 2;
    video_memory[offset] = c;
    video_memory[offset + 1] = 0x07;

    col++;
    if (col >= 80) {
        col = 0;
        row++;
    }
}


int main(uint32_t structAddr, uint32_t magic, struct multiboot_info* mb_info_addr) {
      // Initialize the Global Descriptor Table (GDT)
      gdt_init();

      // Initialize the Interrupt Descriptor Table (IDT)
      idt_init();

      irq_init(); // Initialize IRQs after IDT

  
      // Write "Hello, World!" to the terminal
      terminal_write("Hello, World!");
  
      // Example usage of a struct
      myStruct* myStructPtr = (myStruct*) structAddr;
  
      // Example computation
      int noop = 0;
      int res = compute(1, 2);
  
      // 🔥 Trigger some test interrupts
      asm volatile ("int $0");
      asm volatile ("int $1");
      asm volatile ("int $2");

      asm volatile ("sti");

  
      // Enter the kernel main loop
      return kernel_main();
  
}