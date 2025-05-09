#include "matrix/matrix.h"
#include "kernel/keyboard.h"
#include "kernel/pit.h"
#include "kernel/system.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include "shell/shell.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define TAIL_LENGTH 5

// VGA text buffer at 0xB8000
static volatile uint16_t *const vga = (volatile uint16_t *)0xB8000;

// 32-bit LFSR PRNG
static uint32_t lfsr = 1;
static inline uint8_t prng(void) {
  lfsr ^= lfsr << 13;
  lfsr ^= lfsr >> 17;
  lfsr ^= lfsr << 5;
  return (uint8_t)(lfsr & 0xFF);
}
// map PRNG → printable glyph
static inline char random_char(void) { return (char)((prng() & 0x3F) + 0x30); }

// one drop head position per column; negative = not-yet-started
static int drops[VGA_WIDTH];

// draw one frame of “rain” with fixed-length tails
static void matrix_draw_frame(void) {
  for (int x = 0; x < VGA_WIDTH; x++) {
    int y = drops[x]++;

    // draw bright head
    if (y >= 0 && y < VGA_HEIGHT) {
      vga[y * VGA_WIDTH + x] = random_char() | (0x0F << 8);
    }
    // clear the tail cell
    int tail = y - TAIL_LENGTH;
    if (tail >= 0 && tail < VGA_HEIGHT) {
      vga[tail * VGA_WIDTH + x] = ' ' | (0x0A << 8);
    }
    // reset when off‐screen
    if (y > VGA_HEIGHT + TAIL_LENGTH) {
      drops[x] = -(prng() % VGA_HEIGHT);
    }
  }
}

void matrix_start_command(void) {
  bool quit = false;

  // clear screen
  for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    vga[i] = ' ' | (0x0A << 8);
  }
  // start positions
  for (int x = 0; x < VGA_WIDTH; x++) {
    drops[x] = -(prng() % VGA_HEIGHT);
  }

  while (!quit) {
    matrix_draw_frame();
    sleep_interrupt(100);

    uint8_t sc = inb(0x60);
    if (!(sc & 0x80) && sc < SCANCODE_MAX) {
      if (scancode_to_ascii[sc] == 'q') {
	quit = true;
      }
    }
  }

  shell_init();
}
