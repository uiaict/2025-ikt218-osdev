#include "musicPlayer.h"
#include "display.h"
#include "pcSpeaker.h"
#include "memory_manager.h"
#include "miscFuncs.h"
#include "programmableIntervalTimer.h"
#include "interruptHandler.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

/**
 * Oppretter en ny sangavspiller
 */
SongPlayer* create_song_player(void) {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    if (player) {
        player->play_song = play_song_impl;
    }
    return player;
}

/**
 * Implementasjon av sangavspilling som håndterer avspilling av noter
 */
void play_song_impl(Song* song) {
    if (!song || !song->notes || song->length == 0) {
        display_write_color("Ugyldige sangdata\n", COLOR_RED);
        return;
    }
    
    // Slå av høyttaler for å sikre ren start
    disable_speaker();
    
    // Spill av hver note i sekvens
    for (uint32_t i = 0; i < song->length; i++) {
        const Note* current_note = &song->notes[i];
        
        // Spill av noten med direkte kontroll for bedre ytelse
        if (current_note->frequency > 0) {
            // Beregn divisor
            uint16_t divisor = 1193180 / current_note->frequency;
            
            // Deaktiver interrupts under lydkonfigurasjon
            __asm__ volatile("cli");
            
            // Konfigurer PIT kanal 2
            outb(0x43, 0xB6);
            outb(0x42, divisor & 0xFF);
            outb(0x42, (divisor >> 8) & 0xFF);
            
            // Aktiver høyttaler
            outb(0x61, inb(0x61) | 0x03);
            
            // Reaktiver interrupts
            __asm__ volatile("sti");
        } else {
            // Slå av høyttaler for pause
            disable_speaker();
        }
        
        // Vent i notens varighet
        uint32_t start_time = get_current_tick();
        while (get_current_tick() - start_time < current_note->duration) {
            __asm__ volatile("pause");
        }
        
        // Stopp lyden eksplisitt
        disable_speaker();
        
        // Legg til liten pause mellom noter (5ms) for bedre lydseparasjon
        start_time = get_current_tick();
        while (get_current_tick() - start_time < 5) {
            __asm__ volatile("pause");
        }
    }
    
    // Sikre at høyttaler er avslått til slutt
    disable_speaker();
}

/**
 * Spill av en sang med standard implementasjon
 */
void play_song(Song* song) {
    play_song_impl(song);
}

/**
 * Frigjør en sangavspiller
 */
void free_song_player(SongPlayer* player) {
    if (player) {
        free(player);
    }
}

/**
 * Frigjør sangressurser
 */
void free_song(Song* song) {
    if (song) {
        if (song->notes) {
            free(song->notes);
        }
        free(song);
    }
}

/**
 * Oppretter en ny note
 */
Note* create_note(uint32_t frequency, uint32_t duration) {
    Note* note = (Note*)malloc(sizeof(Note));
    if (note) {
        note->frequency = frequency;
        note->duration = duration;
    }
    return note;
}

/**
 * Oppretter en ny sang
 */
Song* create_song(Note* notes, uint32_t length) {
    Song* song = (Song*)malloc(sizeof(Song));
    if (song) {
        song->notes = notes;
        song->length = length;
    }
    return song;
} 