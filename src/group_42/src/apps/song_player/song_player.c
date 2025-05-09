#include "song_player/song_player.h"
#include "kernel/pit.h"
#include "kernel/system.h"
#include "libc/stddef.h"
#include "libc/stdio.h"

void enable_speaker() {
  // Reads current state and sets bits 0 and 1
  uint8_t current_state = inb(PC_SPEAKER_PORT);
  // Only write if needed to potentially reduce clicks
  if ((current_state & 0x03) != 0x03) {
    outb(PC_SPEAKER_PORT, current_state | 0x03);
  }
}

void disable_speaker() {
  // Reads current state and clears bits 0 and 1
  uint8_t current_state = inb(PC_SPEAKER_PORT);
  outb(PC_SPEAKER_PORT, current_state & 0xFC); // Clears bits 0 and 1
}

void stop_sound() {
  uint8_t speaker_state = inb(PC_SPEAKER_PORT);
  // Clear only bit 1
  outb(PC_SPEAKER_PORT, speaker_state & 0xFD);
}

void play_sound(uint32_t frequency) {
  if (frequency == 0) {
    stop_sound(); // Stop sound for rest notes
    return;
  }

  uint16_t divisor = PIT_BASE_FREQUENCY / frequency;

  // Send command 0xB6: Channel 2, lobyte/hibyte, mode 3 (square wave)
  outb(PIT_CMD_PORT, 0xB6);

  // Send divisor low byte then high byte
  outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));
  outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor >> 8));

  enable_speaker();
}

void play_song_impl(Song *song) {
  for (uint32_t i = 0; i < song->length; i++) {
    Note current_note = song->notes[i];
    play_sound(current_note.frequency);

    if (current_note.duration > 0) {
      sleep_interrupt(current_note.duration);
    }

    stop_sound();
  }
  
  disable_speaker();
}

void play_song(Song *song) {
  if (song != NULL && song->notes != NULL && song->length > 0) {
    play_song_impl(song);
  } else {
    printf("Invalid song data\n");
  }
}

SongPlayer *create_song_player() {
  static SongPlayer player = {play_song};
  return &player;
}