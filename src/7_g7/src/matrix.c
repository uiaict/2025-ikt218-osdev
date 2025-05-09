#include "pit.h"
#include "libc/stdint.h"
#include "matrix.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

int rain_y[SCREEN_WIDTH];

uint32_t rand_seed = 123456789;

uint32_t rand()
{
    rand_seed = rand_seed * 1664525 + 1013904223;
    return rand_seed;
}

char random_char()
{
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    return charset[rand() % (sizeof(charset) - 1)];
}

// دالة لرسم حرف في (x, y) باستخدام monitor driver
void monitor_put_char_at(int x, int y, char c, uint8_t color)
{
    uint16_t* video_memory = (uint16_t*)0xB8000;
    video_memory[y * SCREEN_WIDTH + x] = (color << 8) | c;
}

void matrix_rain_step()
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        if (rain_y[x] < SCREEN_HEIGHT)
        {
            monitor_put_char_at(x, rain_y[x], random_char(), 0x0A); // لون أخضر

            rain_y[x]++;
        }
        else
        {
            if (rand() % 20 == 0) // عودة عشوائية
            {
                rain_y[x] = 0;
            }
        }
    }
}

void start_matrix_rain()
{
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        rain_y[i] = rand() % SCREEN_HEIGHT;
    }

    while (1)
    {
        matrix_rain_step();
        sleep_interrupt(50); // تأخير 50 مللي ثانية
    }
}
