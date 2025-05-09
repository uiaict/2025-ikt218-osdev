#include "libc/stdint.h"
#include "../util/util.h"
#include "../idt/idt.h"
#include "../vga/vga.h"
#include "PIT.h"

volatile uint32_t system_ticks = 0;
uint16_t counter = 1000;

void onIrq0(struct InterruptRegisters *regs) {
    system_ticks++;
    outPortB(PIC1_CMD_PORT, PIC_EOI);
}

void initPit() {
    irq_install_handler(0,&onIrq0);
    system_ticks = 0;
    outPortB(PIT_CMD_PORT,0x36);
    outPortB(PIT_CHANNEL0_PORT,DIVIDER);
    outPortB(PIT_CHANNEL0_PORT,(DIVIDER >> 8));
}

void setTimer(float milliseconds) {
    counter = milliseconds;
}

void sleepBusy(uint32_t milliseconds) {
    uint32_t start = system_ticks;
    while(system_ticks - start < milliseconds) {}
}

void sleepInterrupt(uint32_t milliseconds) {
    uint32_t end_ticks = system_ticks + milliseconds;
    while(system_ticks < end_ticks) {
        asm volatile("sti; hlt");
    }
    
}

uint32_t getSystemTicks() {
    return system_ticks;
}
