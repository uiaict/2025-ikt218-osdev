#include "song.h"
#include "../PIT/pit.h"
#include "libc/common.h"
#include "libc/stdio.h"
#include "libc/monitor.h"
#include "libc/system.h"
#include "../memory/memory.h"
#include "SongPlayer.h"

void enable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 0x03);
}

void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC);
}

void play_sound(uint32_t frequency) {
    if (frequency == 0) return;

    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;

    outb(PIT_CMD_PORT, 0xB6);   // Set PIT to mode 3, binary

    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);       // Low byte
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF); // High byte

    enable_speaker();          // Start playing
}

volatile bool stop_requested = false;

void play_song_impl(Song *song) {
    if (!song || !song->notes || song->length == 0) {
        monitor_write("Invalid song\n");
        return;
    }

    // Reset stop flag before starting song
    stop_song_requested = false;
    
    monitor_write("Playing song: ");
    monitor_write(song->name);
    monitor_write("\nPress ESC to stop...\n");

    for (uint32_t i = 0; i < song->length && !stop_song_requested; i++) {
        Note current_note = song->notes[i];
        
        if (current_note.frequency > 0)
            play_sound(current_note.frequency);
        else
            disable_speaker(); // Rest

        // Play the note for its duration, but check for stop request periodically
        if (current_note.duration > 0) {
            // Break the sleep into smaller chunks to check for stop flag more frequently
            uint32_t remaining = current_note.duration;
            uint32_t check_interval = 50; // Check every 50ms
            
            while (remaining > 0 && !stop_song_requested) {
                uint32_t sleep_time = (remaining > check_interval) ? check_interval : remaining;
                sleep_interrupt(sleep_time);
                remaining -= sleep_time;
                
                // Check if ESC was pressed
                if (stop_song_requested) {
                    break;
                }
            }
        }

        disable_speaker();
        
        // Exit loop if stop was requested
        if (stop_song_requested) {
            break;
        }
    }
    
    disable_speaker();
    
    monitor_write(stop_song_requested ? "Song stopped.\n" : "Song finished!\n");
}


SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    if (player) {
        player->play_song = play_song_impl;
    }
    return player;
}

void play_song(Song *song) {
    play_song_impl(song);
}

void stop_sound() {
    disable_speaker();
}
