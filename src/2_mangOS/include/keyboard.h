#pragma once

void init_keyboard();
char getChar(void);
char peekChar(void);
void clearBuffer(void);

static volatile uint16_t kbd_head;
static volatile uint16_t kbd_tail;
