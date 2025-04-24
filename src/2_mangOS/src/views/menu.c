#include "view.h"
#include "libc/stdio.h"
#include "libc/terminal.h"
#include "keyboard.h"

int take_input()
{
    char input[2];
    int choice = 0;
    input[0] = getChar();
    input[1] = '\0';
    if (input[0] >= '1' && input[0] <= '4')
    {
        choice = input[0] - '0';
    }
    else
    {
        printf("Invalid choice. Please try again.\n");
        return take_input();
    }
    return choice;
}

int menu()
{
    terminal_clear();
    terminal_setcolor(VGA_COLOR_YELLOW);
    const char *mango_logo[] = {
        "             .-''''''-.",
        "           .'        '.",
        "          /            \\",
        "         /              \\",
        "        |                |",
        "        |                |",
        "         \\              /",
        "          \\            /",
        "           '.        .'",
        "             '------'",
        "                                            ,-----.   ,---.   ",
        "      ,--,--,--.  ,--,--. ,--,--,   ,---.  '  .-.  ' '   .-'  ",
        "      |        | ' ,-.  | |      \ | .-. | |  |  |  | `.  `-.  ",
        "      |  |  |  | \ '-'  | |  ||  | ' '-' ' '  '-'  ' .-'    | ",
        "      `--`--`--'  `--`--' `--''--' .`-  /   `-----'  `-----'  ",
        "                                    `---'                      "};

    for (size_t i = 0; i < sizeof(mango_logo) / sizeof(mango_logo[0]); i++)
    {
        printf("%s", mango_logo[i]);
        terminal_put('\n');
    }
    printf("Welcome to the MangOS menu!\n");
    printf("1. Song App\n");
    printf("2. Memory Overview\n");
    printf("4. Exit\n");
    printf("> ");
    int choice = take_input();
    printf("------------------------------------------\n");
    return choice;
}