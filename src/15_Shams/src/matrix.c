#include <libc/terminal.h>
#include <libc/pit.h>
#include <libc/stdint.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// Hver kolonne har en "y"-posisjon for hvor regnet er
int rain_y[SCREEN_WIDTH];

// Random generator
uint32_t rand_seed = 123456789;

uint32_t rand()
{
    rand_seed = rand_seed * 1664525 + 1013904223;
    return rand_seed;
}

char random_char()
{
    // Returner en tilfeldig bokstav eller tall
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    return charset[rand() % (sizeof(charset) - 1)];
}

void matrix_rain_step()
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        if (rain_y[x] < SCREEN_HEIGHT)
        {
            // Flytt cursor
            uint16_t pos = rain_y[x] * 80 + x;

            // Tegn en tilfeldig bokstav
            volatile uint16_t* video_memory = (uint16_t*)0xB8000;
            video_memory[pos] = (0x0A << 8) | random_char(); // Grønn tekst (0x0A)

            rain_y[x]++;
        }
        else
        {
            // Start ny "regnstråle" tilfeldig
            if (rand() % 20 == 0) // Ikke for ofte
            {
                rain_y[x] = 0;
            }
        }
    }
}

// Kjør matrix regn i loop
void start_matrix_rain()
{
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        rain_y[i] = rand() % SCREEN_HEIGHT;
    }

    while (1)
    {
        matrix_rain_step();
        
        // Vent en liten stund (juster for hastighet)
        sleep_interrupt(50); // 50 ms
    }
}
