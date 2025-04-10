#include "musicplayer.h"
#include "memory.h"
#include "kprint.h"
#include "pit.h"
#include "isr.h"

// Speaker port
#define SPEAKER_PORT 0x61

// Play a direct beep sound (bypassing PIT for testing)
void play_direct_beep(uint32_t duration_ms) {
    // Direct method to generate a beep
    uint8_t tmp = inb(SPEAKER_PORT);
    
    // Turn speaker on (alternate between on and off quickly)
    for (uint32_t i = 0; i < duration_ms; i++) {
        // Turn on
        outb(SPEAKER_PORT, tmp | 3);
        
        // Short delay
        for (volatile uint32_t j = 0; j < 10000; j++);
        
        // Turn off
        outb(SPEAKER_PORT, tmp & 0xFC);
        
        // Short delay
        for (volatile uint32_t j = 0; j < 10000; j++);
    }
}

// Play sound at specified frequency
void play_sound(uint32_t frequency) {
    if (frequency == 0) return;
    
    // The PIT runs at 1,193,180 Hz
    uint32_t divisor = 1193180 / frequency;
    
    // Save interrupt flag and disable interrupts
    uint32_t eflags;
    __asm__ volatile ("pushfl; popl %0; cli" : "=r" (eflags));
    
    // Configure PIT channel 2
    outb(PIT_CMD_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);
    
    // Read current value of speaker port
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    
    // Enable speaker
    outb(PC_SPEAKER_PORT, tmp | 3);
    
    // Restore interrupt flag
    if (eflags & 0x200) __asm__ volatile ("sti");
    
    kprint("Speaker enabled with frequency ");
    kprint_dec(frequency);
    kprint(" Hz\n");
}

// Stop the sound
void stop_sound(void) {
    // Save interrupt flag and disable interrupts
    uint32_t eflags;
    __asm__ volatile ("pushfl; popl %0; cli" : "=r" (eflags));
    
    // Read current value of speaker port
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    
    // Disable speaker
    outb(PC_SPEAKER_PORT, tmp & 0xFC);
    
    // Restore interrupt flag
    if (eflags & 0x200) __asm__ volatile ("sti");
    
    kprint("Speaker disabled\n");
}

// Modified implementation to try different approaches
void play_song_impl(Song* song) {
    if (!song || !song->notes || song->note_count == 0) {
        kprint("Invalid song or empty song\n");
        return;
    }
    
    // Try a direct beep first to see if speaker works at all
    kprint("Testing direct beep...\n");
    play_direct_beep(500);
    sleep_interrupt(500);
    
    // Now play the actual song
    for (size_t i = 0; i < song->note_count; i++) {
        kprint("Playing note ");
        kprint_dec(i);
        kprint(" - Frequency: ");
        kprint_dec(song->notes[i].freq);
        kprint(" Hz, Duration: ");
        kprint_dec(song->notes[i].duration);
        kprint(" ms\n");
        
        if (song->notes[i].freq > 0) {
            // Try with much higher frequencies for testing
            uint32_t test_freq = song->notes[i].freq * 4;
            kprint("Using test frequency: ");
            kprint_dec(test_freq);
            kprint(" Hz\n");
            
            play_sound(test_freq);
            sleep_interrupt(song->notes[i].duration);
            stop_sound();
        } else {
            sleep_interrupt(song->notes[i].duration);
        }
        
        sleep_interrupt(10);
    }
}

// Create a new SongPlayer
SongPlayer* create_song_player(void) {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    if (player) {
        player->play_song = play_song_impl;
    }
    return player;
}