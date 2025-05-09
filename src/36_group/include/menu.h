#ifndef MENU_H
#define MENU_H

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"

#include "kernel_memory.h"
#include "pit.h"
#include "paging.h"
#include "interrupt.h"
#include "io.h"
#include "song.h"
#include "keyboard.h"

typedef enum
{
    MODE_NONE,
    MODE_MUSIC_PLAYER,
    MODE_TEST,
    MODE_MATRIX,
    MODE_MUSIC_MENU
} KernelMode;

int kernel_main(void);
void run_isr_tests(void);

#endif