#include "sound.h"
#include "timer.h"
#include "io.h"

uint32_t spk_frequency;

void enable_speaker(){

 	uint8_t spk_state = inb(IO_PORT);

   // Prevents unesessary I/O write
  	if (spk_state != (spk_state | 0x03)){ 
      // If last two bits is not 1, set them both to 1
 		outb(IO_PORT, spk_state | 0x03);
 	}
}

void disable_speaker(){

 	uint8_t spk_state = inb(IO_PORT);

   // Prevents unesessary I/O write
  	if (spk_state != (spk_state & 0xFC)){ 
      // If last two bits is not 0, set them both to 0
 		outb(IO_PORT, spk_state & 0xFC);
 	}
}

void play_sound(uint32_t freq){

   spk_frequency = freq;

 	uint32_t divisor = 1193180 / spk_frequency;
 
    outb(PIT_COMMAND, 0xB6); // 0x36 = 10 11 011 0
                             // channel2, l/h, mode3, 16bit binary

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t lo = (uint8_t)(divisor & 0xFF);
    uint8_t hi = (uint8_t)((divisor>>8) & 0xFF);
 
    // Send the frequency divisor.
    outb(PIT_DATACHANNEL_2, lo);
    outb(PIT_DATACHANNEL_2, hi);

}

void stop_sound(){
 	uint8_t spk_state = inb(IO_PORT);

   // Prevents unesessary I/O write
  	if (spk_state != (spk_state & 0xFD)){ 
      // If second to last bit is not 0, set it to 0
 		outb(IO_PORT, spk_state & 0xFD);
 	}
}

void play_song(){

}