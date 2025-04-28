#include "libc/stdio.h"
#include "libc/terminal.h"
#include "keyboard.h"
#include "libc/string.h"
#include "libc/stdbool.h"

#define CLI_BUF_SIZE 128

// Trim leading/trailing whitespace in place
static void trim(char *s)
{
    // Trim front
    char *p = s;
    while (*p == ' ' || *p == '\t')
        p++;
    if (p != s)
        memmove(s, p, strlen(p) + 1);

    // Trim back
    int len = strlen(s);
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t' || s[len - 1] == '\r'))
    {
        s[--len] = '\0';
    }
}

// Read a line into buf (including handling backspace), returns when Enter pressed
static void read_line(char *buf, int max)
{
    int idx = 0;
    while (1)
    {
        char c = getChar();

        if (c == '\r' || c == '\n')
        {
            terminal_put('\n');
            buf[idx] = '\0';
            return;
        }
        else if (c == '\b')
        {
            if (idx > 0)
            {
                idx--;
                terminal_put('\b');
                terminal_put(' ');
                terminal_put('\b');
            }
        }
        else if (c >= 32 && idx < max - 1)
        {
            buf[idx++] = c;
            terminal_put(c);
        }
        // ignore other control chars
    }
}

void start_cli(void)
{
    char line[CLI_BUF_SIZE];

    terminal_setcolor(VGA_COLOR_YELLOW);
    terminal_clear();
    printf("Entering terminal mode. Type 'help' for commands, 'exit' to leave.\n");

    while (1)
    {
        printf("mangOS> ");
        read_line(line, CLI_BUF_SIZE);
        trim(line);
        if (line[0] == '\0')
            continue; // empty

        // Split into command + args
        char *cmd = strtok(line, " ");
        char *args = strtok(NULL, "");

        // Dispatch
        if (strcmp(cmd, "exit") == 0)
        {
            printf("Exiting terminal mode...\n");
            break;
        }
        else if (strcmp(cmd, "help") == 0)
        {
            printf("Available commands:\n");
            printf("  help        - show this message\n");
            printf("  clear       - clear the screen\n");
            printf("  echo <text> - echo text\n");
            printf("  exit        - exit CLI\n");
            // add your own here: snake, play, meminfo, etc.
        }
        else if (strcmp(cmd, "clear") == 0)
        {
            terminal_clear();
        }
        else if (strcmp(cmd, "echo") == 0)
        {
            if (args)
                printf("%s\n", args);
            else
                printf("\n");
        }
        else
        {
            printf("Unknown command: '%s'  (type 'help')\n", cmd);
        }
    }
}
