extern "C"{
    #include "libc/system.h"
    #include "memory.h"
    #include "ports.h"
    #include "keyboard.h"
    #include "song.h"
}

void* operator new(size_t size) {
    return malloc(size);
}

void* operator new[](size_t size) {
    return malloc(size);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

void operator delete(void* ptr, size_t size) noexcept {
    (void)size;
    free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size;
    free(ptr);
}

SongPlayer* create_song_player() {
    SongPlayer* player = new SongPlayer();
    player->play_song = play_song_impl;
    return player;
}

extern "C" void play_star_wars() {
    SongPlayer* player = create_song_player();
    Song* song = new Song({starwars_theme, sizeof(starwars_theme) / sizeof(Note)});
    player->play_song(song);
}