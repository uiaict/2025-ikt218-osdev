#ifndef INPUT_H
#define INPUT_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/isr.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool caps_enabled;
extern bool shift_pressed;

char scancode_to_ascii(uint8_t scancode);
void init_keyboard(void);

#ifdef __cplusplus
}
#endif

#endif // INPUT_H