#include "libc/stdint.h"
#include "../util/util.h"
#include "../idt/idt.h"
#include "../vga/vga.h"
#include "timer.h"

float ticker;
uint16_t count = 1193;
uint16_t counter = 1000;

void onIrq0(struct InterruptRegisters *regs) {
    ticker += 1;
    if(ticker >= counter) {
        printf("<Tick>");
        ticker = 0;
    }
}

void initTimer() {
    ticker = 0;

    irq_install_handler(0,&onIrq0);

    outPortB(0x43,0x36);
    outPortB(0x40,count & 0xFF);
    outPortB(0x40,(count >> 8) & 0xFF);
}

void setTimerFreq(float milliseconds) {
    counter = milliseconds;
}