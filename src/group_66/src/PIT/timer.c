#include "libc/stdint.h"
#include "../util/util.h"
#include "../idt/idt.h"
#include "../vga/vga.h"
#include "timer.h"

float ticker;
uint32_t divisor = 1;
uint16_t counter = 1;

void onIrq0(struct InterruptRegisters *regs) {
    ticker += 0.0054;
    if(ticker >= counter) {
        printf("HELLO");
        ticker = 0;
    }
}

void initTimer() {
    ticker = 0;

    uint32_t count = (FREQ) / divisor;

    irq_install_handler(0,&onIrq0);

    outPortB(0x43,0x36);
    outPortB(0x40,count & 0xFF);
    outPortB(0x40,(count >> 8) & 0xFF);
}

void setTimerFreq(float newCounter) {
    counter = newCounter;
}