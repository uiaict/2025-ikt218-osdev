#pragma once

#include <libc/stdint.h>
#include <libc/stdbool.h>

extern bool menu_active;
extern int selected_option;

void display_menu(void);
void select_menu_option(uint8_t option);

void handle_menu_input(char ascii_char);
void highlight_selected_option(uint8_t option);