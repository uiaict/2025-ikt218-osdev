#pragma once

#include <stdint.h>

void pic_remap();
void irq_acknowledge(int irq);
void pic_send_eoi(uint8_t irq);
