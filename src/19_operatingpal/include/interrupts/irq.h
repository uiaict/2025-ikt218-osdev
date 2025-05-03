#pragma once

#include "libc/stdint.h"

void irq_install_handler(int irq, void (*handler)());
void irq_uninstall_handler(int irq);
void irq_handler(uint32_t irq_num);
void irq_init();
