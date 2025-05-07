/* keyboard.c  –  PS/2 Set‑1 driver   */
/* Uses C99 designated initialisers to avoid alignment mistakes   */

#include "port_io.h"
#include "terminal.h"
#include <stdint.h>

/* -----------------------------------------------------------------
   ASCII map for Nordic QWERTY (plain 8‑bit ASCII placeholders).
   Any index you do **not** list is implicitly 0.
   ----------------------------------------------------------------- */
static const char scancode_table[128] = {
    /*   --- 0x00 block ------------------------------------------------ */
    [0x01] = 27,            /* Esc              */
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
    [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8',
    [0x0A] = '9', [0x0B] = '0', [0x0C] = '+', [0x0D] = '\'',
    [0x0E] = '\b',          /* Backspace        */
    [0x0F] = '\t',          /* Tab              */

    /*   --- 0x10 block ------------------------------------------------ */
    [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r',
    [0x14] = 't', [0x15] = 'y', [0x16] = 'u', [0x17] = 'i',
    [0x18] = 'o', [0x19] = 'p', [0x1A] = 'a',     /* å→a placeholder */
    [0x1B] = '^',           /* ¨ dead key → ^   */
    [0x1C] = '\n',          /* Enter (main)     */
    /* 0x1D = Left Ctrl  → 0 (non‑printable)     */

    /*   --- 0x20 block ------------------------------------------------ */
    [0x1E] = 'a', [0x1F] = 's', [0x20] = 'd', [0x21] = 'f',
    [0x22] = 'g', [0x23] = 'h', [0x24] = 'j', [0x25] = 'k',
    [0x26] = 'l', [0x27] = 'o',     /* ø→o placeholder               */
    [0x28] = 'z',     /* æ→z placeholder */
    [0x29] = '`',     /* §/½ key → back‑tick placeholder          */

    /*   --- 0x30 block ------------------------------------------------ */
    [0x2B] = '\\',            /* Nordic <LSGT> is back‑slash here     */
    [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v',
    [0x30] = 'b', [0x31] = 'n', [0x32] = 'm',
    [0x33] = ',', [0x34] = '.', [0x35] = '-',      /* minus / underscore */
    /* 0x36 = Right Shift → 0                         */
    [0x37] = '*',              /* keypad * (multiply)                 */
    /* 0x38 = Left Alt      → 0                         */
    [0x39] = ' ',              /* **space‑bar**                       */
    /* 0x3A = Caps‑Lock     → 0                         */
    /* the rest stay 0 for now (function & cursor keys) */
};

/* ----------------------------------------------------------------- */
void keyboard_handler(void)
{
    static uint8_t extended = 0;
    uint8_t sc = inb(0x60);

    /* detect 0xE0 prefix for extended keys */
    if (sc == 0xE0) { extended = 1; return; }

    /* ignore key‑up events */
    if (sc & 0x80) { extended = 0; return; }

    /* skip extended keys for now */
    if (extended) { extended = 0; return; }

    /* translate printable keys */
    if (sc < sizeof scancode_table) {
        char ch = scancode_table[sc];
        if (ch) {
            char buf[2] = { ch, 0 };
            terminal_write(buf);
        }
    }
}
