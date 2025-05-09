#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/monitor.h"
#include "libc/stdbool.h"
#include "libc/common.h"
#include "../gdt/isr.h"
#include "../piano/piano.h"
#include "../song/SongPlayer.h"
#include "../ui/shell.h"

void init_keyboard();
void keyboard_handler(registers_t regs);
void read_line(char* buffer);
#endif