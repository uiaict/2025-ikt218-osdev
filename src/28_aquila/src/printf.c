

#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include <multiboot2.h>

#define VGA_ADDRESS 0xB8000
volatile char *vga = (volatile char *)VGA_ADDRESS;
int cursor = 0;

void printf(const char *message) {
  for (int i = 0; message[i] != '\0'; i++) {
    vga[(cursor) * 2] = message[i];
    vga[(cursor) * 2 + 1] = 0x07;

    cursor++;
    if (cursor % 80 == 0 && cursor < 80 * 25) {
      // La den gå til neste linje automatisk
    } else if (cursor >= 80 * 25) {
      cursor = 0; // wrap rundt
    }
  }
}
