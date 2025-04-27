#include "libc/stdio.h"
#include "libc/terminal.h"
#include "keyboard.h"
#include "libc/string.h"
#include "libc/stdbool.h"

void start_cli(void)
{
    terminal_clear();
    printf("Entering terminal mode. Type 'exit' to leave.\n");
    printf("> ");

    char buffer[256];
    int index = 0;
    while (1)
    {
        char c = getChar();

        if (c == '\n') // user pressed Enter
        {
            buffer[index] = '\0';
            if (strcmp(buffer, "exit") == 0)
            {
                printf("Exiting terminal mode...\n");
                break;
            }
            else
            {
                printf("\nYou typed: %s\n", buffer);
            }
            index = 0; // reset buffer for next line
            printf("> ");
        }
        else if (c == '\b') // user pressed Backspace
        {
            if (index > 0)
            {
                index--;
                terminal_put('\b');
                terminal_put(' ');
                terminal_put('\b');
            }
        }
        else
        {
            if ((long unsigned int)index < sizeof(buffer) - 1)
            {
                buffer[index++] = c;
                terminal_put(c); // Echo the character
            }
        }
    }
}