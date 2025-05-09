#include <driver/include/test_speaker.h>
#include <song/song.h>
#include <kernel/interrupt/pit.h>
#include <libc/printf.h>

////////////////////////////////////////
// PC Speaker Test Routine
////////////////////////////////////////

// Run a sequence of test tones on the PC speaker
void test_speaker() {
    printf("===== PC SPEAKER TEST =====\n");

    printf("Initializing speaker...\n");
    stop_sound();
    sleep_busy(500);

    ////////////////////////////////////////
    // Test 1: 1000 Hz tone (2 seconds)
    ////////////////////////////////////////
    printf("Test 1: 1000Hz tone for 2 seconds\n");
    play_sound(1000);
    sleep_busy(2000);
    stop_sound();
    sleep_busy(500);

    ////////////////////////////////////////
    // Test 2: 440 Hz tone (A4 note)
    ////////////////////////////////////////
    printf("Test 2: 440Hz tone (A4 note) for 1 second\n");
    play_sound(440);
    sleep_busy(1000);
    stop_sound();
    sleep_busy(500);

    ////////////////////////////////////////
    // Test 3: 880 Hz tone (A5 note)
    ////////////////////////////////////////
    printf("Test 3: 880Hz tone (A5 note) for 1 second\n");
    play_sound(880);
    sleep_busy(1000);
    stop_sound();
    sleep_busy(500);

    ////////////////////////////////////////
    // Test 4: C Major Scale (C4 to C5)
    ////////////////////////////////////////
    printf("Test 4: Simple scale (C major)\n");
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
