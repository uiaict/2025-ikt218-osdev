
uint32_t get_current_tick();

void init_pit();
void on_irq0(struct interrupt_registers *regs);
void sleep_interrupt(uint32_t milliseconds);
void sleep_busy(uint32_t milliseconds);