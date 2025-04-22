#include "timer.h"
#include "libc/stdio.h"

volatile uint32_t tick = 0;
uint32_t int_frequency;

uint32_t get_global_tick(){
    return tick;
}

void init_pit(uint32_t freq){
    // Lowest frequency = 18.207(19)Hz
    
    register_interrupt_handler(IRQ0, pit_handler);
    
    int_frequency = freq;
    
    uint32_t divisor = PIT_REFRESHRATE / int_frequency;
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
    uint32_t ticks_to_wait = (uint32_t)((ms/1000) * int_frequency); // Convert ms to s
    uint32_t elapsed_ticks = 0;
    
        while (elapsed_ticks < ticks_to_wait){
            
            // elapsed_tick will only increment when tick increments
            // 20 == (10 + 10); elapsed_tick++;
            // 20 == (10 + 11);     (tick++;)
            // 21 == (10 + 11); elapsed_tick++;
            while (get_global_tick() == (start_tick + elapsed_ticks)){      
                // Checks even when tick is unchanged
            }
            elapsed_ticks++;
        }    
            // 255 == (254 + 1); elapsed_tick++;    255 == 255
            // 255 == (254 + 2);     (tick++;)      255 == 256
            // 256 == (254 + 2); elapsed_tick++     256 == 256
            // 256 == (254 + 3);     (tick++;)      256 == 1
            //   1 == (254 + 3); elapsed_tick++       1 == 1
}

void interrupt_sleep(uint32_t ms){
    uint32_t start_tick = get_global_tick();
    uint32_t current_tick = start_tick;
    uint32_t ticks_to_wait = (uint32_t)(ms/1000) * int_frequency; // Convert ms to s
    uint32_t end_tick = current_tick + ticks_to_wait;
    
    while ((current_tick - start_tick) < ticks_to_wait){ // Prevents overflow issues
        asm volatile("sti"); // enable interrupts
        asm volatile("hlt"); // halt CPU until interrupt
        current_tick = get_global_tick(); // interrupt happens, tick is changed
    }   
}