#include "game/wordgame.h"
#include "libc/scrn.h"
//#include "memory/memory.h"
#include "libc/stdbool.h"
#include "audio/speaker.h"
#include "pit/pit.h"
#include "audio/tracks.h"
#include "audio/player.h"


#define MAX_WORDS 50
#define MAX_WORD_LENGTH 32
#define MAX_NAME_LENGTH 16
#define MAX_HIGHSCORES 10

// Struktur for highscore, lagrer navn, highscore og hvor lang tid det tok
typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
    int duration_ms;
} HighscoreEntry;

HighscoreEntry highscores[MAX_HIGHSCORES];
int highscore_count = 0;

char word_bank[MAX_WORDS][MAX_WORD_LENGTH];
int word_count = 0;
int highscore = 0;

// Hovedmenyen, 3 forskjellige valg, start spill, se highscore og avslutte
void start_game_menu() {
    while (1) {
        printf("\n==== Word Game Menu ====\n");
        printf("1: Start game\n");
        printf("2: Show highscores\n");
        printf("q: Quit game\n");
        printf("Your choice: ");

        char choice[4];
        get_input(choice, sizeof(choice));

        if (choice[0] == '1') {
            start_word_game();
        } else if (choice[0] == '2') {
            show_highscores();
        } else if (choice[0] == 'q' || choice[0] == 'Q') {
            printf("Exiting game...\n");
            return;
        } else {
            printf("Invalid input. Try again.\n");
        }
    }
}
// Stokker bokstavene i ordet for å lage utfordringen
// Bruker her Fisher-Yates-algoritmen for å stokke bokstavene, finn kilder!!
// Bruker tick-teller som "random seed"
void shuffle_word(const char* word, char* shuffled) {
    int len = 0;
    while (word[len] != '\0') len++;

    // Kopier originalordet til shuffled
    for (int i = 0; i < len; i++) {
        shuffled[i] = word[i];
    }
    shuffled[len] = '\0';

    // Bruk en enkel PRNG basert på tick som "seed"
    uint32_t seed = pit_get_tick();
    uint32_t rand = seed;

    // Fisher-Yates shuffle - stokker rekkefølgen på bokstavene
    for (int i = len - 1; i > 0; i--) {
        // Enkel pseudo-random (lineær kongruensgenerator)
        rand = (rand * 1103515245 + 12345) & 0x7fffffff; // enkel PRNG
        int j = rand % (i + 1);

        // Bytt shuffled[i] og shuffled[j] - altså bytter to bokstaver
        char tmp = shuffled[i];
        shuffled[i] = shuffled[j];
        shuffled[j] = tmp;
    }
}

// Kjører en runde der spilleren gjetter riktig ord
int play_round(const char* original_word) {
    char shuffled[MAX_WORD_LENGTH];
    shuffle_word(original_word, shuffled);

    printf("Guess the word: %s\n(Type 'exit' to quit)\n", shuffled);

    for (int attempt = 1; attempt <= 3; attempt++) {
        char guess[MAX_WORD_LENGTH];
        get_input(guess, MAX_WORD_LENGTH);

        // Tillater å avslutte spillet underveis
        if (strcmp(guess, "exit") == 0) {
            return -1;
        }

        // Sjekker om brukeren gjettet riktig
        if (strcmp(guess, original_word) == 0) {
            printf("Correct!\n\n");
            highscore++; // Øker poengsummen
            return 1;
        } else {
            printf("Wrong! Attempt %d/3\n", attempt);
        }
    }

    printf("You lost. The correct word was: %s\n\n", original_word);
    return 0; // Spilleren tapte runden
}

// Fuller word_bank med forhåndsdefinerte ord
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

// Sorterer highscore etter poeng, deretter tid brukt (lavest tid først)
void sort_highscores() {
    for (int i = 0; i < highscore_count - 1; i++) {
        for (int j = 0; j < highscore_count - i - 1; j++) {
            bool swap = false;

            if (highscores[j].score < highscores[j + 1].score) {
                swap = true;
            } else if (highscores[j].score == highscores[j + 1].score &&
                       highscores[j].duration_ms > highscores[j + 1].duration_ms) {
                swap = true;
            }

            if (swap) {
                HighscoreEntry temp = highscores[j];
                highscores[j] = highscores[j + 1];
                highscores[j + 1] = temp;
            }
        }
    }
}

// Skriver ut hele highscore-listen i sortert rekkefølge
void show_highscores() {
    printf("\n=== Highscore List ===\n");

    if (highscore_count == 0) {
        printf("No highscores yet.\n");
        return;
    }

    sort_highscores();

    for (int i = 0; i < highscore_count; i++) {
        printf("%d. %s - %d points in %d.", i + 1,
               highscores[i].name,
               highscores[i].score,
               highscores[i].duration_ms / 1000);

        int ms = highscores[i].duration_ms % 1000;
        if (ms < 100) printf("0");
        if (ms < 10) printf("0");
        printf("%d sec\n", ms);
    }
}


// Starter nytt spill og tar imot spillerens navn
void start_word_game() {
    char player_name[MAX_NAME_LENGTH];
    printf("Enter your name: ");
    get_input(player_name, MAX_NAME_LENGTH);

    printf("Welcome, %s!\n", player_name);
    printf("Type 'exit' at any time to quit.\n");

    collect_words();
    highscore = 0;

    play_music(start_melody, sizeof(start_melody) / sizeof(Note));

    printf("\nStarting the game...\n\n");

    uint32_t start_time = pit_get_tick(); // Start tidtaking

    for (int i = 0; i < word_count; i++) {
        int result = play_round(word_bank[i]);

        if (result == -1) {
            printf("Game aborted.\n");
            play_music(victory_melody, sizeof(victory_melody) / sizeof(Note));

            break;
        }

        if (result == 0) {
            printf("Game over!\n");
            play_music(failure_melody, sizeof(failure_melody) / sizeof(Note));

            break;
        }
    }

    uint32_t end_time = pit_get_tick(); // Slutttid
    uint32_t total_ms = end_time - start_time;

    if (highscore == word_count) {
        play_music(victory_melody, sizeof(victory_melody) / sizeof(Note));
    }
// Spillerens poengsum og tid vises
    printf("\n=== Game Summary ===\n");
    printf("Score: %d/%d\n", highscore, word_count);
    printf("Time: %d.", total_ms / 1000);
    int ms = total_ms % 1000;
    if (ms < 100) printf("0");
    if (ms < 10) printf("0");
    printf("%d seconds\n", ms);

    // Lagrer resultat til hichscore-listen hvis det er plass
    if (highscore_count < MAX_HIGHSCORES) {
        strcpy(highscores[highscore_count].name, player_name);
        highscores[highscore_count].score = highscore;
        highscores[highscore_count].duration_ms = total_ms;
        highscore_count++;
    }
    sort_highscores(); // Sorter før visning

    printf("\n=== Highscore List ===\n");
    if (highscore_count == 0) {
        printf("No highscores yet.\n");
    } else {
        for (int i = 0; i < highscore_count; i++) {
            printf("%d. %s - %d points in %d.", i + 1,
                   highscores[i].name,
                   highscores[i].score,
                   highscores[i].duration_ms / 1000);

            int ms = highscores[i].duration_ms % 1000;
            if (ms < 100) printf("0");
            if (ms < 10) printf("0");
            printf("%d sec\n", ms);
        }
    }
}
