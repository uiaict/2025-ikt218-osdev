#include "game/wordgame.h"
#include "game/input_buffer.h"
#include "libc/scrn.h"
#include "memory/memory.h"
#include "libc/stdbool.h"
#include "audio/speaker.h"
#include "pit/pit.h"

#define MAX_WORDS 50
#define MAX_WORD_LENGTH 32

char word_bank[MAX_WORDS][MAX_WORD_LENGTH];
int word_count = 0;
int highscore = 0;

void start_game_menu() {
    while (1) {
        printf("==== Word Game Menu ====\n");
        printf("1: Start game\n");
        printf("q: Quit game\n");
        printf("Your choice: ");

        char choice[4];
        get_input(choice, sizeof(choice));

        if (choice[0] == '1') {
            start_word_game();
        } else if (choice[0] == 'q' || choice[0] == 'Q') {
            printf("Exiting game...\n");
            return;
        } else {
            printf("Invalid input. Try again.\n");
        }
    }
}

void shuffle_word(const char* word, char* shuffled) {
    int len = 0;
    while (word[len] != '\0') len++;

    for (int i = 0; i < len; i++) {
        shuffled[i] = word[i];
    }
    shuffled[len] = '\0';

    for (int i = 0; i < len * 2; i++) {
        int a = pit_get_tick() % len;
        int b = (pit_get_tick() + i) % len;
        char tmp = shuffled[a];
        shuffled[a] = shuffled[b];
        shuffled[b] = tmp;
    }
}

int play_round(const char* original_word) {
    char shuffled[MAX_WORD_LENGTH];
    shuffle_word(original_word, shuffled);

    printf("Guess the word: %s, or type 'exit' to end the game\n", shuffled);

    for (int attempt = 1; attempt <= 3; attempt++) {
        char guess[MAX_WORD_LENGTH];
        get_input(guess, MAX_WORD_LENGTH);

        if (strcmp(guess, "exit") == 0) {
            return -1;
        }

        if (strcmp(guess, original_word) == 0) {
            printf("Correct!\n\n");
            highscore++;
            return 1;
        } else {
            printf("Wrong! Attempt %d/3\n", attempt);
        }
    }

    printf("You lost. The correct word was: %s\n\n", original_word);
    return 0;
}

void collect_words() {
    strcpy(word_bank[0], "operatingsystem");
    strcpy(word_bank[1], "coding");
    strcpy(word_bank[2], "programming");
    strcpy(word_bank[3], "data");
    strcpy(word_bank[4], "linux");
    strcpy(word_bank[5], "powershell");
    strcpy(word_bank[6], "database");
    strcpy(word_bank[7], "computer");
    strcpy(word_bank[8], "kernel");
    strcpy(word_bank[9], "memory");
    strcpy(word_bank[10], "hardware");
    strcpy(word_bank[11], "software");
    strcpy(word_bank[12], "network");
    strcpy(word_bank[13], "algorithm");
    strcpy(word_bank[14], "function");
    strcpy(word_bank[15], "variable");
    strcpy(word_bank[16], "pointer");
    strcpy(word_bank[17], "compiler");
    strcpy(word_bank[18], "debugging");
    strcpy(word_bank[19], "scripting");
    strcpy(word_bank[20], "ubuntu");
    strcpy(word_bank[21], "windows");
    strcpy(word_bank[22], "macos");
    strcpy(word_bank[23], "shell");
    strcpy(word_bank[24], "bash");
    strcpy(word_bank[25], "python");
    strcpy(word_bank[26], "java");
    strcpy(word_bank[27], "javascript");
    strcpy(word_bank[28], "html");
    strcpy(word_bank[29], "css");
    strcpy(word_bank[30], "typescript");
    strcpy(word_bank[31], "sql");
    strcpy(word_bank[32], "json");
    strcpy(word_bank[33], "xml");
    strcpy(word_bank[34], "api");
    strcpy(word_bank[35], "http");
    strcpy(word_bank[36], "https");
    strcpy(word_bank[37], "tcp");
    strcpy(word_bank[38], "udp");
    strcpy(word_bank[39], "ftp");
    strcpy(word_bank[40], "ssh");
    strcpy(word_bank[41], "git");
    strcpy(word_bank[42], "github");
    strcpy(word_bank[43], "gitlab");
    strcpy(word_bank[44], "bitbucket");
    strcpy(word_bank[45], "docker");

    word_count = 46;
}

void start_word_game() {
    printf("Welcome to the word game!\n");
    printf("Type 'exit' at any time to quit.\n");

    collect_words();
    highscore = 0;

    play_start_melody();
    printf("\nStarting the game...\n\n");

    for (int i = 0; i < word_count; i++) {
        int result = play_round(word_bank[i]);

        if (result == -1) {
            printf("Game aborted by user.\n");
            play_failure_melody();
            break;
        }

        if (result == 0) {
            printf("Game over!\n");
            play_failure_melody();
            break;
        }
    }

    if (highscore == word_count) {
        play_victory_melody();
    }

    printf("\n=== Game Summary ===\n");
    printf("Score: %d out of %d words\n", highscore, word_count);
}
