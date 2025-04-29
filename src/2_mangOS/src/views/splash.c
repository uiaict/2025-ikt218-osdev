#include "view.h"
#include "libc/stdio.h"
#include "libc/terminal.h"
#include "keyboard.h"
#include "pit.h"

static const char *mango_outline[] = {
    "                   .-''''''-.",
    "                 .'        '.",
    "               /            \\",
    "              /              \\",
    "             |                |",
    "            |                |",
    "            \\              /",
    "             \\            /",
    "              '.        .'",
    "                '------'"};
#define MANGO_ROWS (sizeof(mango_outline) / sizeof(mango_outline[0]))

void splash_screen(void)
{
    terminal_clear();
    // top‐left
    const int x0 = 20;
    const int y0 = 5;

    // Draw each row
    for (int i = 0; i < MANGO_ROWS; i++)
    {
        const char *row = mango_outline[i];
        int len = strlen(row);

        // left and right border columns
        int left = 0, right = len - 1;
        while (left < len && row[left] == ' ')
            left++;
        while (right >= 0 && row[right] == ' ')
            right--;

        for (int j = 0; j < len; j++)
        {
            char c = row[j];
            if (c != ' ')
            {
            }
            else if (j > left && j < right)
            {
                // Interior space
                terminal_putentryat(' ',
                                    vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_YELLOW),
                                    x0 + j, y0 + i);
            }
        }
    }

    const char *title = "       mangOS";
    int tx = x0 + (strlen(mango_outline[0]) - strlen(title)) / 2;
    int ty = y0 + MANGO_ROWS + 1;
    terminal_setcolor(VGA_COLOR_YELLOW);
    for (size_t k = 0; k < strlen(title); k++)
    {
        terminal_putentryat(title[k],
                            vga_entry_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK),
                            tx + k, ty);
    }
    move_cursor_to(0, 24);
}