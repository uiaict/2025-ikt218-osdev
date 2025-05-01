#include "adventure.h"
#include "terminal.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "keyboard.h"

void start_adventure() {
    terminal_printf("\n=== [TEXT ADVENTURE GAME] ===\n");
    terminal_printf("You wake up in a dark forest. There's a path to the LEFT and one to the RIGHT.\n");
    terminal_printf("Which direction do you go? [l/r]: ");

    char choice = keyboard_read_char(); // kendi tanımlı klavye input'un
    terminal_printf("%c\n", choice);

    if (choice == 'l') {
        terminal_printf("You went LEFT and encountered a wolf!\n");
        terminal_printf("Do you RUN or FIGHT? [r/f]: ");
        char action = keyboard_read_char();
        terminal_printf("%c\n", action);

        if (action == 'r') {
            terminal_printf("You run fast and escape! 🏃‍♂️\n");
        } else if (action == 'f') {
            terminal_printf("You bravely fight and defeat the wolf! 🐺\n");
        } else {
            terminal_printf("Invalid action. The wolf eats you. 💀\n");
        }

    } else if (choice == 'r') {
        terminal_printf("You went RIGHT and found a river.\n");
        terminal_printf("Do you SWIM across or BUILD a raft? [s/b]: ");
        char action = keyboard_read_char();
        terminal_printf("%c\n", action);

        if (action == 's') {
            terminal_printf("The river is too strong. You get swept away. 🌊💀\n");
        } else if (action == 'b') {
            terminal_printf("Smart move! You cross safely. 🛶\n");
        } else {
            terminal_printf("You hesitate too long and get eaten by mosquitoes. 🦟💀\n");
        }

    } else {
        terminal_printf("You stand still and do nothing. The forest swallows you. 🌲💀\n");
    }

    terminal_printf("=== [END OF GAME] ===\n");
}
