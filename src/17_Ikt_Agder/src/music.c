#include "libc/music.h"
#include "libc/sound.h"
#include "libc/fs.h"
#include "libc/stdio.h"

// Initialize music system (sound and file system)
void init_music(void) {
    init_sound();
}

// Play a WAV file
void play_wav(const char *filename) {
    size_t length;
    uint8_t *file_data = read_file(filename, &length);

    if (file_data == NULL) {
        printf("Error: Could not load the file.\n");
        return;
    }

    // Here we would normally parse the WAV file and play the data.
    // For simplicity, we are playing raw sound data.
    play_sound(file_data, length);
}

// Stop any playing music
void stop_music(void) {
    stop_sound();
}
