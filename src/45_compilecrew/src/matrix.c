#include "libc/terminal.h"
#include "libc/system.h"
#include "libc/keyboard.h"


#define WIDTH 80
#define HEIGHT 25

static uint32_t rand_seed = 1;
static int rand_simple(void) {
    rand_seed = rand_seed * 1103515245 + 12345;
    return (rand_seed >> 16) & 0x7FFF;
}

typedef struct {
    int head_y;
    int length;
    int speed;
    int tick;
    int active;
} Column;

void draw_matrix_rain(void) {
    terminal_clear();
    disable_cursor();

    Column columns[WIDTH] = {0};

    for (int x = 0; x < WIDTH; x++) {
        columns[x].active = 0;
        columns[x].tick = rand_simple() % 10;
    }

    while (1) {
        if (get_last_key() == 27) {
            draw_front_page();
            return;
        }
        for (int x = 0; x < WIDTH; x++) {
            Column* col = &columns[x];

            if (!col->active) {
                if (rand_simple() % 10 == 0) {
                    col->active = 1;
                    col->head_y = 0;
                    col->length = 5 + rand_simple() % 16; // 5–20
                    col->speed = 1 + rand_simple() % 3;
                    col->tick = 0;
                }
                continue;
            }

            if (++col->tick >= col->speed) {
                col->tick = 0;

                int tail_y = col->head_y - col->length;

                // Clear tail
                if (tail_y >= 0 && tail_y < HEIGHT) {
                    terminal_putentryat(' ', 0x00, x, tail_y);
                }

                // Update trail
                for (int i = 1; i < col->length; i++) {
                    int y = col->head_y - i;
                    if (y >= 0 && y < HEIGHT) {
                        char c = 33 + rand_simple() % 94;
                        terminal_putentryat(c, 0x0A, x, y); // green
                    }
                }

                // Update head
                if (col->head_y >= 0 && col->head_y < HEIGHT) {
                    char c = 33 + rand_simple() % 94;
                    terminal_putentryat(c, 0x0F, x, col->head_y); // white
                }

                col->head_y++;

                if (col->head_y - col->length > HEIGHT) {
                    col->active = 0;
                }
            }
        }

        sleep_interrupt(30);
    }

    enable_cursor(14, 15);
}
