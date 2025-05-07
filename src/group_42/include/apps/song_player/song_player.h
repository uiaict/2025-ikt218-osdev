#ifndef SONG_PLAYER_H
#define SONG_PLAYER_H

#include "libc/stdint.h"
#include "song.h"

/**
 * @brief Enable the PC speaker.
 */
void enable_speaker();

/**
 * @brief Disable the PC speaker.
 */
void disable_speaker();

/**
 * @brief Stop the sound.
 */
void stop_sound();

/**
 * @brief Play a sound at a given frequency.
 * @param frequency The frequency of the sound in Hz.
 */
void play_sound(uint32_t frequency);

/**
 * @brief Play a song using the song player.
 * @param song The song to be played.
 */
void play_song_impl(Song *song);

/**
 * @brief Play a song using the song player.
 * @param song The song to be played.
 */
void play_song(Song *song);

/**
 * @brief Create a new song player instance.
 * @return A pointer to the created SongPlayer.
 */
SongPlayer *create_song_player();

#endif