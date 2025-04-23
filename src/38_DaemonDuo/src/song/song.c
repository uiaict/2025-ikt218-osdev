#include "song.h"
#include "song_player.h"
#include "pit.h"
#include "terminal.h"
#include "frequencies.h"

// Define an example song (simple scale)
const struct note example_song[] = {
    { 262, 200, 50 },  // C4
    { 294, 200, 50 },  // D4
    { 330, 200, 50 },  // E4
    { 349, 200, 50 },  // F4
    { 392, 200, 50 },  // G4
    { 440, 200, 50 },  // A4
    { 494, 200, 50 },  // B4
    { 523, 200, 50 },  // C5
    END_OF_SONG       // End of song marker
};

// Play a song defined as an array of notes
void play_song(const struct note song[]) {
    int i = 0;
    
    // Enable the speaker at the beginning
    enable_speaker();

    // Loop through the song until we reach the end marker
    while (song[i].frequency != 0) {
        // Print the note being played
        printf("Playing note: %d Hz for %d ms\n", song[i].frequency, song[i].duration);
        
        // Play the current note
        play_sound(song[i].frequency);
        
        // Play for the specified duration
        sleep_interrupt(song[i].duration);

        // Stop the sound
        stop_sound();
        
        // Pause between notes if needed
        if (song[i].pause > 0) {
            sleep_interrupt(song[i].pause);
        }
        
        // Move to the next note
        i++;
    }
    writeline("Finished playing song\n");
    // Disable the speaker at the end
    disable_speaker();
}
