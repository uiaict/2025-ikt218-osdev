#include "song_commands.h"
#include "song_player.h"
#include "libc/stdio.h"
#include "song.h"

void play_music_1() {
    printf("Playing Mario theme...\n");
    Song song_to_play = { music_1, sizeof(music_1) / sizeof(Note) };
    SongPlayer* player = create_song_player();
    player->play_song(&song_to_play);
    printf("Finished playing Mario theme.\n");
}

void play_starwars() {
    printf("Playing Star Wars theme...\n");
    Song song_to_play = { starwars_theme, sizeof(starwars_theme) / sizeof(Note) };
    SongPlayer* player = create_song_player();
    player->play_song(&song_to_play);
    printf("Finished playing Star Wars theme.\n");
}

void play_bf1942() {
    printf("Playing Battlefield 1942 theme...\n");
    Song song_to_play = { battlefield_1942_theme, sizeof(battlefield_1942_theme) / sizeof(Note) };
    SongPlayer* player = create_song_player();
    player->play_song(&song_to_play);
    printf("Finished playing Battlefield 1942 theme.\n");
}

void play_music_2() {
    printf("Playing music_2...\n");
    Song song_to_play = { music_2, sizeof(music_2) / sizeof(Note) };
    SongPlayer* player = create_song_player();
    player->play_song(&song_to_play);
    printf("Finished playing music_2.\n");
}

void play_music_3() {
    printf("Playing music_3...\n");
    Song song_to_play = { music_3, sizeof(music_3) / sizeof(Note) };
    SongPlayer* player = create_song_player();
    player->play_song(&song_to_play);
    printf("Finished playing music_3.\n");
}

void play_music_4() {
    printf("Playing music_4...\n");
    Song song_to_play = { music_4, sizeof(music_4) / sizeof(Note) };
    SongPlayer* player = create_song_player();
    player->play_song(&song_to_play);
    printf("Finished playing music_4.\n");
}

void play_music_5() {
    printf("Playing music_5...\n");
    Song song_to_play = { music_5, sizeof(music_5) / sizeof(Note) };
    SongPlayer* player = create_song_player();
    player->play_song(&song_to_play);
    printf("Finished playing music_5.\n");
}

void play_music_6() {
    printf("Playing music_6...\n");
    Song song_to_play = { music_6, sizeof(music_6) / sizeof(Note) };
    SongPlayer* player = create_song_player();
    player->play_song(&song_to_play);
    printf("Finished playing music_6.\n");
}


void test_sound() {
  uint32_t freq = 440;
  uint32_t duration = 1000;
  printf("Playing test sound...\n");
  play_sound(freq);
  sleep_interrupt(duration);
  stop_sound();
  disable_speaker();
  printf("Sound test finished.\n");
}
