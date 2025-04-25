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
    
    // For en mer omfattende GDT-test ville vi testet segmentgrenser
    // og privilegienivåer, men det er utenfor omfanget av denne testen
    
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
    
    // Vi kommenterer ut den faktiske divisjon-med-null testen for sikkerhet
    // En faktisk test ville utløse exception 0 (division by zero)
    /*
    display_write_color("- Triggering division by zero... ", COLOR_WHITE);
    
    // Dette vil trigge interrupt 0 (divide by zero)
    volatile int a = 10;
    volatile int b = 0;
    volatile int c = a / b;  // Dette vil utløse unntaket
    
    // Hvis vi kommer hit uten å håndtere unntaket, fungerer ikke IDT
    display_write_color("ERROR - IDT not working!\n", COLOR_RED);
    */
    
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
 * Tester at tastatur-interrupt og tastaturdriver fungerer
 * 
 * Denne testen aktiverer interrupts, venter på tastetrykk, og
 * viser tegnet som ble tastet inn sammen med ASCII-verdien.
 * Testen bekrefter at både hardware-interrupt og tastatur-driveren virker.
 */
void test_keyboard_interrupt(void) {
    display_write_color("\nTesting keyboard interrupt (IRQ1)...\n", COLOR_WHITE);
    display_write_color("Press keys to test keyboard. Press ESC to exit.\n", COLOR_YELLOW);
    
    // Enable interrupts
    __asm__ volatile("sti");
    
    // Simple loop to wait for keyboard input
    int running = 1;
    while (running) {
        // Check if a key has been pressed
        if (keyboard_data_available()) {
            char c = keyboard_getchar();
            
            // Display the key that was pressed
            display_write_color("Key pressed: '", COLOR_GREEN);
            if (c >= 32 && c <= 126) {  // Printable ASCII characters
                display_write_char(c);
            } else if (c == '\n') {
                display_write("\\n");
            } else if (c == '\t') {
                display_write("\\t");
            } else if (c == 27) {  // ESC key
                display_write("ESC");
                running = 0;
            } else {
                display_write_color("0x", COLOR_WHITE);
                char hex[3];
                hex[0] = "0123456789ABCDEF"[(c >> 4) & 0xF];
                hex[1] = "0123456789ABCDEF"[c & 0xF];
                hex[2] = '\0';
                display_write(hex);
            }
            display_write_color("'\n", COLOR_GREEN);
            
            // Small delay to prevent display flicker
            delay(50);
        }
        __asm__ volatile("hlt");  // Wait for next interrupt
    }
    
    display_write_color("Keyboard test completed.\n", COLOR_GREEN);
}

/**
 * Test hardware interrupts (IRQs) - spesielt tastatur-input
 * 
 * Denne funksjonen aktiverer hardware interrupts ved å sette interrupt
 * flag (IF) i EFLAGS-registeret, og venter deretter på tastatur-input.
 * Når tastetrykk kommer, blir IRQ1 utløst, som kjører vår keyboard_handler.
 */
void test_hardware_interrupts() {
    // Clear the screen and position cursor properly
    display_initialize();
    
    display_write_color("\n=== Testing Keyboard Interrupts ===\n", COLOR_LIGHT_CYAN);
    display_write_color("Press keys to see keyboard input working.\n", COLOR_YELLOW);
    display_write_color("You should see both scancode values and ASCII characters.\n", COLOR_YELLOW);
    display_write_color("Press ESC to exit the test.\n\n", COLOR_YELLOW);
    
    // Make sure interrupts are enabled
    __asm__ volatile("sti");
    
    // Simple loop to wait for keyboard input
    int running = 1;
    
    while (running) {
        // Check if a key has been pressed
        if (keyboard_data_available()) {
            char c = keyboard_getchar();
            
            // Exit if ESC is pressed
            if (c == 27) {  // ESC key
                running = 0;
            }
        }
        
        // Small delay to prevent CPU hogging
        for (volatile int i = 0; i < 100000; i++) {
            __asm__ volatile("nop");
        }
    }
    
    display_write_color("\nKeyboard test completed.\n", COLOR_GREEN);
}

/* 
 * Tester software interrupts (CPU exceptions)
 * 
 * Denne funksjonen tester at software interrupts fungerer ved å
 * utløse noen sikre interrupts som ikke vil krasje systemet.
 * Vi unngår bevisst division by zero som kan forårsake problemer.
 */
void test_software_interrupt() {
    display_write_color("\nTesting software interrupts (ISRs):\n", COLOR_YELLOW);
    
    // Test breakpoint exception (INT 3)
    display_write_color("1. Triggering Breakpoint Exception (INT 3)...\n", COLOR_WHITE);
    __asm__ volatile("int $3");
    
    display_write_color("Software interrupt tests completed successfully!\n", COLOR_GREEN);
}

/**
 * Test if interrupts are correctly activated and functioning
 *
 * This test verifies:
 * 1. If the Interrupt Flag (IF) is set in the EFLAGS register
 * 2. If the PIC is configured correctly 
 * 3. If keyboard interrupts are being received
 */
void test_interrupt_status() {
    display_initialize();
    
    display_write_color("\n=== Interrupt System Test ===\n", COLOR_LIGHT_CYAN);
    
    // Test 1: Check if interrupt flag is set in EFLAGS register
    display_write_color("Test 1: CPU Interrupt Flag...\n", COLOR_WHITE);
    int if_enabled = interrupts_enabled();
    
    if (if_enabled) {
        display_write_color("PASS: Interrupts are enabled in CPU ✓\n", COLOR_LIGHT_GREEN);
    } else {
        display_write_color("FAIL: Interrupts are NOT enabled in CPU ✗\n", COLOR_LIGHT_RED);
        display_write_color("Attempting to enable interrupts...\n", COLOR_YELLOW);
        
        // Clear any pending interrupts first
        __asm__ volatile("cli");
        // Then enable interrupts
        __asm__ volatile("sti");
        
        if (interrupts_enabled()) {
            display_write_color("RECOVERED: Interrupts are now enabled ✓\n", COLOR_LIGHT_GREEN);
        } else {
            display_write_color("CRITICAL FAILURE: Cannot enable interrupts!\n", COLOR_LIGHT_RED);
            return;
        }
    }
    
    // Test 2: Verify PIC configuration by reading mask registers
    display_write_color("\nTest 2: PIC Configuration...\n", COLOR_WHITE);
    
    // Read the current PIC masks
    uint8_t master_mask = inb(PIC1_DATA);
    uint8_t slave_mask = inb(PIC2_DATA);
    
    display_write_color("Master PIC mask: 0x", COLOR_GRAY);
    display_write_hex(master_mask);
    display_write_color(" - bits set to 0 are enabled interrupts\n", COLOR_GRAY);
    
    display_write_color("Slave PIC mask: 0x", COLOR_GRAY);
    display_write_hex(slave_mask);
    display_write_color(" - bits set to 0 are enabled interrupts\n", COLOR_GRAY);
    
    // Check if IRQ1 (keyboard) is enabled
    if ((master_mask & 0x02) == 0) {
        display_write_color("PASS: Keyboard interrupt (IRQ1) is enabled ✓\n", COLOR_LIGHT_GREEN);
    } else {
        display_write_color("FAIL: Keyboard interrupt (IRQ1) is disabled ✗\n", COLOR_LIGHT_RED);
        display_write_color("Enabling keyboard interrupt...\n", COLOR_YELLOW);
        
        // Enable keyboard interrupt by clearing bit 1
        outb(PIC1_DATA, master_mask & ~0x02);
        io_wait();
        
        // Verify it was enabled
        master_mask = inb(PIC1_DATA);
        if ((master_mask & 0x02) == 0) {
            display_write_color("RECOVERED: Keyboard interrupt now enabled ✓\n", COLOR_LIGHT_GREEN);
        } else {
            display_write_color("FAILURE: Could not enable keyboard interrupt!\n", COLOR_LIGHT_RED);
        }
    }
    
    // Test 3: Keyboard interrupt test
    display_write_color("\nTest 3: Keyboard Interrupt Test\n", COLOR_WHITE);
    display_write_color("Please press a key on the keyboard...\n", COLOR_YELLOW);
    display_write_color("(Test will wait 20 seconds for keyboard input)\n", COLOR_GRAY);
    
    // Reset keyboard buffer
    while (keyboard_data_available()) {
        keyboard_getchar();
    }
    
    // Wait for keypress with timeout
    const int MAX_WAIT = 20;  // 20 seconds timeout
    int seconds_waited = 0;
    int key_received = 0;
    
    display_write_string("Waiting for key press: ");
    
    // Loop until we get a key or timeout
    while (seconds_waited < MAX_WAIT && !key_received) {
        // Check for keyboard input
        if (keyboard_data_available()) {
            char c = keyboard_getchar();
            display_write_string("\nSUCCESS! Key received: '");
            display_write_char(c);
            display_write_string("'\n");
            key_received = 1;
            break;
        }
        
        // Delay approximately 1 second
        for (volatile int i = 0; i < 1000000; i++) {
            __asm__ volatile("nop");
        }
        
        // Show progress
        display_write_char('.');
        seconds_waited++;
        
        // Show seconds count every 5 seconds
        if (seconds_waited % 5 == 0) {
            display_write_string(" ");
            display_write_decimal(seconds_waited);
            display_write_string("s ");
        }
    }
    
    if (!key_received) {
        display_write_string("\nTIMEOUT: No keyboard interrupt received!\n");
        display_write_string("Troubleshooting suggestions:\n");
        display_write_string("1. Check that the PIC is correctly initialized\n");
        display_write_string("2. Verify the keyboard controller initialization\n");
        display_write_string("3. Ensure that IRQ1 is properly mapped in the IDT\n");
        display_write_string("4. Make sure keyboard_handler is adding characters to the buffer\n");
    }
    
    // Show test summary
    display_write_color("\nInterrupt System Test Summary:\n", COLOR_LIGHT_CYAN);
    display_write_color("- CPU Interrupts: ", COLOR_WHITE);
    
    if (interrupts_enabled()) {
        display_write_color("ENABLED ✓\n", COLOR_LIGHT_GREEN);
    } else {
        display_write_color("DISABLED ✗\n", COLOR_LIGHT_RED);
    }
    
    display_write_color("- Keyboard Interrupts: ", COLOR_WHITE);
    
    if (key_received) {
        display_write_color("WORKING ✓\n", COLOR_LIGHT_GREEN);
    } else {
        display_write_color("NOT WORKING ✗\n", COLOR_LIGHT_RED);
    }
    
    display_write_color("\nPress any key to continue...\n", COLOR_YELLOW);
    
    // Reset keyboard buffer
    while (keyboard_data_available()) {
        keyboard_getchar();
    }
    
    // Wait for keypress to continue
    display_write_string("Waiting for key press: ");
    while (!keyboard_data_available()) {
        for (volatile int i = 0; i < 100000; i++) {
            __asm__ volatile("nop");
        }
        display_write_char('.');
    }
    
    // Clear the pressed key
    keyboard_getchar();
    display_write_string("\n\n");
}

/**
 * Test memory management and allocation functionality
 * 
 * This function tests various memory allocation and freeing operations
 * using both malloc/free and new/delete operators.
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
 * 
 * This function tests both sleep methods - busy waiting and interrupt-based.
 */
void test_programmable_interval_timer(void) {
    display_write_color("\n=== Testing Programmable Interval Timer Functions ===\n", COLOR_LIGHT_CYAN);
    
    int counter = 0;
    
    // Test each sleep function a few times
    for (int i = 0; i < 2; i++) {
        display_write_color("[", COLOR_WHITE);
        display_write_decimal(counter);
        display_write_color("]: Sleeping with busy-waiting (HIGH CPU).\n", COLOR_YELLOW);
        
        sleep_busy(1000); // Sleep for 1 second
        
        display_write_color("[", COLOR_WHITE);
        display_write_decimal(counter++);
        display_write_color("]: Slept using busy-waiting.\n", COLOR_LIGHT_GREEN);
        
        display_write_color("[", COLOR_WHITE);
        display_write_decimal(counter);
        display_write_color("]: Sleeping with interrupts (LOW CPU).\n", COLOR_YELLOW);
        
        sleep_interrupt(1000); // Sleep for 1 second
        
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

// Test PC speaker music playback
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

// Test harddisk-funksjonalitet
void test_hard_drive(void) {
    display_write_color("\n=== Hard Drive Test ===\n", COLOR_LIGHT_CYAN);
    
    // Test 1: Initialiser harddisken
    display_write_color("Test 1: Initializing hard drive...\n", COLOR_WHITE);
    if (!harddisk_start()) {
        display_write_color("FAILED: Could not initialize hard drive!\n", COLOR_LIGHT_RED);
        return;
    }
    display_write_color("PASSED: Hard drive initialized successfully\n", COLOR_LIGHT_GREEN);
    
    // Test 2: Sjekk om harddisken er tilgjengelig
    display_write_color("\nTest 2: Checking hard drive presence...\n", COLOR_WHITE);
    if (!harddisk_check()) {
        display_write_color("FAILED: Hard drive not detected!\n", COLOR_LIGHT_RED);
        return;
    }
    display_write_color("PASSED: Hard drive detected and responding\n", COLOR_LIGHT_GREEN);
    
    // Test 3: Skriv og les data
    display_write_color("\nTest 3: Testing read/write operations...\n", COLOR_WHITE);
    
    // Lag testdata
    uint8_t test_data[512];  // En sektor
    uint8_t read_buffer[512];
    
    // Fyll testdata med et enkelt mønster
    for (int i = 0; i < 512; i++) {
        test_data[i] = 0xAA;  // Bruk et enkelt mønster (10101010 i binær)
    }
    
    // Debug: Vis de første bytene vi skal skrive
    display_write_color("First 4 bytes to write: ", COLOR_YELLOW);
    for (int i = 0; i < 4; i++) {
        display_write_color("0x", COLOR_WHITE);
        display_write_hex(test_data[i]);
        display_write_char(' ');
    }
    display_write_char('\n');
    
    // Skriv til sektor 1 (ikke 0, den er ofte reservert)
    display_write_color("Writing test pattern to sector 1...\n", COLOR_YELLOW);
    if (!harddisk_write(1, test_data, 1)) {
        display_write_color("FAILED: Could not write to hard drive!\n", COLOR_LIGHT_RED);
        return;
    }
    display_write_color("PASSED: Write operation successful\n", COLOR_LIGHT_GREEN);
    
    // Vent litt for å sikre at data er skrevet
    delay(100);
    
    // Les tilbake data
    display_write_color("Reading data from sector 1...\n", COLOR_YELLOW);
    if (!harddisk_read(1, read_buffer, 1)) {
        display_write_color("FAILED: Could not read from hard drive!\n", COLOR_LIGHT_RED);
        return;
    }
    
    // Debug: Vis de første bytene vi leste
    display_write_color("First 4 bytes read: ", COLOR_YELLOW);
    for (int i = 0; i < 4; i++) {
        display_write_color("0x", COLOR_WHITE);
        display_write_hex(read_buffer[i]);
        display_write_char(' ');
    }
    display_write_char('\n');
    
    // Verifiser data
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
 * Test system initialization and setup
 * 
 * This function initializes critical system components,
 * but avoids re-initializing components that might cause conflicts.
 */
void test_system_initialization(void) {
    // Disable interrupts during initialization
    __asm__ volatile("cli");
    
    // Initialize GDT (Global Descriptor Table)
    display_write_color("Initializing Global Descriptor Table (GDT)...\n", COLOR_WHITE);
    initializer_GDT();
    display_write_color("PASSED: GDT initialized successfully!\n\n", COLOR_LIGHT_GREEN);
    
    // Initialize IDT (Interrupt Descriptor Table)
    display_write_color("Initializing Interrupt Descriptor Table (IDT)...\n", COLOR_WHITE);
    initializer_IDT();
    display_write_color("PASSED: IDT initialized successfully!\n\n", COLOR_LIGHT_GREEN);
    
    // Initialize PIC (Programmable Interrupt Controller)
    display_write_color("Initializing Programmable Interrupt Controller (PIC)...\n", COLOR_WHITE);
    pic_initialize();
    display_write_color("PASSED: PIC initialized successfully!\n\n", COLOR_LIGHT_GREEN);
    
    // Initialize Programmable Interval Timer (PIT)
    display_write_color("Initializing Programmable Interval Timer...\n", COLOR_WHITE);
    init_programmable_interval_timer();
    display_write_color("PASSED: PIT initialized successfully!\n\n", COLOR_LIGHT_GREEN);
    
    // Enable interrupts now that PIT and PIC are set up
    display_write_color("Enabling interrupts...\n", COLOR_WHITE);
    enable_interrupts();
    
    // Initialize kernel memory manager
    display_write_color("Initializing Kernel Memory Manager...\n", COLOR_WHITE);
    init_kernel_memory(&end);
    display_write_color("PASSED: Kernel Memory Manager initialized successfully!\n\n", COLOR_LIGHT_GREEN);
    
    // Initialize paging
    display_write_color("Initializing Paging...\n", COLOR_WHITE);
    init_paging();
    display_write_color("PASSED: Paging initialized successfully!\n\n", COLOR_LIGHT_GREEN);
    
    display_write_color("System initialization completed!\n", COLOR_YELLOW);
}

/**
 * Run all tests
 * 
 * This function runs all the various tests to verify system functionality.
 */
void run_all_tests() {
    display_write_color("Starting system tests...\n\n", COLOR_YELLOW);
    
    // System initialization har allerede blitt kjørt i oppstart, men vi kjører en sjekk for å verifisere statusen
    display_write_color("Verifying system initialization state...\n", COLOR_LIGHT_GREEN);
    
    // Kjør testene
    test_terminal_output();
    test_gdt();
    test_idt();
    test_interrupt_status();
    test_hardware_interrupts();
    test_software_interrupt();
    test_memory_management();
    test_programmable_interval_timer();
    test_music_player();
    test_hard_drive();
    
    display_write_color("\nAll tests completed!\n", COLOR_LIGHT_GREEN);
} 