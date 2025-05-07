#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <multiboot2.h>

#include "../terminal.h"
#include "../gdt.h"

#include "kmem.h"
#include "paging.h"
#include "pit.h"
#include "bootinfo.h"
#include "printf.h"
#include "song_player.h"
#include "song.h"
#include "../serial.h"       // Include the serial header

extern void idt_install(void);
extern void pic_remap(int offset1, int offset2);
extern uint32_t end;    

// Function to play music through the PC speaker
void play_music(void) {
    // Create a song using the predefined notes
    Song song;
    song.notes = music_1;
    song.length = sizeof(music_1) / sizeof(Note);
    song.id = 0;
    
    // Create a song player
    SongPlayer* player = create_song_player();
    
    // Play the song
    terminal_write("Playing music through PC Speaker...\n");
    serial_write("Playing music through PC Speaker...\n");
    
    player->play_song(&song);
    
    terminal_write("Music playback completed.\n");
    serial_write("Music playback completed.\n");
}

int main(uint32_t magic, void *mb_addr) {
    gdt_install();
    terminal_initialize();
    init_serial();  // Initialize serial port

    pic_remap(0x20, 0x28);
    idt_install();

    init_kernel_memory(&end);
    init_paging();

    struct multiboot_tag *tag = (void *)((uint8_t *)mb_addr + 8);
    const struct multiboot_tag *raw =
        mb2_find_tag(tag, MULTIBOOT_TAG_TYPE_MMAP);
    const struct multiboot_tag_mmap *mmap_tag =
        (const struct multiboot_tag_mmap *)raw;
    if (mmap_tag) {
        print_memory_layout(mmap_tag, (uint32_t)&end);
    }

    init_pit();
    asm volatile("sti");

    terminal_write("Hello from the IDT-enabled kernel!\n");
    serial_write("Serial port initialized - logging enabled\n");

    void *a = kmalloc(1024, 0);
    void *b = kmalloc(2048, 0);
    kfree(a);
    kfree(b);
    terminal_write("heap test OK\n");
    serial_write("heap test OK\n");

    int counter = 0;
    for (int count = 0; count < 5; count++) {
        printf("[%d]: Sleeping with busy-waiting...\n", counter);
        serial_write("Sleeping with busy-waiting...\n");
        sleep_busy(1000);
        printf("[%d]: Woke up from busy-waiting\n", counter++);
        serial_write("Woke up from busy-waiting\n");

        printf("[%d]: Sleeping with interrupts...\n", counter);
        serial_write("Sleeping with interrupts...\n");
        sleep_interrupt(1000);
        printf("[%d]: Woke up from interrupts\n", counter++);
        serial_write("Woke up from interrupts\n");
    }

    terminal_write("Playing music through PC Speaker...\n");
    serial_write("Playing music through PC Speaker...\n");
    
    // Play the actual song
    play_music();
    
    for (;;)
        asm volatile ("hlt");
    return 0;
}