#ifndef KERNEL_MAIN_H
#define KERNEL_MAIN_H

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"

#include "kernel_memory.h"
#include "pit.h"
#include "paging.h"
#include "interrupt.h"
#include "io.h"
#include "song/song.h"
#include "keyboard.h"

// Define modes for the kernel loop
typedef enum {
    MODE_NONE,
    MODE_MUSIC_PLAYER,
    MODE_TEST,
    MODE_PIANO,
    MODE_MATRIX,
    MODE_MUSIC_MENU
} KernelMode;

int kernel_main(void);
void run_isr_tests(void); // Declaration

#endif