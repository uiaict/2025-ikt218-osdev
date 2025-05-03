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
    // Pseudocode for enable_speaker:
    // 1. Read the current state from the PC speaker control port.
    // 2. The control register bits are usually defined as follows:
    //    - Bit 0 (Speaker gate): Controls whether the speaker is on or off.
    //    - Bit 1 (Speaker data): Determines if data is being sent to the speaker.
    // 3. Set both Bit 0 and Bit 1 to enable the speaker.
    //    - Use bitwise OR operation to set these bits without altering others.
}

void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC);
    // Pseudocode for disable_speaker:
    // 1. Read the current state from the PC speaker control port.
    // 2. Clear both Bit 0 and Bit 1 to disable the speaker.
    //    - Use bitwise AND with the complement of 3 (0b11) to clear these bits.
}

void play_sound(uint32_t frequency) {
    if(frequency==0) return;

    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;

    outb(PIT_CMD_PORT, 0xB6);

    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);
    enable_speaker();
    // Pseudocode for play_sound:
    // 1. Check if the frequency is 0. If so, exit the function as this indicates no sound.
    // 2. Calculate the divisor for setting the PIT (Programmable Interval Timer) frequency.
    //    - The PIT frequency is a base value, typically 1.193182 MHz.
    //    - The divisor is PIT frequency divided by the desired sound frequency.
    // 3. Configure the PIT to the desired frequency:
    //    - Send control word to PIT control port to set binary counting, mode 3, and access mode (low/high byte).
    //    - Split the calculated divisor into low and high bytes.
    //    - Send the low byte followed by the high byte to the PIT channel 2 port.
    // 4. Enable the speaker (by setting the appropriate bits) to start sound generation.
}

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

    // Make sure speaker is disabled when we're done
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
    // Pseudocode for play_song:
    // 1. Call play_song_impl with the given song.
    //    - This function handles the entire process of playing each note in the song.
}