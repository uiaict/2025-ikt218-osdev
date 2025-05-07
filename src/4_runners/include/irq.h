#pragma once
#include "libc/stdint.h"
#include "libc/stdbool.h"

// Keyboard scan codes (used by both IRQ and Snake)
#define SCANCODE_UP     0x48 
#define SCANCODE_DOWN   0x50
#define SCANCODE_LEFT   0x4B
#define SCANCODE_RIGHT  0x4D
#define SCANCODE_ESC    0x01
#define SCANCODE_P      0x19
#define SCANCODE_R      0x13
#define SCANCODE_S      0x1F  // Added S key for starting the game

// Function declarations
void irq_init(void);
void irq_handler(int irq);
uint8_t keyboard_getchar(void);  // Changed to uint8_t to match implementation
void pit_handler(void);
void initkeyboard(void);
void set_game_mode(bool enabled);
bool get_game_mode(void);

// IRQ stubs
void irq0_stub(void);
void irq1_stub(void);
void irq2_stub(void);
void irq3_stub(void);
void irq4_stub(void);
void irq5_stub(void);
void irq6_stub(void);
void irq7_stub(void);
void irq8_stub(void);
void irq9_stub(void);
void irq10_stub(void);
void irq11_stub(void);
void irq12_stub(void);
void irq13_stub(void);
void irq14_stub(void);
void irq15_stub(void);