#pragma once

#include "libc/stdint.h"
#include "libc/stddef.h"

#define BOOT_ART_LINES 5
static const char *boot_art[BOOT_ART_LINES] = {
    "  _   _      _ _         __        __         _     _ ",
    " | | | | ___| | | ___    \\ \\      / /__  _ __| | __| |",
    " | |_| |/ _ \\ | |/ _ \\    \\ \\ /\\ / / _ \\| '__| |/ _` |",
    " |  _  |  __/ | | (_) |    \\ V  V / (_) | |  | | (_| |",
    " |_| |_|\\___|_|_|\\___/      \\_/\\_/ \\___/|_|  |_|\\__,_|"};

static inline void print_boot_art(void)
{
    for (int i = 0; i < BOOT_ART_LINES; i++)
        printf("%s\n", boot_art[i]);
}
