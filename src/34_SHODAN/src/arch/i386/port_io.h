
#ifndef PORT_IO_H
#define PORT_IO_H

#include <stdint.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

void play_sound(uint32_t frequency);
void stop_sound(void);

#endif // PORT_IO_H
