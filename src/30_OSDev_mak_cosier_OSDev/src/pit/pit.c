/*
void init_pit() 
{
    // Command byte: 0x36 sets channel 0, access mode: lobyte/hibyte, mode 3 (square wave)
    outb(PIT_CMD_PORT, 0x36);
    
    uint16_t divisor = DIVIDER;
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));   // Low byte
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // High byte
}


void sleep_busy(uint32_t milliseconds) 
{
    uint32_t start_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    while (get_current_tick() - start_tick < ticks_to_wait) 
    {
        // Busy waiting: do nothing
    }
}


void sleep_interrupt(uint32_t milliseconds) 
{
    uint32_t current_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_tick = current_tick + ticks_to_wait;
    while (get_current_tick() < end_tick) 
    {
        asm volatile ("sti; hlt");
    }
}
*/