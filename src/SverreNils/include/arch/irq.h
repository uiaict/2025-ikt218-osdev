#pragma once
#include <stdint.h>
#include "arch/isr.h"

void irq_install();
void irq_handler(struct registers* regs);
