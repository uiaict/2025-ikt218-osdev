#pragma once

void isr_handler(int int_no, int err_code);

// You can declare ISR stubs here if you want to reference them elsewhere
extern void isr0();
extern void isr1();
extern void isr2();
