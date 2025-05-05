#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "menu.h"

void music_player_show_menu(void);
KernelMode music_player_handle_input(char key);
KernelMode music_player_update(void);
void music_player_cleanup(void);

#endif // MUSIC_PLAYER_H
