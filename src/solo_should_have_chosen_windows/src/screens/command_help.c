#include "screens/screens.h"
#include "terminal/print.h"
#include "terminal/cursor.h"
#include "libc/stdint.h"
#include "libc/string.h"

static const char *title = "SHOULD HAVE WINDOWS COMMANDS";
static const char *esc_message = "Press ESC to return to the main menu";
static const char *lines[] = {
    "Shell commands being with shc-launch: shc-launch <command>:\n",
    "\tinfo\t: Displays information about the OS.",
    "\thelp\t: Displays command list.",
    "\tclear\t: Clears terminal.",
    "\tmusic\t: Launches the music player.",
    ""
};

void print_command_help(void) {
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
        printf("%s\n",lines[i]);
    }
 
    printf("\n%s", esc_message);
}