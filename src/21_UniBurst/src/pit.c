#include "pit.h"
#include "isr.h"
#include "io.h"


volatile uint32_t tick = 0;                      

// The PIT handler
void pitHandler(registers_t regs) {
    tick++;                                      
}

// Returns the current tick
uint32_t getCurrentTick() {
    return tick;
}

// Initializes the PIT
void initPit() {

    outb(PIT_CMD_PORT, 0x36);                                      
    outb(PIT_CHANNEL0_PORT, (uint8_t)(DIVIDER & 0xFF));            
    outb(PIT_CHANNEL0_PORT, (uint8_t)((DIVIDER >> 8) & 0xFF));     

    registerInterruptHandler(IRQ0, &pitHandler); 
}

// These sleep functions are based on the pseudo code provided by Per-Arne Andersen at https://perara.notion.site/Assignment-4-Memory-and-PIT-83eabc342fd24b88b00733a78b5a86e0 

// Sleeps using busy-waiting
void sleepBusy(uint32_t ms) {
    uint32_t startTick = getCurrentTick();     
    uint32_t ticksToWait = ms * TICKS_PER_MS;   
    uint32_t endTick = startTick + ticksToWait; 

    if (endTick < startTick) {                  
        
        while (getCurrentTick() >= startTick) { 
            // Do nothing 
        }
        while (getCurrentTick() < endTick) {    
            // Do nothing 
        }
    } else {                                    
        while (getCurrentTick() < endTick) {
            // Do nothing 
        }
    }
}

// Sleeps using interrupts
void sleepInterrupt(uint32_t ms) {
    uint32_t startTick = getCurrentTick();      
    uint32_t ticksToWait = ms * TICKS_PER_MS;   
    uint32_t endTick = startTick + ticksToWait; 

    if (endTick < startTick) {                         
        
        while (getCurrentTick() >= startTick) {        
            asm volatile (
                "sti\n\t"                              
                "hlt\n\t"                              
            );
        }
        while (getCurrentTick() < endTick) {           
            asm volatile (
                "sti\n\t"                            
                "hlt\n\t"                             
            );
        }
    } else {                                          
        while (getCurrentTick() < endTick) {
            asm volatile (
                "sti\n\t"                              
                "hlt\n\t"                              
            );
        }
    }
}

