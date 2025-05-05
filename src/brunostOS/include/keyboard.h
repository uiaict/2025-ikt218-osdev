#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/stdint.h"
#include "isr.h"
#include "libc/stdbool.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_COMMAND_PORT 0x64

#define LSHIFT_CODE 0x2A
#define RSHIFT_CODE 0x36
#define CAPSLOCK_CODE 0x3A
#define ALTGR_CODE 0x38   // Sends 0xE0 first

// relevant CP437 extended ASCII
// OS uses CP437, but something else (compiler?) uses othert encodeing for extended ASCII
// https://www.ascii-codes.com/
#define å 134
#define Å 143
#define æ 145
#define Æ 146
#define ø 236   // Infinity
#define Ø 237   // Greek lower case phi
#define µ 230 
#define GBP 156 // £
#define ´ 0     // might implement ó, porbably not
#define € 155   // No € support, replaced with ¢
#define ¨ 0     // Might implement ö and ô, porbably not
#define ORB 0   // ¤
#define PGRPH 0 // §

static bool shift = false;
static bool capslock = false;
static bool altgr = false;

static bool us_keyboard_layout = false;

static const uint8_t ASCII_us[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0
};
static const uint8_t ASCII[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '\\', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', å, ¨, '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ø, æ, '|', 0, '\'', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '-', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', ',', 0, 0, '<'
    // enter should be \r\n
};
static const uint8_t ASCII_shift[] = {
    0, 0, '!', '\"', '#', ORB, '%', '&', '/', '(', ')', '=', '?', '`', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', Å, '^', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', Ø, Æ, PGRPH, 0, '*', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ';', ':', '_', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', ',', 0, 0, '>'
    // enter is only \n and not \r also
};
static const uint8_t ASCII_caps[] = {
    // can't be typed: ¨
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '\\', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'T', 'T', 'I', 'O', 'P', Å, ¨, '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', Ø, Æ, '|', 0, '\'', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '-', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', ',', 0, 0, '<'
    // enter should be \r\n
};
static const uint8_t ASCII_caps_shift[] = {
    0, 0, '!', '\"', '#', ORB, '%', '&', '/', '(', ')', '=', '?', '`', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', å, '^', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ø, æ, PGRPH, 0, '*', 'z', 'x', 'c', 'v',
    'B', 'N', 'M', ';', ':', '_', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', ',', 0, 0, '>'
    // enter is only \n and not \r also
};
static const uint8_t ASCII_altgr[] = {
    0, 0, 0, '@', GBP, '$', 0, 0, '{', '[', ']', '}', 0, ´, 0, 0,
    0, 0, €, 0, 0, 0, 0, 0, 0, 0, 0, '~', ' ', 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, µ, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 
    // enter should be \r\n
};


void init_keyboard();
void keyboard_handler(struct registers);


#endif // KEYBOARD_H