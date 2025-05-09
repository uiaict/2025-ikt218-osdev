#ifndef PIANO_H
#define PIANO_H

#include "libc/stdbool.h"
#include "libc/monitor.h"
#include "../song/SongPlayer.h"
#include "../song/frequencies.h"

void init_piano();
void handle_piano_key(unsigned char scancode);
extern bool piano_mode_enabled;

#endif