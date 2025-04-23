#include "pit.h"
#include "libc/stdint.h"
#include "io.h"
#include "terminal.h"
#include "isr.h"
#include "libc/stdio.h"  // Add this for printf and number conversion

// Global variables
static volatile uint32_t pit_ticks = 0;
static uint32_t current_frequency = 0;

// Convert integer to string
static void int_to_string(int value, char* buffer) {
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    int i = 0;
    int is_negative = 0;

    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    // Convert digits to characters in reverse order
    while (value != 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

void pit_handler(void) {
    pit_ticks++;
    outb(PIC1_CMD_PORT, PIC_EOI);
}

uint32_t get_current_tick(void) {
    return pit_ticks;
}

void init_pit(void) {
    // Calculate divisor for ~1ms ticks (1000 Hz)
    uint16_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;

    // Configure channel 0 (system timer)
    outb(PIT_CMD_PORT, 0x36);    // Channel 0, lobyte/hibyte, square wave mode
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);

    // Reset tick counter
    pit_ticks = 0;

    // Log initialization
    char freq_str[16];
    int_to_string(TARGET_FREQUENCY, freq_str);
    terminal_write("PIT initialized at ");
    terminal_write(freq_str);
    terminal_write(" Hz\n");
}

void pit_set_speaker_freq(uint32_t frequency) {
    if (frequency == 0) {
        // Turn off the speaker
        uint8_t tmp = inb(PC_SPEAKER_PORT);
        outb(PC_SPEAKER_PORT, tmp & 0xFC);
        current_frequency = 0;
        return;
    }

    // Calculate divisor
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;
    if (divisor > 0xFFFF) divisor = 0xFFFF; // Clamp to maximum

    // Configure channel 2 (speaker)
    outb(PIT_CMD_PORT, 0xB6);    // Channel 2, lobyte/hibyte, square wave mode
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);

    // Enable speaker
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 3);
    
    current_frequency = frequency;
}

void sleep_busy(uint32_t milliseconds) {
    if (milliseconds == 0) return;

    uint32_t start = get_current_tick();
    uint32_t target_ticks = milliseconds * TICKS_PER_MS;
    uint32_t end_tick = start + target_ticks;

    // Handle potential overflow
    if (end_tick < start) {
        while (get_current_tick() >= start) {
            asm volatile("pause");
        }
        while (get_current_tick() < end_tick) {
            asm volatile("pause");
        }
    } else {
        while (get_current_tick() < end_tick) {
            asm volatile("pause");
        }
    }
}

void sleep_interrupt(uint32_t milliseconds) {
    if (milliseconds == 0) return;

    uint32_t start = get_current_tick();
    uint32_t target_ticks = milliseconds * TICKS_PER_MS;
    uint32_t end_tick = start + target_ticks;

    // Handle potential overflow
    if (end_tick < start) {
        while (get_current_tick() >= start) {
            asm volatile("sti; hlt; cli");
        }
        while (get_current_tick() < end_tick) {
            asm volatile("sti; hlt; cli");
        }
    } else {
        while (get_current_tick() < end_tick) {
            asm volatile("sti; hlt; cli");
        }
    }
}