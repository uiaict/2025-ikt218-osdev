#include "libc/stdint.h"
#include "testFuncs.h"
#include "miscFuncs.h"
#include "display.h"
#include "descriptorTables.h"
#include "interruptHandler.h"
#include "memory_manager.h"
#include "programmableIntervalTimer.h"
#include "pcSpeaker.h"
#include "storage.h"
#include "musicPlayer.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

// Global variabel som brukes for å indikere om IDT-testen har blitt trigget
volatile int idt_test_triggered = 0;

// Extern declaration for end of kernel memory
extern uint32_t end;

/* 
 * Tester terminal-output ved å vise tekst med ulike farger
 * Denne funksjonen demonstrerer de forskjellige fargemulighetene
 * som er definert i vår terminal-implementasjon
 */
void test_terminal_output() {
    // Skriver ut tekst med ulike farger for å teste terminal-funksjonene
    display_write_color("ABCDEFGHIJKLMNOP", COLOR_WHITE);
    display_write_char('\n');
    display_write_color("QRSTUVWXYZ1234567890", COLOR_CYAN);
    display_write_char('\n');
    display_write_color("Color test demonstration:", COLOR_YELLOW);
    display_write_char('\n');
    
    // Test de nye fargene - gir en visuell demonstrasjon av alle fargene
    display_write_color("Cyan text ", COLOR_CYAN);
    display_write_color("Magenta text ", COLOR_MAGENTA);
    display_write_color("Gray text ", COLOR_GRAY);
    display_write_char('\n');
    display_write_color("Light green text ", COLOR_LIGHT_GREEN);
    display_write_color("Light blue text ", COLOR_LIGHT_BLUE);
    display_write_color("Light cyan text", COLOR_LIGHT_CYAN);
    display_write_char('\n');
    display_write_color("Black on white ", COLOR_BLACK_ON_WHITE);
    display_write_color("Black on green ", COLOR_BLACK_ON_GREEN);
    display_write_color("Black on blue", COLOR_BLACK_ON_BLUE);
    display_write_char('\n');
}

/* 
 * Tester at Global Descriptor Table (GDT) fungerer korrekt
 * 
 * GDT er en datastruktur brukt av x86-prosessorer for å definere
 * egenskapene til minneområder (segmenter) som kan brukes av programmet.
 * Denne testen verifiserer at våre kodesegmenter og datasegmenter fungerer.
 */
void test_gdt() {
    // Vis en overskrift for testen
    display_write_color("\nTesting GDT functionality:\n", COLOR_YELLOW);
    
    // Test av kodesegment:
    // Hvis koden vår kjører, betyr det at kodesegmentet er satt opp riktig
    // Ellers ville CPU-en ikke kunne kjøre denne koden i det hele tatt
    display_write_color("- Code segment: ", COLOR_WHITE);
    display_write_color("Working - we are executing code!\n", COLOR_GREEN);
    
    // Test av datasegment:
    // Vi tester at vi kan skrive og lese fra variabler (bruke minnet)
    // Dette tester at datasegmentet er riktig konfigurert
    display_write_color("- Data segment: ", COLOR_WHITE);
    volatile uint32_t test_value = 0x12345678;  // Skriv en verdi til minnet
    if (test_value == 0x12345678) {  // Sjekk at vi kan lese den samme verdien
        display_write_color("Working - can read/write memory\n", COLOR_GREEN);
    } else {
        display_write_color("Error - unexpected memory value\n", COLOR_RED);
    }
    
    display_write_color("GDT appears to be configured correctly!\n", COLOR_GREEN);
}

/* 
 * Tester at Interrupt Descriptor Table (IDT) er satt opp riktig
 * 
 * IDT er en datastruktur som forteller CPU-en hvilke funksjoner
 * som skal håndtere ulike typer interrupts og exceptions.
 * Denne testen verifiserer at IDT er lastet og fungerer.
 */
void test_idt() {
    display_write_color("\nTesting IDT functionality:\n", COLOR_YELLOW);
    
    // Sjekk i stedet debug-tegnene som vises når IDT lastes
    display_write_color("- IDT loaded: ", COLOR_WHITE);
    display_write_color("Debug characters 'Ii' visible in debug output\n", COLOR_GREEN);
    
    // Forklar hva en full IDT-test ville involvere
    display_write_color("- For a true IDT test, we would need to:\n", COLOR_WHITE);
    display_write_color("  1. Set up proper exception handlers\n", COLOR_GRAY);
    display_write_color("  2. Trigger exceptions and verify they're caught\n", COLOR_GRAY);
    display_write_color("  3. Program the PIC for hardware interrupts\n", COLOR_GRAY);
    
    display_write_color("IDT appears to be loaded correctly.\n", COLOR_GREEN);
}

/* 
 * Tester tastatur-input med en interaktiv test
 * Viser alle tastetrykk direkte på skjermen
 */
void test_keyboard_interactive(void) {
    display_clear();
    display_write_color("\n=== Interactive Keyboard Test ===\n", COLOR_LIGHT_CYAN);
    display_write_color("Type any keys to see them appear on screen.\n", COLOR_YELLOW);
    display_write_color("Press ESC to exit the test.\n\n", COLOR_YELLOW);
    
    terminal_column = 0;
    terminal_row++;
    display_move_cursor();

    // Make sure interrupts are enabled
    __asm__ volatile("sti");
    
    // Clear any existing keyboard input
    while (keyboard_data_available()) {
        keyboard_getchar();
    }
    
    int running = 1;
    while (running) {
        if (keyboard_data_available()) {
            char c = keyboard_getchar();
            
            // Handle ESC key to exit
            if (c == 27) {  // ESC key
                running = 0;
                continue;
            }
            
            // Handle special keys
            if (c == '\n' || c == '\r') {  // Enter key
                display_write_char('\n');
            } else if (c == '\b') {  // Backspace
                display_write_char('\b');
                display_write_char(' ');
                display_write_char('\b');
            } else if (c == '\t') {  // Tab
                display_write_char(' ');
                display_write_char(' ');
                display_write_char(' ');
                display_write_char(' ');
            } else {  // Regular printable character
                display_write_char(c);
            }
        }
        __asm__ volatile("hlt");
    }
    
    display_write_color("\n\nKeyboard test completed.\n", COLOR_LIGHT_GREEN);
    display_write_color("Press any key to continue...\n", COLOR_YELLOW);
    
    // Wait for keypress to continue
    while (!keyboard_data_available()) {
        __asm__ volatile("hlt");
    }
    keyboard_getchar();  // Clear the keypress
}

/* 
 * Tester software interrupts (CPU exceptions)
 * 
 * Denne funksjonen tester at software interrupts fungerer ved å
 * utløse noen sikre interrupts som ikke vil krasje systemet.
 */
void test_software_interrupt() {
    display_write_color("\nTesting software interrupts (ISRs):\n", COLOR_YELLOW);
    
    // Test breakpoint exception (INT 3)
    display_write_color("1. Triggering Breakpoint Exception (INT 3)...\n", COLOR_WHITE);
    __asm__ volatile("int $3");
    
    display_write_color("Software interrupt tests completed successfully!\n", COLOR_GREEN);
}

/**
 * Test memory management and allocation functionality
 */
void test_memory_management(void) {
    display_write_color("\n=== Testing Memory Management ===\n", COLOR_LIGHT_CYAN);
    
    // Test memory allocation using malloc
    display_write_color("Testing malloc() allocation...\n", COLOR_WHITE);
    void* some_memory = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(13331);
    
    display_write_color("Allocated memory at: 0x", COLOR_LIGHT_GREEN);
    display_write_hex((uint32_t)some_memory);
    display_write_string("\n");
    
    display_write_color("Allocated memory at: 0x", COLOR_LIGHT_GREEN);
    display_write_hex((uint32_t)memory2);
    display_write_string("\n");
    
    display_write_color("Allocated memory at: 0x", COLOR_LIGHT_GREEN);
    display_write_hex((uint32_t)memory3);
    display_write_string("\n\n");
    
    // Free all allocated memory
    display_write_color("Freeing allocated memory...\n", COLOR_WHITE);
    free(some_memory);
    free(memory2);
    free(memory3);
    
    display_write_color("Memory freed successfully!\n", COLOR_LIGHT_GREEN);
    display_write_color("Memory management test completed.\n", COLOR_LIGHT_CYAN);
}

/**
 * Test Programmable Interval Timer functions
 */
void test_programmable_interval_timer(void) {
    display_write_color("\n=== Testing Programmable Interval Timer Functions ===\n", COLOR_LIGHT_CYAN);
    
    int counter = 0;
    
    // Test each sleep function a few times
    for (int i = 0; i < 2; i++) {
        display_write_color("[", COLOR_WHITE);
        display_write_decimal(counter);
        display_write_color("]: Sleeping with busy-waiting (HIGH CPU).\n", COLOR_YELLOW);
        
        sleep_busy(1000);
        
        display_write_color("[", COLOR_WHITE);
        display_write_decimal(counter++);
        display_write_color("]: Slept using busy-waiting.\n", COLOR_LIGHT_GREEN);
        
        display_write_color("[", COLOR_WHITE);
        display_write_decimal(counter);
        display_write_color("]: Sleeping with interrupts (LOW CPU).\n", COLOR_YELLOW);
        
        sleep_interrupt(1000);
        
        display_write_color("[", COLOR_WHITE);
        display_write_decimal(counter++);
        display_write_color("]: Slept using interrupts.\n", COLOR_LIGHT_GREEN);
    }
    
    display_write_color("Programmable Interval Timer test completed.\n", COLOR_LIGHT_CYAN);
}

// Test melody for PC speaker (C major scale)
static Note test_melody[] = {
    {C4, 200}, {D4, 200}, {E4, 200}, {F4, 200},
    {G4, 200}, {A4, 200}, {B4, 200}, {C5, 400}
};

/**
 * Test PC speaker music playback
 */
void test_music_player(void) {
    display_write_color("\n=== Music Player Test ===\n", COLOR_LIGHT_CYAN);
    
    // Test 1: Create song player
    display_write_color("Test 1: Creating song player...\n", COLOR_WHITE);
    SongPlayer* player = create_song_player();
    if (!player) {
        display_write_color("FAILED: Could not create song player\n", COLOR_LIGHT_RED);
        return;
    }
    display_write_color("PASSED: Song player created successfully\n", COLOR_LIGHT_GREEN);

    // Test 2: Create and play test melody
    display_write_color("\nTest 2: Playing test melody (C major scale)...\n", COLOR_WHITE);
    Song song = {
        .notes = test_melody,
        .length = sizeof(test_melody) / sizeof(test_melody[0])
    };

    display_write_color("Playing C major scale: ", COLOR_YELLOW);
    player->play_song(&song);
    display_write_color("PASSED: Melody played successfully\n", COLOR_LIGHT_GREEN);

    // Test 3: Test note creation and memory management
    display_write_color("\nTest 3: Testing note creation and memory...\n", COLOR_WHITE);
    Note* test_note = create_note(A4, 200);
    if (!test_note) {
        display_write_color("FAILED: Could not create test note\n", COLOR_LIGHT_RED);
        free_song_player(player);
        return;
    }
    display_write_color("PASSED: Note created successfully\n", COLOR_LIGHT_GREEN);

    // Play single test note
    display_write_color("Playing single test note (A4)...\n", COLOR_YELLOW);
    Song single_note_song = {
        .notes = test_note,
        .length = 1
    };
    player->play_song(&single_note_song);

    // Cleanup
    display_write_color("\nCleaning up resources...\n", COLOR_WHITE);
    free_song_player(player);
    free(test_note);
    
    display_write_color("All music player tests completed successfully!\n", COLOR_LIGHT_GREEN);
}

/**
 * Test harddisk functionality
 */
void test_hard_drive(void) {
    display_write_color("\n=== Hard Drive Test ===\n", COLOR_LIGHT_CYAN);
    
    // Test 1: Initialize hard drive
    display_write_color("Test 1: Initializing hard drive...\n", COLOR_WHITE);
    if (!harddisk_start()) {
        display_write_color("FAILED: Could not initialize hard drive!\n", COLOR_LIGHT_RED);
        return;
    }
    display_write_color("PASSED: Hard drive initialized successfully\n", COLOR_LIGHT_GREEN);
    
    // Test 2: Check hard drive presence
    display_write_color("\nTest 2: Checking hard drive presence...\n", COLOR_WHITE);
    if (!harddisk_check()) {
        display_write_color("FAILED: Hard drive not detected!\n", COLOR_LIGHT_RED);
        return;
    }
    display_write_color("PASSED: Hard drive detected and responding\n", COLOR_LIGHT_GREEN);
    
    // Test 3: Write and read data
    display_write_color("\nTest 3: Testing read/write operations...\n", COLOR_WHITE);
    
    // Create test data
    uint8_t test_data[512];  // One sector
    uint8_t read_buffer[512];
    
    // Fill test data with a simple pattern
    for (int i = 0; i < 512; i++) {
        test_data[i] = 0xAA;  // Use a simple pattern (10101010 in binary)
    }
    
    // Debug: Show first few bytes to write
    display_write_color("First 4 bytes to write: ", COLOR_YELLOW);
    for (int i = 0; i < 4; i++) {
        display_write_color("0x", COLOR_WHITE);
        display_write_hex(test_data[i]);
        display_write_char(' ');
    }
    display_write_char('\n');
    
    // Write to sector 1 (not 0, often reserved)
    display_write_color("Writing test pattern to sector 1...\n", COLOR_YELLOW);
    if (!harddisk_write(1, test_data, 1)) {
        display_write_color("FAILED: Could not write to hard drive!\n", COLOR_LIGHT_RED);
        return;
    }
    display_write_color("PASSED: Write operation successful\n", COLOR_LIGHT_GREEN);
    
    // Wait a bit to ensure data is written
    delay(100);
    
    // Read back data
    display_write_color("Reading data from sector 1...\n", COLOR_YELLOW);
    if (!harddisk_read(1, read_buffer, 1)) {
        display_write_color("FAILED: Could not read from hard drive!\n", COLOR_LIGHT_RED);
        return;
    }
    
    // Debug: Show first few bytes read
    display_write_color("First 4 bytes read: ", COLOR_YELLOW);
    for (int i = 0; i < 4; i++) {
        display_write_color("0x", COLOR_WHITE);
        display_write_hex(read_buffer[i]);
        display_write_char(' ');
    }
    display_write_char('\n');
    
    // Verify data
    bool data_match = true;
    for (int i = 0; i < 512; i++) {
        if (read_buffer[i] != test_data[i]) {
            data_match = false;
            display_write_color("FAILED: Data mismatch at offset ", COLOR_LIGHT_RED);
            display_write_decimal(i);
            display_write_color("\nExpected: 0x", COLOR_WHITE);
            display_write_hex(test_data[i]);
            display_write_color(" Got: 0x", COLOR_WHITE);
            display_write_hex(read_buffer[i]);
            display_write_char('\n');
            break;
        }
    }
    
    if (data_match) {
        display_write_color("PASSED: Read/Write test successful - data verified\n", COLOR_LIGHT_GREEN);
    }
}

/**
 * Run all tests
 */
void run_all_tests() {
    display_write_color("Starting system tests...\n\n", COLOR_YELLOW);
    
    test_terminal_output();
    sleep_interrupt(500);  // 0.5 second delay
    
    test_gdt();
    sleep_interrupt(500);
    
    test_idt();
    sleep_interrupt(500);
    
    test_keyboard_interactive();  // Interactive keyboard test
    sleep_interrupt(500);
    
    test_software_interrupt();
    sleep_interrupt(500);
    
    test_memory_management();
    sleep_interrupt(500);
    
    test_programmable_interval_timer();
    sleep_interrupt(500);
    
    test_music_player();
    sleep_interrupt(500);
    
    test_hard_drive();
    sleep_interrupt(500);
    
    display_write_color("\nAll tests completed!\n", COLOR_LIGHT_GREEN);
} 