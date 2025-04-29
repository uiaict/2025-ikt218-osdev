#include <stdint.h>
#include <stdlib.h>
#include "common.h"

#define WIDTH 80
#define HEIGHT 25

typedef struct
{
    int y_pos;
    int speed;
} ColumnState;

static ColumnState columns[WIDTH];

static uint8_t color_palette[] = {0x02, 0x04, 0x01, 0x0E};

void init_matrix()
{
    for (int i = 0; i < WIDTH; i++)
    {
        columns[i].y_pos = rand() % HEIGHT;
        columns[i].speed = 1 + rand() % 3;
    }
}

void draw_matrix_frame()
{
    for (int x = 0; x < WIDTH; x++)
    {
        int y = columns[x].y_pos;

        char ch = 33 + rand() % 94;
        uint8_t color = color_palette[rand() % 4];

        if (y < HEIGHT)
        {
            volatile char *cell = (char *)0xB8000 + 2 * (y * WIDTH + x);
            cell[0] = ch;
            cell[1] = color;
        }

        if (y > 0 && (y - 1) < HEIGHT)
        {
            volatile char *cell = (char *)0xB8000 + 2 * ((y - 1) * WIDTH + x);
            cell[1] = 0x08;
        }

        if (y > 1 && (y - 2) < HEIGHT)
        {
            volatile char *cell = (char *)0xB8000 + 2 * ((y - 2) * WIDTH + x);
            cell[0] = ' ';
            cell[1] = 0x00;
        }

        columns[x].y_pos += columns[x].speed;

        if (columns[x].y_pos >= HEIGHT + 3)
        {
            columns[x].y_pos = 0;
            columns[x].speed = 1 + rand() % 3;
        }
    }
}
