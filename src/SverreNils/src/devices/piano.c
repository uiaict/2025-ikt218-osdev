#include "printf.h"
#include "pit.h"
#include "libc/io.h"
#include "arch/irq.h"
#include "devices/keyboard.h"
#include "devices/song_player.h"

#define PC_SPEAKER_PORT 0x61
#define PIT_CHANNEL2    0x42
#define PIT_COMMAND     0x43

static int piano_active = 0;

void play_note(uint32_t freq) {
    if (freq == 0) return;

    uint32_t divisor = 1193182 / freq;
    outb(0x43, 0xB6);
    outb(0x42, (uint8_t)(divisor & 0xFF));
    outb(0x42, (uint8_t)((divisor >> 8) & 0xFF));

    enable_speaker();
}

void stop_note() {
    disable_speaker();
}

static void piano_key_handler(uint8_t scancode) {
    static uint32_t freqs[8] = { 262, 294, 330, 349, 392, 440, 494, 523 };

    if (scancode >= 2 && scancode <= 9) {
        uint32_t freq = freqs[scancode - 2];
        play_note(freq);
        sleep_interrupt(200);
        stop_note();
    }
}
static void piano_keyboard_wrapper(struct registers* regs) {
    if (!piano_active) return;

    uint8_t scancode = inb(0x60);

    if (scancode == 1) { 
        printf("\n🎹 Avslutter piano-modus.\n");
        piano_active = 0;

        extern void restore_keyboard_handler();  
        restore_keyboard_handler();              

        extern void shell_prompt();              
        shell_prompt();
        return;
    }

    piano_key_handler(scancode); 
}


void init_piano() {
    piano_active = 1;
    irq_register_handler(1, piano_keyboard_wrapper);
    printf("\n🎹 Piano mode aktiv! Trykk 1-8 for toner. (ESC for å avslutte)\n");
}
