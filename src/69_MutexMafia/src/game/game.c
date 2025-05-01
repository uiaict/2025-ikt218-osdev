#include "game.h"


Bird bird;
Pipe pipes[3];
int score = 0;
bool game_over = false;
bool exit_game = false;
bool flap = false;
extern int index; // Index for terminalBuffer
extern char terminalBuffer[]; // Buffer for keyboard input


void play_start_sound() {
    play_sound(523);  // C5
    sleep_interrupt(100);
    play_sound(659);  // E5
    sleep_interrupt(100);
    play_sound(784);  // G5
    sleep_interrupt(100);
    play_sound(988);  // B5
    sleep_interrupt(100);
    play_sound(1046); // C6 (high)
    sleep_interrupt(200);
    play_sound(784);  // G5
    sleep_interrupt(100);
    play_sound(659);  // E5
    sleep_interrupt(100);
    play_sound(523);  // C5
    sleep_interrupt(200);
    stop_sound();
}

void game_over_sound() {
    play_sound(659);  // E5
    sleep_interrupt(150);
    play_sound(523);  // C5
    sleep_interrupt(150);
    play_sound(392);  // G4
    sleep_interrupt(150);
    play_sound(349);  // F4
    sleep_interrupt(150);
    play_sound(330);  // E4
    sleep_interrupt(150);
    play_sound(261);  // C4
    sleep_interrupt(200);
    play_sound(261);  // G3 (low, final)
    sleep_interrupt(250);
    stop_sound();
}






HighscoreTable* highscores = NULL;

void init_highscores() {
    highscores = (HighscoreTable*) malloc(sizeof(HighscoreTable));
    if (highscores) {
        highscores->count = 0;
        for (int i = 0; i < MAX_HIGHSCORES; i++)
            highscores->scores[i] = 0;
    }
}

void insert_highscore(int new_score) {
    if (!highscores) return;

    // Insert if we have space or new score is better than the lowest
    if (highscores->count < MAX_HIGHSCORES || new_score > highscores->scores[highscores->count - 1]) {
        if (highscores->count < MAX_HIGHSCORES) {
            highscores->scores[highscores->count++] = new_score;
        } else {
            highscores->scores[MAX_HIGHSCORES - 1] = new_score;
        }

        // Sort descending
        for (int i = 0; i < highscores->count - 1; i++) {
            for (int j = i + 1; j < highscores->count; j++) {
                if (highscores->scores[j] > highscores->scores[i]) {
                    int tmp = highscores->scores[i];
                    highscores->scores[i] = highscores->scores[j];
                    highscores->scores[j] = tmp;
                }
            }
        }
    }
}

void print_highscores() {
    mafiaPrint("\n--- High Scores ---\n");
    for (int i = 0; i < highscores->count; i++) {
        mafiaPrint("%d. %d\n", i + 1, highscores->scores[i]);
    }
}

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
    } else if (key == 'x' || key == 'X') { // ESC
        exit_game = true;
    }
}


void update_game() {
    // Bird movement
    if (flap) {
        bird.velocity = FLAP_STRENGTH;        
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
                    draw_char_at(pipes[i].x + w, y, '|', 2);
                }
            }
        }
    }

    // Draw bird
    draw_char_at(4, (int)bird.y, '@', 14);  // Cast if using float bird.y

    move_cursor();
}

void play_game(void) {
    exit_game = false;
    mafiaPrint("Press space to start");
    
    while (1)
    {
        handle_game_input();
        if (flap) break;  // La handle_game_input() sette `flap`
        sleep_interrupt(100);
    }
    update_game();
    draw_game();
    while (1) {
        reset_game();
        play_start_sound();

        while (!game_over) {
    
            handle_game_input();
            if (exit_game) {

                mafiaPrint("Exiting game...\n");
                clear_screen();
                return; // Exit the game loop
            }
            update_game();
            draw_game();
            sleep_interrupt(FRAME_DELAY_MS);
        }

    
        mafiaPrint("\nGame Over! Final Score: %d\n", score);
        insert_highscore(score);
        mafiaPrint("Press R to restart or X to return to menu...\n");
        game_over_sound();

        while (1) {
            game_over = false; 
            handle_game_input(); // reads terminalBuffer
            if (exit_game) {
                mafiaPrint("Exiting game...\n");
                clear_screen();
                return; // Exit the game loop
            }
            if (game_over) break; // R satt `game_over = true`
            sleep_interrupt(100);
        }
        
    }
}

