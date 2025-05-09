#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include "song.h"
#include "libc/common.h"
#include "libc/stdio.h"
#include "libc/monitor.h"
#include "libc/system.h"
#include "../PIT/pit.h"
#include "../memory/memory.h"

void enable_speaker();
void disable_speaker();
void stop_sound();
void play_sound(uint32_t frequency);
void stop_sound();
void play_song(Song *song);

extern volatile bool stop_song_requested;

#endif /* SONGPLAYER_H */