#include "music_player.h"
#include "pcspkr.h"
#include "monitor.h"
#include "keyboard.h"
#include "timer.h"  // Include timer.h for sleep function
#include <stddef.h> // For NULL definition

// Flag to indicate if a song is currently playing
static u8int playing = 0;

// Flag to indicate if we're in piano mode
static u8int piano_mode = 0;

// Structure to define a musical note with duration
typedef struct {
    const char* note;  // Note in musical notation (e.g., "C4")
    u32int duration;   // Duration in milliseconds
} music_note_t;

// Structure to define a song
typedef struct {
    const char* name;        // Song name
    music_note_t* notes;     // Array of notes in the song
    u32int note_count;       // Number of notes in the song
} song_t;

// Define some simple songs

// Twinkle Twinkle Little Star
static music_note_t twinkle_notes[] = {
    {"C4", 400}, {"C4", 400}, {"G4", 400}, {"G4", 400},
    {"A4", 400}, {"A4", 400}, {"G4", 800},
    {"F4", 400}, {"F4", 400}, {"E4", 400}, {"E4", 400},
    {"D4", 400}, {"D4", 400}, {"C4", 800}
};

// Happy Birthday
static music_note_t birthday_notes[] = {
    {"C4", 200}, {"C4", 200}, {"D4", 400}, {"C4", 400}, {"F4", 400}, {"E4", 800},
    {"C4", 200}, {"C4", 200}, {"D4", 400}, {"C4", 400}, {"G4", 400}, {"F4", 800},
    {"C4", 200}, {"C4", 200}, {"C5", 400}, {"A4", 400}, {"F4", 400}, {"E4", 400}, {"D4", 800},
    {"A#4", 200}, {"A#4", 200}, {"A4", 400}, {"F4", 400}, {"G4", 400}, {"F4", 800}
};

// Jingle Bells (simplified)
static music_note_t jingle_notes[] = {
    {"E4", 300}, {"E4", 300}, {"E4", 600},
    {"E4", 300}, {"E4", 300}, {"E4", 600},
    {"E4", 300}, {"G4", 300}, {"C4", 450}, {"D4", 150}, {"E4", 900},
    {"F4", 300}, {"F4", 300}, {"F4", 450}, {"F4", 150},
    {"F4", 300}, {"E4", 300}, {"E4", 300}, {"E4", 150}, {"E4", 150},
    {"E4", 300}, {"D4", 300}, {"D4", 300}, {"E4", 300}, {"D4", 600}, {"G4", 600}
};

// Collection of all songs
static song_t songs[] = {
    {"Twinkle Twinkle", twinkle_notes, sizeof(twinkle_notes) / sizeof(music_note_t)},
    {"Happy Birthday", birthday_notes, sizeof(birthday_notes) / sizeof(music_note_t)},
    {"Jingle Bells", jingle_notes, sizeof(jingle_notes) / sizeof(music_note_t)}
};

// Number of songs in the collection
static const u8int SONG_COUNT = sizeof(songs) / sizeof(song_t);

// Piano keyboard mapping (from computer keyboard to musical notes)
// This maps the middle row of a QWERTY keyboard to a C major scale
typedef struct {
    char key;           // Keyboard key
    const char* note;   // Musical note
} piano_key_t;

static piano_key_t piano_keys[] = {
    {'a', "C4"}, {'s', "D4"}, {'d', "E4"}, {'f', "F4"},
    {'g', "G4"}, {'h', "A4"}, {'j', "B4"}, {'k', "C5"},
    {'l', "D5"}, {';', "E5"},
    // Sharps/flats on the row above
    {'w', "C#4"}, {'e', "D#4"}, {'t', "F#4"},
    {'y', "G#4"}, {'u', "A#4"},
    {0, NULL} // Terminator
};

// Initialize the music player
void init_music_player() {
    // Initialize the PC speaker
    init_pcspkr();
    monitor_write("Music player initialized\n");
    monitor_write("Press 1-");
    monitor_write_dec(SONG_COUNT);
    monitor_write(" to play a song, 'p' for piano mode, 'q' to stop\n");
}

// Play built-in song by index
void music_play_song(u8int song_index) {
    // Check if the song index is valid
    if (song_index >= SONG_COUNT) {
        monitor_write("Invalid song index\n");
        return;
    }

    // Stop any currently playing song and exit piano mode
    music_stop();
    piano_mode = 0;

    // Mark as playing
    playing = 1;

    // Get the song
    song_t song = songs[song_index];

    // Display which song we're playing
    monitor_write("\nPlaying: ");
    // Need to cast to non-const to match monitor_write function signature
    monitor_write((char*)song.name);
    monitor_write("\n");

    // Play each note in the song
    for (u32int i = 0; i < song.note_count && playing; i++) {
        music_note_t note = song.notes[i];
        pcspkr_play_note(note.note, note.duration);

        // Small pause between notes
        sleep(50);
    }

    // Song finished or was stopped
    playing = 0;
    monitor_write("Song ended\n");
}

// Stop currently playing song
void music_stop() {
    playing = 0;
    pcspkr_stop();
}

// Number of built-in songs available
u8int music_get_song_count() {
    return SONG_COUNT;
}

// Get the name of a song by index
const char* music_get_song_name(u8int song_index) {
    if (song_index < SONG_COUNT) {
        return songs[song_index].name;
    }
    return "Unknown Song";
}

// Enter piano mode
void music_enter_piano_mode() {
    // Stop any currently playing song
    music_stop();

    // Enter piano mode
    piano_mode = 1;

    monitor_write("Piano Mode active\n");
    monitor_write("Use A-L keys for C4-E5 scale, W,E,T,Y,U for sharps\n");
    monitor_write("Press 'q' to exit piano mode\n");
}

// Exit piano mode
void music_exit_piano_mode() {
    piano_mode = 0;
    pcspkr_stop();
    monitor_write("Exited piano mode\n");
}

// Check if we're in piano mode
u8int music_is_piano_mode() {
    return piano_mode;
}

// Play a note in piano mode
void music_play_piano_note(char key) {
    // Check if we're in piano mode
    if (!piano_mode) return;

    // Exit piano mode if 'q' is pressed
    if (key == 'q') {
        music_exit_piano_mode();
        return;
    }

    // Find the note for this key
    for (int i = 0; piano_keys[i].key != 0; i++) {
        if (piano_keys[i].key == key) {
            // Found a matching key
            pcspkr_play_note(piano_keys[i].note, 200);
            return;
        }
    }

    // No matching key found
    pcspkr_stop();
}
