#define FREQ 1193182 // Mhz

void initTimer();
void onIrq0(struct InterruptRegisters *regs);
void setTimerFreq(float newCounter);

