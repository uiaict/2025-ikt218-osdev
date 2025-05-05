#ifndef GAME_H
#define GAME_H
#include "libc/stdbool.h"
#include "libc/monitor.h"
#include "libc/string.h"
#include "../keyboard/keyboard.h"

typedef struct {
    char *name;
    char *description;
    int north, south, east, west; // indexes to other rooms
    bool has_key;
    bool has_torch;
    bool is_dark;
    bool is_locked;
    bool is_exit;
} Room;

typedef struct {
    Room *rooms;
    int current_room;
    int has_torch;
    int has_key;
} GameState;

void run_game();
void process_game_command(char *input, GameState* state);
void init_game();
void try_move(GameState *state, Room rooms[], const char *direction);
#endif