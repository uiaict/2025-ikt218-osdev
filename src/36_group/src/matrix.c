#include "matrix.h"
#include "io.h"
#include <libc/stdint.h>
#include <libc/stdbool.h>

#define COLS 80
#define ROWS 25
#define MAX_TAIL 8

static const uint8_t text_colors[] = {0x0A, 0x09, 0x0C, 0x0E};
static const uint8_t bg_colors[] = {0x00, 0x10, 0x20, 0x40};
static int text_color_index = 0;
static int bg_color_index = 0;

static uint32_t lfsr = 0x12345678;
static uint32_t rnd(void)
{
    lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xB400u);
    return lfsr;
}

void matrix(void)
{
    __asm__ volatile("cli");
    clear_screen();
    volatile uint16_t *vga = (volatile uint16_t *)0xB8000;

    int head_row[COLS], tail_len[COLS], speed_div[COLS];
    for (int c = 0; c < COLS; c++)
    {
        head_row[c] = rnd() % ROWS;
        tail_len[c] = 3 + (rnd() % (MAX_TAIL - 2));
        speed_div[c] = 1 + (rnd() % 3);
    }

    uint8_t agebuf[ROWS][COLS] = {{0}};
    uint32_t frame = 0;

    const char *prompt = "Backspace=Exit  1=TextColor  2=BgColor                                      ";
    int prompt_len = 40;
    int prompt_row = 0;
    int prompt_col = 0;

    while (1)
    {
        uint8_t ATTR_HEAD = text_colors[text_color_index];
        uint8_t ATTR_BG = bg_colors[bg_color_index];
        uint8_t ATTR_TAIL = (ATTR_BG & 0xF0) | (ATTR_HEAD & 0x0F);

        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLS; c++)
            {
                char ch = ' ';
                uint8_t attr = ATTR_TAIL;
                if (agebuf[r][c])
                {
                    agebuf[r][c]--;
                    ch = (rnd() & 1) ? 'X' : '0';
                    attr = (agebuf[r][c] == tail_len[c]) ? ((ATTR_BG & 0xF0) | 0x0F) : ATTR_TAIL;
                }

                // Prompt overlay
                if (r == prompt_row && c >= prompt_col && c < prompt_col + prompt_len)
                {
                    ch = prompt[c - prompt_col];
                    attr = ATTR_TAIL;
                }

                vga[r * COLS + c] = (attr << 8) | (uint8_t)ch;
            }
        }

        for (int c = 0; c < COLS; c++)
        {
            if ((frame % speed_div[c]) == 0)
            {
                int r = head_row[c];
                agebuf[r][c] = tail_len[c];
                head_row[c] = (r + 1) % ROWS;
                if (head_row[c] == 0)
                {
                    tail_len[c] = 3 + (rnd() % (MAX_TAIL - 2));
                    speed_div[c] = 1 + (rnd() % 3);
                }
            }
        }

        if (inb(0x64) & 1)
        {
            uint8_t sc = inb(0x60);
            if (!(sc & 0x80))
            {
                switch (sc)
                {
                case 0x0E: // Backspace
                    clear_screen();
                    __asm__ volatile("sti");
                    return;
                case 0x02: // 1
                    text_color_index = (text_color_index + 1) % (sizeof(text_colors) / sizeof(text_colors[0]));
                    break;
                case 0x03: // 2
                    bg_color_index = (bg_color_index + 1) % (sizeof(bg_colors) / sizeof(bg_colors[0]));
                    break;
                default:
                    break;
                }
            }
        }

        for (volatile uint32_t d = 0; d < 2000000; d++)
            __asm__ volatile("nop");

        frame++;
    }
}
