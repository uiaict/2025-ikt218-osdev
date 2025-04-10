#include "screens/screens.h"
#include "terminal/print.h"
#include "terminal/cursor.h"
#include "libc/stdint.h"
#include "libc/string.h"

static const char *title = "ASCII ART HELP";
static const char *esc_message = "Press ESC to return to the main menu";
static const char *lines[] = {
    "Commands for ascii art mode start with shc-art <command>:\n",
    "\thelp\t: Displays command list.",
    "\tclear\t: Clears terminal.",
    "\tnew <name>\t: Creates new drawing with name <name>.",
    "\tload <name>\t: Loads drawing with name <name>.",
    "\texit\t: Exits ascii art mode.\n\n",
    "When art is loaded, you can draw with all keys.\nSaving is automatic. Press ESC to save and exit.\n",
    ""
};

void print_art_help(void) {
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