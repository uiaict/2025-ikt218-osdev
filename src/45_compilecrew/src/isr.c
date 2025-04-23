#include "libc/isr.h"

void isr_handler(registers_t* regs) {
    exception_handler(regs);
}
