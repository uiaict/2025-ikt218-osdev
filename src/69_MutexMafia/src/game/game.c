#include "game.h"


Bird bird;
Pipe pipes[3];
int score = 0;
bool game_over = false;
bool flap = false;
extern int index; // Index for terminalBuffer
extern char terminalBuffer[]; // Buffer for keyboard input


void reset_game() {
    bird.y = SCREEN_HEIGHT / 4;
    bird.velocity = 0;

    for (int i = 0; i < 3; ++i) {
        pipes[i].x = SCREEN_WIDTH + i * (SCREEN_WIDTH / 3);
        pipes[i].gap_y = 5 + (i * 3) % (SCREEN_HEIGHT - GAP_HEIGHT - 2);
    }

    score = 0;
    game_over = false;
}

void handle_game_input() {
    if (index == 0) return;

    char key = terminalBuffer[index - 1]; // get the last key typed
    index = 0; // clear after processing

    if (key == ' ' || key == 'w') {
        flap = true;
    } else if (key == 'r' || key == 'R') {
        game_over = true;
    } else if (key == 27) { // ESC
        game_over = true;
    }
}


void update_game() {
    // Bird movement
    if (flap) {
        bird.velocity = FLAP_STRENGTH;
        //flap sound here?
    } else {
        bird.velocity += GRAVITY;
    }

    bird.y += bird.velocity;
    flap = false;

    // Move pipes
    for (int i = 0; i < 3; ++i) {
        pipes[i].x -= PIPE_SPEED;

        if (pipes[i].x + PIPE_WIDTH < 0) {
            pipes[i].x = SCREEN_WIDTH;
            pipes[i].gap_y = 3 + (score * 7) % (SCREEN_HEIGHT - GAP_HEIGHT - 3);
            score++;
        }
    }

    // Collision detection
    if (bird.y < 0 || bird.y >= SCREEN_HEIGHT) {
        game_over = true;
        return;
    }

    for (int i = 0; i < 3; ++i) {
        if (pipes[i].x <= 5 && pipes[i].x + PIPE_WIDTH >= 3) {
            if (bird.y < pipes[i].gap_y || bird.y > pipes[i].gap_y + GAP_HEIGHT) {
                game_over = true;
                return;
            }
        }
    }
}

void draw_game() {
    clear_screen();

    // Draw pipes
    for (int i = 0; i < 3; ++i) {
        for (int y = 0; y < SCREEN_HEIGHT; ++y) {
            if (y < pipes[i].gap_y || y > pipes[i].gap_y + GAP_HEIGHT) {
                for (int w = 0; w < PIPE_WIDTH; ++w) {
                    draw_char_at(pipes[i].x + w, y, '|', 7);
                }
            }
        }
    }

    // Draw bird
    draw_char_at(4, (int)bird.y, '@', 2);  // Cast if using float bird.y

    move_cursor();
}

void play_game(void) {
    mafiaPrint("Press space to start");
    
    while (1)
    {
        handle_game_input();
        if (flap) break;  // La handle_game_input() sette `flap`
        sleep_interrupt(1000);
    }

    while (1) {
        reset_game();

        while (!game_over) {
            handle_game_input();
            update_game();
            draw_game();
            sleep_interrupt(FRAME_DELAY_MS);
        }

        mafiaPrint("\nGame Over! Final Score: %d\n", score);
        mafiaPrint("Press R to restart or ESC to return to menu...\n");

        while (1) {
            game_over = false; 
            handle_game_input(); // reads terminalBuffer
            if (game_over) break; // ESC eller R satt `game_over = true`
            sleep_interrupt(1000);
        }

    }
}

