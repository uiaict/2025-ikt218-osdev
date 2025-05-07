#include "test_speaker.h"
#include "song_player.h"
#include "pit.h"
#include "printf.h"

// Test the PC speaker with simple tones
void test_speaker() {
    printf("===== PC SPEAKER TEST =====\n");
    
    // Make sure speaker starts in a known state
    printf("Initializing speaker...\n");
    stop_sound();
    sleep_busy(500);
    
    // Test 1: Play a simple 1kHz tone for 2 seconds
    printf("Test 1: 1000Hz tone for 2 seconds\n");
    play_sound(1000);
    sleep_busy(2000);
    stop_sound();
    sleep_busy(500);
    
    // Test 2: Play a lower tone (440Hz - A4 note) for 1 second
    printf("Test 2: 440Hz tone (A4 note) for 1 second\n");
    play_sound(440);
    sleep_busy(1000);
    stop_sound();
    sleep_busy(500);
    
    // Test 3: Play a higher tone (880Hz - A5 note) for 1 second
    printf("Test 3: 880Hz tone (A5 note) for 1 second\n");
    play_sound(880);
    sleep_busy(1000);
    stop_sound();
    sleep_busy(500);
    
    // Test 4: Play a simple scale
    printf("Test 4: Simple scale (C major)\n");
    
    // C4 to C5 scale
    uint16_t scale[] = {262, 294, 330, 349, 392, 440, 494, 523};
    
    for (int i = 0; i < 8; i++) {
        printf("Playing note: %dHz\n", scale[i]);
        play_sound(scale[i]);
        sleep_busy(300);
        stop_sound();
        sleep_busy(100);
    }
    
    printf("===== SPEAKER TEST COMPLETE =====\n");
}