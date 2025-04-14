#include "game/wordgame.h"
#include "game/input_buffer.h"
#include "libc/scrn.h"
#include "memory/memory.h"
#include "libc/stdbool.h"
#include "audio/speaker.h"
#include "pit/pit.h"

#define MAX_WORDS 20
#define MAX_WORD_LENGTH 32

char word_bank[MAX_WORDS][MAX_WORD_LENGTH];
int word_count = 0;

void shuffle_word(const char* word, char* shuffled) {
    int len = 0;
    while (word[len] != '\0') len++;

    for (int i = 0; i < len; i++) {
        shuffled[i] = word[i];
    }
    shuffled[len] = '\0';

    // Enkelt shuffle – bytt to tilfeldig valgte bokstaver mange ganger
    for (int i = 0; i < len * 2; i++) {
        int a = pit_get_tick() % len;
        int b = (pit_get_tick() + i) % len;
        char tmp = shuffled[a];
        shuffled[a] = shuffled[b];
        shuffled[b] = tmp;
    }
}

bool play_round(const char* original_word) {
    char shuffled[MAX_WORD_LENGTH];
    shuffle_word(original_word, shuffled);

    printf("Guess the word %s\n", shuffled);

    for (int attempt = 1; attempt <= 3; attempt++) {
        char guess[MAX_WORD_LENGTH];
        get_input(guess, MAX_WORD_LENGTH);

        if (strcmp(guess, original_word) == 0) {
            printf("Correct!\n\n");
            play_success_melody();
           
            return true;
        } else {
            printf("Wrong! Attempt nr %d/3\n", attempt);
            play_error_melody();

        }
    }

    printf("You lost, the correct word were: %s\n\n", original_word);
return false;
}

void collect_words() {
    // Legg til ord du ønsker å bruke i spillet
    strcpy(word_bank[0], "operatingsystem");
    strcpy(word_bank[1], "coding");
    strcpy(word_bank[2], "programming");
    strcpy(word_bank[3], "datageek");
    strcpy(word_bank[4], "linux");

    word_count = 5; // Husk å oppdatere dette hvis du endrer antall ord
}



void start_word_game() {
    printf("Welcome to the word game!\n");
    collect_words();

    printf("\nStarting the game\n\n");

    for (int i = 0; i < word_count; i++) {
        if (!play_round(word_bank[i])) {
            printf("Game over!\n");
            return;
        }
    }
    

    printf("Game ending...\n");
}