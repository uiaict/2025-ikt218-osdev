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

// Complete Super Mario Bros. theme
const struct note mario_theme[] = {
    // First section
    { E5, 100, 15 },
    { E5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { E5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { C5, 100, 15 },
    { E5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { G5, 100, 15 },
    { 0,  180, 15 }, // Rest
    { G4, 100, 15 },
    { 0,  180, 15 }, // Rest

    // Second section
    { C5, 100, 15 },
    { 0,  100, 15 },  // Rest 
    { G4, 100, 15 },
    { 0,  100, 15 },  // Rest
    { E4, 100, 15 },
    { 0,  100, 15 },  // Rest
    { A4, 100, 15 },
    { 0,  100, 15 },  // Rest
    { B4, 100, 15 },
    { 0,  100, 15 },  // Rest
    { As4, 100, 15 },
    { A4, 100, 15 },

    // Third section
    { G4, 75, 15 },
    { E5, 75, 15 },
    { G5, 75, 15 },
    { A5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { F5, 100, 15 },
    { G5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { E5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { C5, 100, 15 },
    { D5, 100, 15 },
    { B4, 100, 15 },
    { 0,  180, 15 }, // Rest

    // Fourth section - repeat of second section
    { C5, 100, 15 },
    { 0,  100, 15 },  // Rest 
    { G4, 100, 15 },
    { 0,  100, 15 },  // Rest
    { E4, 100, 15 },
    { 0,  100, 15 },  // Rest
    { A4, 100, 15 },
    { 0,  100, 15 },  // Rest
    { B4, 100, 15 },
    { 0,  100, 15 },  // Rest
    { As4, 100, 15 },
    { A4, 100, 15 },

    // Fifth section - repeat of third section
    { G4, 75, 15 },
    { E5, 75, 15 },
    { G5, 75, 15 },
    { A5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { F5, 100, 15 },
    { G5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { E5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { C5, 100, 15 },
    { D5, 100, 15 },
    { B4, 100, 15 },
    { 0,  180, 15 }, // Rest

    // Sixth section - underground theme part
    { G5, 100, 15 },
    { Fs5, 100, 15 },
    { F5, 100, 15 },
    { D5, 100, 15 },
    { E5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { G4, 100, 15 },
    { A4, 100, 15 },
    { C5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { A4, 100, 15 },
    { C5, 100, 15 },
    { D5, 100, 15 },
    { 0,  180, 15 }, // Rest

    // Seventh section
    { G5, 100, 15 },
    { Fs5, 100, 15 },
    { F5, 100, 15 },
    { D5, 100, 15 },
    { E5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { C6, 100, 15 },  // High C
    { 0,  100, 15 },  // Rest
    { C6, 100, 15 },
    { C6, 100, 15 },
    { 0,  180, 15 }, // Rest

    // Eighth section
    { C5, 100, 15 },
    { C5, 100, 15 },
    { C5, 100, 15 },
    { 0,  100, 15 },  // Rest
    { C5, 100, 15 },
    { D5, 100, 15 },
    { E5, 100, 15 },
    { C5, 100, 15 },
    { A4, 100, 15 },
    { G4, 100, 15 },
    { 0,  180, 15 }, // Rest

    // Ninth section - 1-UP section
    { E5, 75, 15 },
    { C5, 75, 15 },
    { G4, 100, 15 },
    { 0,  100, 15 },  // Rest
    { G4, 100, 15 },
    { A4, 100, 15 },
    { F5, 100, 15 },
    { F5, 100, 15 },
    { A4, 100, 15 },
    { 0,  180, 15 }, // Rest

    // Ending section
    { C5, 100, 15 },
    { G4, 100, 15 },
    { E4, 100, 15 },
    { 0,  100, 15 },  // Rest
    { A4, 100, 15 },
    { B4, 100, 15 },
    { A4, 100, 15 },
    { 0,  100, 15 },  // Rest
    { Gs4, 100, 15 },
    { As4, 100, 15 },
    { Gs4, 100, 15 },
    { 0,  100, 15 },  // Rest
    
    // Final notes
    { G4, 75, 15 },
    { Fs4, 75, 15 },
    { G4, 100, 15 },

    END_OF_SONG
};

// Play a song defined as an array of notes
void play_song(const struct note song[]) {
    int i = 0;
    
    // Make sure timer and keyboard interrupts are enabled
    enable_irq(0);
    enable_irq(1);
    
    // Enable the speaker at the beginning
    enable_speaker();

    // Loop through the song until we reach the end marker
    while (!(song[i].frequency == 0 && song[i].duration == 0 && song[i].pause == 0)) {
        // Print the note being played
        printf("Playing note: %d Hz for %d ms\n", song[i].frequency, song[i].duration);
        
        // Play the current note
        if (song[i].frequency > 0) {
            play_sound(song[i].frequency);
            
            // Play for the specified duration with timeout checking
            uint32_t start_tick = tick_count;
            uint32_t duration_ticks = song[i].duration * TICKS_PER_MS;
            
            while (tick_count - start_tick < duration_ticks) {
                // Use hlt to save power and let interrupts happen
                asm volatile("sti; hlt");
                
                // Add a small timeout in case we get stuck
                if (tick_count - start_tick > duration_ticks * 2) {
                    break;
                }
            }
            
            // Stop the sound
            stop_sound();
        } else {
            // Just a rest - no sound, just wait
            uint32_t start_tick = tick_count;
            uint32_t duration_ticks = song[i].duration * TICKS_PER_MS;
            
            while (tick_count - start_tick < duration_ticks) {
                // Use hlt to save power and let interrupts happen
                asm volatile("sti; hlt");
                
                // Add a small timeout in case we get stuck
                if (tick_count - start_tick > duration_ticks * 2) {
                    break;
                }
            }
        }
        
        // Pause between notes if needed
        if (song[i].pause > 0) {
            uint32_t start_tick = tick_count;
            uint32_t pause_ticks = song[i].pause * TICKS_PER_MS;
            
            while (tick_count - start_tick < pause_ticks) {
                // Use hlt to save power and let interrupts happen
                asm volatile("sti; hlt");
                
                // Add a small timeout in case we get stuck
                if (tick_count - start_tick > pause_ticks * 2) {
                    break;
                }
            }
        }
        
        // Move to the next note
        i++;
    }
    
    writeline("Finished playing song\n");
    
    // Disable the speaker at the end
    disable_speaker();
    
    // Reset the PIT to make sure timer interrupts work again
    reset_pit_timer();
    
    // Re-enable the keyboard IRQ explicitly
    enable_irq(1);
    enable_irq(0);
}
