#ifndef PC_SPEAKER_H
#define PC_SPEAKER_H

#include "libc/stdint.h"

// PC høyttaler porter
#define PC_SPEAKER_PORT 0x61
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL2_PORT 0x42

// PIT frekvens konstanter
#define PIT_BASE_FREQUENCY 1193180

/**
 * Aktiverer PC høyttaleren
 * 
 * Setter riktige bits i kontrollporten for å aktivere høyttaleren
 */
void enable_speaker(void);

/**
 * Deaktiverer PC høyttaleren
 * 
 * Nullstiller bits i kontrollporten for å deaktivere høyttaleren
 */
void disable_speaker(void);

/**
 * Spiller av lyd med angitt frekvens
 * 
 * @param frequency Ønsket frekvens i Hz
 */
void play_sound(uint32_t frequency);

/**
 * Stopper lydavspilling
 * 
 * Stopper eventuell pågående lydavspilling
 */
void stop_sound(void);

/**
 * Beregner PIT divisor for gitt frekvens
 * 
 * @param frequency Ønsket frekvens i Hz
 * @return Divisor verdi for bruk med PIT
 */
uint32_t calculate_pit_divisor(uint32_t frequency);

#endif /* PC_SPEAKER_H */ 