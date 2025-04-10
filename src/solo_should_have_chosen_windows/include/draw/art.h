#pragma once

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

typedef struct {
    char name[SCREEN_WIDTH];
    char board[SCREEN_SIZE];
} Drawing;

typedef struct {
    bool (*space_available)(void);
    void (*create_drawing)(char* str);
    bool (*drawings_exist)(void);
    Drawing* (*fetch_drawing)(char* str);
    void (*save_drawing)(Drawing* drawing);
    void (*print_board)(Drawing* drawing);
} ArtManager;

ArtManager* create_art_manager(void);
void destroy_art_manager(ArtManager *manager);