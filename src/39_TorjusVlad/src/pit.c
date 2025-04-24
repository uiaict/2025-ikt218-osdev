#include "libc/stdint.h"
#include "arch/i386/idt.h"
#include "libc/stdio.h"
#include "pit.h"
#include "libc/portio.h"



uint32_t ticks;
const uint32_t freq = 100;

void on_irq0(struct interrupt_registers *regs){

    ticks += 1;

    //printf("Timer tick tocks!!");

}

void init_pit(){
    ticks = 0;
    irq_install_handler(0,&on_irq0);

    //oscillator runs at 1.193181666
    uint32_t divisor = 1193180/freq;

    
    outb(0x43,0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40,(uint8_t)((divisor >> 8) & 0xFF));
}

uint32_t get_current_tick() {
    return ticks;
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start = get_current_tick();
    uint32_t ticks_to_wait = milliseconds; // since you're using 1000 Hz later

    while ((get_current_tick() - start) < ticks_to_wait);
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t end = get_current_tick() + milliseconds;

    while (get_current_tick() < end) {
        asm volatile("sti\nhlt");
    }
}