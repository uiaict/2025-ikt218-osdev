// song_player.c
#include "song.h"
#include "pit.h"
#include "port_io.h"
#include "printf.h"
#include "serial.h"
#include "kmem.h"

// PC Speaker ports
#define SPEAKER_CTRL_PORT 0x61
#define PIT_CHANNEL2_PORT 0x42
#define PIT_COMMAND_PORT  0x43
#define PIT_FREQUENCY     1193180

// Helper function to convert an integer to a string
void int_to_str(int value, char* buffer) {
    char tmp[16];
    int i = 0;
    int is_negative = 0;
    
    // Handle 0 specially
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    
    // Handle negative numbers
    if (value < 0) {
        is_negative = 1;
        value = -value;
    }
    
    // Convert digits in reverse order
    while (value != 0) {
        tmp[i++] = '0' + (value % 10);
        value /= 10;
    }
    
    // Add negative sign if needed
    int j = 0;
    if (is_negative) {
        buffer[j++] = '-';
    }
    
    // Copy digits in correct order
    while (i > 0) {
        buffer[j++] = tmp[--i];
    }
    
    buffer[j] = '\0'; // Null terminate the string
}

// Function to enable PC speaker output
void enable_speaker() {
    uint8_t tmp = inb(SPEAKER_CTRL_PORT);
    outb(SPEAKER_CTRL_PORT, tmp | 3);  // Set bits 0 and 1
}

// Function to disable PC speaker output
void disable_speaker() {
    uint8_t tmp = inb(SPEAKER_CTRL_PORT);
    outb(SPEAKER_CTRL_PORT, tmp & 0xFC);  // Clear bits 0 and 1
}

// Function to play a sound at the specified frequency
void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        disable_speaker();
        return;
    }

    // Calculate divisor for PIT
    uint32_t divisor = PIT_FREQUENCY / frequency;

    // Set up PIT channel 2 (mode 3, square wave generator)
    outb(PIT_COMMAND_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);         // Low byte
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);  // High byte

    enable_speaker();
}

// Function to stop sound output
void stop_sound() {
    disable_speaker();
}

// Implementation of the song player that plays all notes in a song
void play_song_impl(Song *song) {
    char buffer[32];  // Buffer for number conversion
    
    // Print beginning message
    terminal_write("Playing song with ");
    int_to_str(song->length, buffer);
    terminal_write(buffer);
    terminal_write(" notes...\n");
    
    serial_write("Playing song with ");
    serial_write(buffer);
    serial_write(" notes...\n");
    
    // Make sure speaker is initially off
    stop_sound();
    
    // Play each note in the song
    for (size_t i = 0; i < song->length; ++i) {
        Note n = song->notes[i];
        
        // Convert note index to string
        int_to_str((int)i+1, buffer);
        terminal_write("Note ");
        terminal_write(buffer);
        terminal_write(": ");
        
        // Convert frequency to string
        int_to_str(n.frequency, buffer);
        terminal_write("freq=");
        terminal_write(buffer);
        terminal_write(" Hz, ");
        
        // Convert duration to string
        int_to_str(n.duration, buffer);
        terminal_write("duration=");
        terminal_write(buffer);
        terminal_write(" ms\n");
        
        // Same for serial output
        int_to_str((int)i+1, buffer);
        serial_write("Note ");
        serial_write(buffer);
        serial_write(": ");
        
        // Convert frequency to string
        int_to_str(n.frequency, buffer);
        serial_write("freq=");
        serial_write(buffer);
        serial_write(" Hz, ");
        
        // Convert duration to string
        int_to_str(n.duration, buffer);
        serial_write("duration=");
        serial_write(buffer);
        serial_write(" ms\n");
        
        // Play the current note
        play_sound(n.frequency);
        sleep_busy(n.duration);
        stop_sound();
        
        // Small pause between notes to distinguish them
        sleep_busy(30);
    }

    // Print completion message
    terminal_write("Song finished.\n");
    serial_write("Song finished.\n");
}

// Function to create a new SongPlayer instance
SongPlayer* create_song_player(void) {
    SongPlayer* player = (SongPlayer*)kmalloc(sizeof(SongPlayer), 0);
    player->play_song = play_song_impl;
    return player;
}