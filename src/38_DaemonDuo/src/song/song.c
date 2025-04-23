#include "song.h"
#include "song_player.h"
#include "pit.h"
//#include "common.h"
#include "libc/stdio.h"

// Create a song with values that fit in uint16_t
const uint16_t example_song[] = {
    // Define your song here, for example:
    // Format: [frequency, duration]
    440, 500,  // 440 Hz for 500 ms
    880, 500,  // 880 Hz for 500 ms
    440, 500,  // 440 Hz for 500 ms
    0, 0       // End of song marker
};

void play_song(const uint16_t* song_data) {
        int i = 0;
    while (song_data[i] != 0) {  // 0 marks the end of the song
        uint32_t frequency = song_data[i];
        uint32_t duration = song_data[i + 1];
                
        play_sound(frequency);  // Play sound with the given frequency
        delay(duration);        // Wait for the duration of the note
        stop_sound();           // Stop the sound

        i += 2;  // Move to the next note (2 steps: frequency and duration)
    }
}
