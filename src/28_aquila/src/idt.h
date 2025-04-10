
#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"

void init_idt();

extern void init_pit(uint32_t frequency);

#endif
