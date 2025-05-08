#include "timer.h"
#include "libc/stdio.h"

volatile uint32_t tick = 0;
uint32_t ticks_per_ms = 0;

uint32_t get_global_tick(){
    return tick;
}

void init_pit(uint32_t frequency){
    // Lowest frequency = 18.207(19)Hz
    // don't send in value as variable??
    if (frequency == 0){
        return;
    }
    

    ticks_per_ms = frequency/1000;
    if (ticks_per_ms == 0){
        // Makes all sleeps wait at least 1 tick
        ticks_per_ms++;
    }
    

    register_interrupt_handler(IRQ0, pit_handler);
    
    
    uint32_t divisor = PIT_REFRESHRATE / frequency;
    // Divisor max value 65535 or 0xFFFF

    outb(PIT_COMMAND, 0x36); // 0x36 = 00 11 011 0
                             // channel0, l/h, mode3, 16bit binary

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t lo = (uint8_t)(divisor & 0xFF);
    uint8_t hi = (uint8_t)((divisor>>8) & 0xFF);
 
    // Send the frequency divisor.
    outb(PIT_DATACHANNEL_0, lo);
    outb(PIT_DATACHANNEL_0, hi);
}


void pit_handler(struct registers reg){
    tick++;
}

void busy_sleep(uint32_t ms){

    uint32_t start_tick = get_global_tick();
    uint32_t ticks_to_wait = ms * ticks_per_ms;
    uint32_t elapsed_ticks = 0;
    
        while (elapsed_ticks < ticks_to_wait){
            
            while (get_global_tick() == (start_tick + elapsed_ticks)){      
                // Checks even when tick is unchanged
            }
            elapsed_ticks++;
        }    
}

void interrupt_sleep(uint32_t ms){
    uint32_t start_tick = get_global_tick();
    uint32_t current_tick = start_tick;
    uint32_t ticks_to_wait = ms * ticks_per_ms;
    uint32_t end_tick = current_tick + ticks_to_wait;
    
    while ((current_tick - start_tick) < ticks_to_wait){ // Prevents overflow issues
        asm volatile("sti"); // enable interrupts
        asm volatile("hlt"); // halt CPU until interrupt
        current_tick = get_global_tick(); // interrupt happens, tick is changed
    }   
}