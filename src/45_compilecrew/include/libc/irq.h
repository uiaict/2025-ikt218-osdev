#pragma once
#include "libc/stdint.h"

void pic_remap(void);
void irq_acknowledge(uint8_t irq);
