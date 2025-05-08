#include "apps/game/snake.h"
#include "apps/song/song.h"

// single global player created at init
static SongPlayer *player;

void game_sound_init(void)
{
    player = create_song_player();
}

// short beep when food is collected 
void game_sound_food(void)
{
    extern Song song_food;      /* declared in song.h */
    player->play_song(&song_food);
}

// longer intro melody at game start 
void game_sound_opening(void)
{
    extern Song song_open;      // declared in song.h 
    player->play_song(&song_open);
}