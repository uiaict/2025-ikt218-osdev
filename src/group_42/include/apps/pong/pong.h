#ifndef PONG_H
#define PONG_H

#include "libc/stdbool.h"

extern bool pong_active;

void pong_init();

void draw_pong();

#endif