#include "pcSpeaker.h"
#include "display.h"
#include "programmableIntervalTimer.h"
#include "interruptHandler.h"

// Globale variabler for å unngå gjentatte I/O-operasjoner
static uint8_t speaker_state = 0;
static uint32_t current_frequency = 0;

/**
 * Aktiverer PC-høyttaleren
 * Setter bit 0 og 1 i kontrollporten for å aktivere lydutgang
 */
void enable_speaker(void) {
    uint8_t value = inb(0x61);
    value |= 0x03;
    outb(0x61, value);
    speaker_state = value;
}

/**
 * Deaktiverer PC-høyttaleren
 * Nullstiller bit 0 og 1 i kontrollporten for å deaktivere lydutgang
 */
void disable_speaker(void) {
    uint8_t value = inb(0x61);
    value &= ~0x03;
    outb(0x61, value);
    speaker_state = value;
    current_frequency = 0;
}

/**
 * Beregner PIT divisor for en gitt frekvens
 * PIT grunnfrekvens er 1.193180 MHz
 */
uint32_t calculate_pit_divisor(uint32_t frequency) {
    if (frequency == 0) return 0;
    return 1193180 / frequency;
}

/**
 * Spiller av en lyd med spesifisert frekvens
 * Konfigurerer PIT kanal 2 og aktiverer PC-høyttaleren
 */
void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        disable_speaker();
        return;
    }
    
    // Beregn PIT divisor for ønsket frekvens
    uint32_t divisor = calculate_pit_divisor(frequency);
    
    // Deaktiver interrupts under PIT-konfigurasjon
    __asm__ volatile("cli");
    
    // Konfigurer PIT kanal 2 for firkantbølgegenerering
    outb(0x43, 0xB6);
    outb(0x42, divisor & 0xFF);
    outb(0x42, (divisor >> 8) & 0xFF);
    
    // Aktiver høyttaler
    uint8_t value = inb(0x61);
    value |= 0x03;
    outb(0x61, value);
    speaker_state = value;
    current_frequency = frequency;
    
    // Reaktiver interrupts
    __asm__ volatile("sti");
}

/**
 * Stopper lydavspilling
 * Deaktiverer PC-høyttaleren
 */
void stop_sound(void) {
    disable_speaker();
} 