#include "programmableIntervalTimer.h"
#include "display.h"
#include "interruptHandler.h"

// Teller for timer ticks
static volatile uint32_t tick_count = 0;

/**
 * Timer IRQ handler (IRQ0)
 * Øker timer tick telleren
 */
void timer_handler(void) {
    tick_count++;
    // Send End of Interrupt til PIC1
    outb(PIC1_COMMAND, 0x20);
}

/**
 * Initialiserer den programmerbare interval timeren
 * Konfigurerer PIT til å generere timer interrupts med spesifisert frekvens
 */
void init_programmable_interval_timer(void) {
    display_write_color("Initializing Programmable Interval Timer...\n", COLOR_WHITE);
    
    // Beregn divisor basert på ønsket frekvens
    uint32_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;
    
    // Gjør divisor partall for Mode 3
    if (divisor & 1) {
        divisor &= ~1;
    }
    
    // Sikre gyldig divisor område
    if (divisor < 2) divisor = 2;
    if (divisor > 65536) divisor = 65536;
    
    // Deaktiver interrupts under konfigurasjon
    __asm__ volatile("cli");
    
    // Konfigurer PIT mode - Kanal 0, Mode 3
    outb(PIT_COMMAND_PORT, PIT_CHANNEL0 | PIT_LOHI | PIT_MODE3);
    
    // Sett frekvens divisor
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);
    
    // Aktiver timer interrupt i PIC
    uint8_t mask = inb(PIC1_DATA);
    mask &= ~(1 << 0);
    outb(PIC1_DATA, mask);
    
    // Nullstill tick teller
    tick_count = 0;
    
    // Reaktiver interrupts
    __asm__ volatile("sti");
    
    // Beregn og vis faktisk frekvens
    uint32_t actual_freq = PIT_BASE_FREQUENCY / divisor;
    
    display_write_color("Timer initialized with frequency: ", COLOR_LIGHT_GREEN);
    display_write_decimal(actual_freq);
    display_write(" Hz (divisor: ");
    display_write_decimal(divisor);
    display_write(")\n");
    
    display_write_color("Timer interrupt (IRQ0) enabled\n", COLOR_LIGHT_GREEN);
}

// Returner nåværende tick teller
uint32_t get_current_tick(void) {
    return tick_count;
}

/**
 * Vent i spesifisert antall millisekunder ved å bruke busy waiting
 */
void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = tick_count;
    uint32_t wait_ticks = milliseconds;
    
    while (tick_count - start_tick < wait_ticks) {
        __asm__ volatile("pause");
    }
}

/**
 * Vent i spesifisert antall millisekunder ved å bruke interrupts
 */
void sleep_interrupt(uint32_t milliseconds) {
    // Bruk busy waiting for veldig korte forsinkelser
    if (milliseconds < 5) {
        sleep_busy(milliseconds);
        return;
    }

    // Sjekk om interrupts er aktivert
    uint32_t flags;
    __asm__ volatile("pushf\n\t"
                    "pop %0"
                    : "=r"(flags));
    
    if (!(flags & 0x200)) {
        sleep_busy(milliseconds);
        return;
    }
    
    uint32_t start_tick = tick_count;
    uint32_t wait_ticks = (milliseconds * TARGET_FREQUENCY) / 1000;
    if (wait_ticks == 0 && milliseconds > 0) {
        wait_ticks = 1;
    }
    
    while (tick_count - start_tick < wait_ticks) {
        __asm__ volatile("pause");
    }
} 