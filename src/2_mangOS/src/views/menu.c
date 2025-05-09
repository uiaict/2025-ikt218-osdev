#include "view.h"
#include "libc/stdio.h"
#include "libc/terminal.h"
#include "keyboard.h"
#include "pit.h"

int take_input()
{
    char input[2];
    int choice = 0;
    input[0] = getChar();
    input[1] = '\0';
    if (input[0] >= '1' && input[0] <= '5')
    {
        choice = input[0] - '0';
    }
    else
    {
        printf("Invalid choice. Please try again.\n");
        printf("       > ");
        return take_input();
    }
    return choice;
}

int menu()
{
    terminal_clear();
    terminal_setcolor(VGA_COLOR_YELLOW);

    // print title
    printf("\n\n\n\n\n\n\n\n");
    printf("           mangOS\n\n");

    // menu options
    terminal_setcolor(VGA_COLOR_LIGHT_GREY);
    printf("       1. Song App\n");
    printf("       2. Memory Overview\n");
    printf("       3. Terminal\n");
    printf("       4. Snake Game\n");
    printf("       5. Exit\n\n");

    // prompt
    terminal_setcolor(VGA_COLOR_YELLOW);
    printf("       > ");
    int choice = take_input();

    // separator
    terminal_setcolor(VGA_COLOR_DARK_GREY);
    printf("\n       ----------------------------\n\n");
    return choice;
}