// kernel.c

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "kprint.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "keyboard.h"
#include "memory.h"
#include "pit.h"
#include "musicplayer.h"

// The linker script defines this symbol (end of kernel).
extern unsigned long end;

// Define a better test song with a wider range of notes
Note music_1[] = {
    {262, 200},   // C4
    {294, 200},   // D4
    {330, 200},   // E4
    {349, 200},   // F4
    {392, 200},   // G4
    {440, 200},   // A4
    {494, 200},   // B4
    {523, 400},   // C5 (longer note)
    {0,   200},   // Rest
    {523, 200},   // C5
    {494, 200},   // B4
    {440, 200},   // A4
    {392, 200},   // G4
    {349, 200},   // F4
    {330, 200},   // E4
    {294, 200},   // D4
    {262, 400}    // C4 (longer note)
};

// play_music creates a Song (with our test music_1) and uses the music player to play it.
void play_music(void) {
    Song songs[] = {
        { music_1, sizeof(music_1) / sizeof(Note) }
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);

    SongPlayer* player = create_song_player();
    if (!player) {
        kprint("Failed to create SongPlayer.\n");
        return;
    }

    // Continuously play each song.
    while (1) {
        for (uint32_t i = 0; i < n_songs; i++) {
            kprint("Playing Song...\n");
            player->play_song(&songs[i]);
            kprint("Finished playing the song.\n");
            
            // Wait between songs
            sleep_interrupt(1000);
        }
    }

    // Unreachable free (included for correctness).
    free(player);
}

int main(unsigned long magic, struct multiboot_info* mb_info_addr) {
    // Write "Hello World" directly to video memory (VGA text mode).
    const char *str = "Hello World";
    char *video_memory = (char*)0xb8000;
    for (int i = 0; str[i] != '\0'; i++) {
        video_memory[i * 2]     = str[i];
        video_memory[i * 2 + 1] = 0x07;  // White text on black background.
    }

    kprint("Loading GDT...\n");
    init_gdt();
    kprint("GDT loaded\n");

    kprint("Initializing IDT...\n");
    idt_init();
    kprint("IDT initialized\n");

    kprint("Initializing ISR...\n");
    isr_init();
    kprint("ISR initialized\n");

    kprint("Initializing PIC...\n");
    pic_init();
    kprint("PIC initialized\n");

    // Enable interrupts.
    kprint("Enabling interrupts...\n");
    __asm__ volatile ("sti");
    kprint("Interrupts enabled\n");

    // Initialize kernel memory manager using the address from the linker.
    kprint("Initializing kernel memory manager...\n");
    init_kernel_memory(&end);

    // Initialize paging.
    kprint("Initializing paging...\n");
    init_paging();

    // Print current memory layout.
    kprint("Printing memory layout...\n");
    print_memory_layout();

    // Initialize the PIT.
    kprint("Initializing PIT...\n");
    init_pit();

    // Display the initial tick count.
    kprint("Initial tick count: ");
    kprint_dec(get_tick_count());
    kprint("\n");

    // Initialize keyboard input.
    kprint("Initializing keyboard...\n");
    keyboard_init();

    // Test software interrupts.
    kprint("Testing NMI interrupt (int 0x2)...\n");
    __asm__ volatile ("int $0x2");
    kprint("Testing breakpoint interrupt (int 0x3)...\n");
    __asm__ volatile ("int $0x3");

    // Test memory allocation.
    kprint("\nTesting memory allocation...\n");
    void* some_memory = malloc(12345);
    void* memory2    = malloc(54321);
    void* memory3    = malloc(13331);
    
    kprint("Allocated memory at: 0x");
    kprint_hex((unsigned long)some_memory);
    kprint("\n");
    
    kprint("Allocated memory at: 0x");
    kprint_hex((unsigned long)memory2);
    kprint("\n");
    
    kprint("Allocated memory at: 0x");
    kprint_hex((unsigned long)memory3);
    kprint("\n");

    // Print updated memory layout.
    kprint("\nUpdated memory layout after allocations:\n");
    print_memory_layout();
    
    // Free one allocation and show updated memory layout.
    kprint("\nFreeing memory...\n");
    free(memory2);
    kprint("Memory layout after free:\n");
    print_memory_layout();

    kprint("\nSystem initialized successfully!\n");
    kprint("Press any key to see keyboard input...\n");

    // Unmask keyboard IRQ (IRQ1)
    outb(PIC1_DATA_PORT, inb(PIC1_DATA_PORT) & ~0x02);

   // Initialize the PC speaker
    kprint("Initializing PC Speaker...\n");
    //init_pc_speaker();

    // Play test beeps to verify speaker is working
    kprint("Playing test beeps...\n");
    beep_blocking(1000, 300);  // 1kHz tone for 300ms
    sleep_interrupt(200);      // Brief pause
    beep_blocking(1500, 300);  // 1.5kHz tone for 300ms

    // Play the music
    kprint("\nStarting music player...\n");
    play_music();

    // Main idle loop (should never reach here because play_music has its own loop)
    kprint("Entering main idle loop...\n");
    while (1) {
        __asm__ volatile ("hlt");
    }

    return 0;
}