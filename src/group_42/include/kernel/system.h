#ifndef SYSTEM_H
#define SYSTEM_H

#include "libc/stdint.h"

/**
 * @brief OUTx
 *
 * Sends a 8/16/32-bit value on a I/O location. Traditional names are
 * outb, outw and outl respectively. The a modifier enforces val to be placed in
 * the eax register before the asm command is issued and Nd allows for one-byte
 * constant values to be assembled as constants, freeing the edx register for
 * other cases.
 *
 * Taken from https://wiki.osdev.org/Inline_Assembly/Examples
 */
static inline void outb(uint16_t port, uint8_t val) {
  __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
  /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers
   * that fit in 8b. (N constraint). Wider immediate constants would be
   * truncated at assemble-time (e.g. "i" constraint). The  outb  %al, %dx
   * encoding is the only option for all other cases. %1 expands to %dx because
   * port  is a uint16_t.  %w1 could be used if we had the port number a wider C
   * type */
}

/**
 * @brief INx
 *
 * Receives a 8/16/32-bit value from an I/O location. Traditional names are inb,
 * inw and inl respectively.
 *
 * Taken from https://wiki.osdev.org/Inline_Assembly/Examples
 */
static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

/**
 * @brief IO_WAIT
 *
 * Wait a very small amount of time (1 to 4 microseconds, generally). Useful for
 * implementing a small delay for PIC remapping on old hardware or generally as
 * a simple but imprecise wait.
 *
 * You can do an IO operation on any unused port: the Linux kernel by default
 * uses port 0x80, which is often used during POST to log information on the
 * motherboard's hex display but almost always unused after boot.
 *
 * Taken from https://wiki.osdev.org/Inline_Assembly/Examples
 */
static inline void io_wait(void) { outb(0x80, 0); }

/**
 * @brief Enable the BIOS cursor.
 *
 * Taken from https://wiki.osdev.org/Text_Mode_Cursor
 */
void cursor_enable(uint8_t cursor_start, uint8_t cursor_end);
/**
 * @brief Disable the BIOS cursor.
 *
 * Taken from https://wiki.osdev.org/Text_Mode_Cursor
 */
void cursor_disable();

/**
 * @brief Moving the Cursor
 *
 * Keep in mind that you don't need to update the cursor's location every time a
 * new character is displayed. It would be faster to instead only update it
 * after printing an entire string.
 *
 * Taken from https://wiki.osdev.org/Text_Mode_Cursor
 */
void update_cursor(int x, int y);

/**
 * @brief Get Cursor Position
 *
 * With this code, you get: pos = y * VGA_WIDTH + x. To obtain the coordinates,
 * just calculate: y = pos / VGA_WIDTH; x = pos % VGA_WIDTH;.
 *
 * Taken from https://wiki.osdev.org/Text_Mode_Cursor
 */
uint16_t get_cursor_position(void);

void switch_to_protected_mode(void);

#endif