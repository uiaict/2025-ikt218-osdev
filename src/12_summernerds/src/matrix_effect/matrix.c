#include "matrix_effect/matrix.h"
#include <libc/stdint.h>
#include "common.h"
#include "random.h"
#include "i386/monitor.h"

#define WIDTH 80
#define HEIGHT 25

typedef struct
{
    int y_pos;
    int speed;
    int color;
} ColumnState;

static ColumnState columns[WIDTH];

static uint8_t color_palette[] = {0x02, 0x04, 0x01, 0x0E};

void init_matrix()
{
    setupRNG(947);
    for (int i = 0; i < WIDTH; i++)
    {
        columns[i].y_pos = randint(100) % HEIGHT;
        columns[i].speed = 1 + randint(100) % 3;
        columns[i].color = color_palette[randint(1000) % 4];
    }
}

void draw_matrix_frame()
// The function that make the matrix effect
//(draws one frame for the matrix effect for each raining character)
{
    monitor_clear();
    for (int x = 0; x < WIDTH; x++)
    {
        int y_max = columns[x].y_pos;

        for (int y = 0; y < y_max; y++)
        {
            char ch = 33 + randint(94);
            if (y < HEIGHT)
            {
                volatile char *cell = (char *)0xB8000 + 2 * (y * WIDTH + x);
                cell[0] = ch;
                cell[1] = columns[x].color;
            }
        }

        columns[x].y_pos += columns[x].speed;

        if (columns[x].y_pos >= HEIGHT + 3)
        {
            columns[x].y_pos = 0;
            columns[x].speed = 1 + randint(3);
            columns[x].color = color_palette[randint(1000) % 4];
        }
    }
}
