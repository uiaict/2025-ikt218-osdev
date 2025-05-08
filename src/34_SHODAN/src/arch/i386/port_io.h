// port_io.h — Interface for low-level I/O and sound functions
#ifndef PORT_IO_H
#define PORT_IO_H

#include <stdint.h>

// Sends a byte to the specified I/O port
void outb(uint16_t port, uint8_t value);

// Reads a byte from the specified I/O port
uint8_t inb(uint16_t port);


// Starts sound playback at the given frequency using the PIT
void play_sound(uint32_t frequency);

// Stops the sound from the speaker
void stop_sound(void);

#endif // PORT_IO_H
