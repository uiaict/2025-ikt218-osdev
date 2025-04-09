#include "about_screen/about_screen.h"
#include "terminal/print.h"
#include "terminal/cursor.h"
#include "libc/stdint.h"
#include "libc/string.h"

static const char *title = "About Should Have Windows";
static const char *esc_message = "Press ESC to return to the main menu";
static const char *lines[] = {
    "This OS was developed as part of a university",
    "project to understand low-level systems and",
    "how operating systems are built from scratch.",
    "",
    "Features include:",
    "- Custom interrupt handlers",
    "- Multistate UI with menu & music player",
    "- Keyboard input with Norwegian QWERTY support",
    ""
};

void print_about_screen(void) {
    clearTerminal();

    const int width = 50;
    const int padding = 2;

    printf("+");
    for (int i = 0; i < width -2; i++) {
        printf("-");
    }
    printf("+\n");

   printf("|");
    int title_pad = (width - 2 - strlen(title)) / 2;
    for (int i = 0; i < title_pad; i++) {
        printf(" ");
    }
    printf("%s", title);
    for (int i = 0; i < width - 2 - title_pad - (int) strlen(title); i++) {
        printf(" ");
    }
    printf("|\n");

    printf("+");
    for (int i = 0; i < width -2; i++) {
        printf("-");
    }
    printf("+\n");
 
    for (int i = 0; i < (int) (sizeof(lines) / sizeof(lines[0])); i++) {
        printf("|");
        printf("%s",lines[i]);
        int space = width - 2 - (int)strlen(lines[i]);
        for (int j = 0;  j < space; j++) {
            printf(" ");
        } 
        printf("|\n");
    }
 
    printf("+");
    for (int i = 0; i < width -2; i++) {
        printf("-");
    }
    printf("+\n");

    printf("\n%s", esc_message);
}