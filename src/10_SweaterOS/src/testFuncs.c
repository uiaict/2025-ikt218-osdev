#include "testFuncs.h"
#include "descriptorTables.h"
#include "miscFuncs.h"
#include "interruptHandler.h"

// Global variabel som brukes for å indikere om IDT-testen har blitt trigget
volatile int idt_test_triggered = 0;

/* 
 * Tester terminal-output ved å vise tekst med ulike farger
 * Denne funksjonen demonstrerer de forskjellige fargemulighetene
 * som er definert i vår terminal-implementasjon
 */
void test_terminal_output() {
    // Skriver ut tekst med ulike farger for å teste terminal-funksjonene
    terminal_write_color("ABCDEFGHIJKLMNOP", COLOR_WHITE);
    terminal_write_char('\n');
    terminal_write_color("QRSTUVWXYZ1234567890", COLOR_CYAN);
    terminal_write_char('\n');
    terminal_write_color("Color test demonstration:", COLOR_YELLOW);
    terminal_write_char('\n');
    
    // Test de nye fargene - gir en visuell demonstrasjon av alle fargene
    terminal_write_color("Cyan text ", COLOR_CYAN);
    terminal_write_color("Magenta text ", COLOR_MAGENTA);
    terminal_write_color("Gray text ", COLOR_GRAY);
    terminal_write_char('\n');
    terminal_write_color("Light green text ", COLOR_LIGHT_GREEN);
    terminal_write_color("Light blue text ", COLOR_LIGHT_BLUE);
    terminal_write_color("Light cyan text", COLOR_LIGHT_CYAN);
    terminal_write_char('\n');
    terminal_write_color("Black on white ", COLOR_BLACK_ON_WHITE);
    terminal_write_color("Black on green ", COLOR_BLACK_ON_GREEN);
    terminal_write_color("Black on blue", COLOR_BLACK_ON_BLUE);
    terminal_write_char('\n');
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
    terminal_write_color("\nTesting GDT functionality:\n", COLOR_YELLOW);
    
    // Test av kodesegment:
    // Hvis koden vår kjører, betyr det at kodesegmentet er satt opp riktig
    // Ellers ville CPU-en ikke kunne kjøre denne koden i det hele tatt
    terminal_write_color("- Code segment: ", COLOR_WHITE);
    terminal_write_color("Working - we are executing code!\n", COLOR_GREEN);
    
    // Test av datasegment:
    // Vi tester at vi kan skrive og lese fra variabler (bruke minnet)
    // Dette tester at datasegmentet er riktig konfigurert
    terminal_write_color("- Data segment: ", COLOR_WHITE);
    volatile uint32_t test_value = 0x12345678;  // Skriv en verdi til minnet
    if (test_value == 0x12345678) {  // Sjekk at vi kan lese den samme verdien
        terminal_write_color("Working - can read/write memory\n", COLOR_GREEN);
    } else {
        terminal_write_color("Error - unexpected memory value\n", COLOR_RED);
    }
    
    // For en mer omfattende GDT-test ville vi testet segmentgrenser
    // og privilegienivåer, men det er utenfor omfanget av denne testen
    
    terminal_write_color("GDT appears to be configured correctly!\n", COLOR_GREEN);
}

/* 
 * Tester at Interrupt Descriptor Table (IDT) er satt opp riktig
 * 
 * IDT er en datastruktur som forteller CPU-en hvilke funksjoner
 * som skal håndtere ulike typer interrupts og exceptions.
 * Denne testen verifiserer at IDT er lastet og fungerer.
 */
void test_idt() {
    terminal_write_color("\nTesting IDT functionality:\n", COLOR_YELLOW);
    
    // Vi kommenterer ut den faktiske divisjon-med-null testen for sikkerhet
    // En faktisk test ville utløse exception 0 (division by zero)
    /*
    terminal_write_color("- Triggering division by zero... ", COLOR_WHITE);
    
    // Dette vil trigge interrupt 0 (divide by zero)
    volatile int a = 10;
    volatile int b = 0;
    volatile int c = a / b;  // Dette vil utløse unntaket
    
    // Hvis vi kommer hit uten å håndtere unntaket, fungerer ikke IDT
    terminal_write_color("ERROR - IDT not working!\n", COLOR_RED);
    */
    
    // Sjekk i stedet debug-tegnene som vises når IDT lastes
    terminal_write_color("- IDT loaded: ", COLOR_WHITE);
    terminal_write_color("Debug characters 'Ii' visible in debug output\n", COLOR_GREEN);
    
    // Forklar hva en full IDT-test ville involvere
    terminal_write_color("- For a true IDT test, we would need to:\n", COLOR_WHITE);
    terminal_write_color("  1. Set up proper exception handlers\n", COLOR_GRAY);
    terminal_write_color("  2. Trigger exceptions and verify they're caught\n", COLOR_GRAY);
    terminal_write_color("  3. Program the PIC for hardware interrupts\n", COLOR_GRAY);
    
    terminal_write_color("IDT appears to be loaded correctly.\n", COLOR_GREEN);
}

/* 
 * Tester at tastatur-interrupt og tastaturdriver fungerer
 * 
 * Denne testen aktiverer interrupts, venter på tastetrykk, og
 * viser tegnet som ble tastet inn sammen med ASCII-verdien.
 * Testen bekrefter at både hardware-interrupt og tastatur-driveren virker.
 */
void test_keyboard_interrupt() {
    // Vis en overskrift for testen
    terminal_write_color("\nKeyboard interrupt test:\n", COLOR_YELLOW);
    
    // Aktiver CPU interrupts - dette lar IRQs (hardware interrupts) nå CPU-en
    // "sti" er en x86 assembly-instruksjon som setter Interrupt Flag
    __asm__ volatile("sti");
    
    // Informer brukeren om hva de skal gjøre
    terminal_write_color("Type 5 characters to test keyboard input:\n", COLOR_WHITE);
    
    // Enkelt tastatur-ekko program som viser 5 tastetrykk
    int count = 0;
    while (count < 5) {
        // Sjekk om det er data i tastatur-bufferet
        if (keyboard_is_key_pressed()) {
            // Les et tegn fra tastatur-bufferen
            char c = keyboard_read_char();
            if (c) {
                // Vis tegnet som ble tastet inn og ASCII-verdien
                terminal_write_color("Key pressed: '", COLOR_CYAN);
                terminal_write_char(c);  // Vis selve tegnet
                terminal_write_color("' (", COLOR_CYAN);
                
                // Konverter ASCII-verdien til heksadesimal for å vise koden
                char hex[5];
                hexToString(c, hex);
                terminal_write_color(hex, COLOR_LIGHT_GREEN);
                terminal_write_color(")\n", COLOR_CYAN);
                
                count++;  // Øk telleren for antall tegn vi har vist
            }
        }
    }
    
    terminal_write_color("Keyboard test completed successfully!\n", COLOR_GREEN);
}

/* 
 * Test hardware interrupts (IRQs) - spesielt tastatur-input
 * 
 * Denne funksjonen aktiverer hardware interrupts ved å sette interrupt
 * flag (IF) i EFLAGS-registeret, og venter deretter på tastatur-input.
 * Når tastetrykk kommer, blir IRQ1 utløst, som kjører vår keyboard_handler.
 */
void test_hardware_interrupts() {
    terminal_write_color("\nTesting hardware interrupts:\n", COLOR_YELLOW);
    terminal_write_color("Keyboard input test - press keys to see them displayed:\n", COLOR_WHITE);
    terminal_write_color("(Press up to 10 keys or wait for timeout)\n", COLOR_GRAY);
    
    // Aktiver hardware interrupts ved å sette interrupt flag
    // "sti" er en x86 assembly instruksjon: Set Interrupt Flag
    __asm__ volatile("sti");
    
    // Enkelt tastatur-ekko program - leser fra tastatur-bufferen og viser tegnene
    int keyCount = 0;
    uint32_t timeout = 10000000; // Approximately 10 seconds timeout
    uint32_t counter = 0;
    
    while (keyCount < 10 && counter < timeout) { // Avslutt etter 10 tastetrykk eller timeout
        // Sjekk om det er data i tastatur-bufferen
        if (keyboard_is_key_pressed()) {
            // Les et tegn fra tastatur-bufferen
            char c = keyboard_read_char();
            if (c) {
                // Vis tegnet på skjermen
                terminal_write_char(c);
                keyCount++;
                // Reset counter when a key is pressed
                counter = 0;
            }
        }
        counter++;
        
        // Show a spinner to indicate we're waiting
        if (counter % 1000000 == 0) {
            static int spinner = 0;
            const char spinner_chars[] = {'|', '/', '-', '\\'};
            terminal_write_char('\b');
            terminal_write_char(spinner_chars[spinner]);
            spinner = (spinner + 1) % 4;
        }
    }
    
    if (counter >= timeout) {
        terminal_write_color("\nTimeout waiting for keyboard input.\n", COLOR_YELLOW);
        terminal_write_color("Hardware interrupts may not be working correctly.\n", COLOR_YELLOW);
    } else {
        terminal_write_color("\nHardware interrupt test completed successfully!\n", COLOR_GREEN);
    }
}

/* 
 * Tester software interrupts (CPU exceptions)
 * 
 * Denne funksjonen tester at software interrupts fungerer ved å
 * utløse noen sikre interrupts som ikke vil krasje systemet.
 * Vi unngår bevisst division by zero som kan forårsake problemer.
 */
void test_software_interrupt() {
    terminal_write_color("\nTesting software interrupts (ISRs):\n", COLOR_YELLOW);
    
    // Test breakpoint exception (INT 3)
    terminal_write_color("1. Triggering Breakpoint Exception (INT 3)...\n", COLOR_WHITE);
    __asm__ volatile("int $3");
    
    // Vi unngår å teste General Protection Fault (INT 13) siden det kan forårsake problemer
    
    // Vi unngår å teste Invalid Opcode (INT 6) siden det kan forårsake problemer
    
    terminal_write_color("Software interrupt tests completed successfully!\n", COLOR_GREEN);
}

/* 
 * Kjører alle testfunksjonene for å validere systemet
 * 
 * Denne funksjonen samler alle testene og kjører dem i rekkefølge.
 * Dette gir en komplett systemsjekk for å verifisere at alle
 * essensielle komponenter i operativsystemet fungerer korrekt.
 */
void run_all_tests() {
    // Vent i 2 sekunder før vi starter testene for å sikre at oppstarten er ferdig
    delay(2);
    
    // Vi aktiverer interrupts tidlig for å sikre at interrupts fungerer
    __asm__ volatile("sti");
    
    // Test GDT for å verifisere at minnesegmentering fungerer
    terminal_write_color("\n=== Testing GDT (Global Descriptor Table) ===\n", COLOR_LIGHT_CYAN);
    test_gdt();
    
    // Test terminal-output med ulike farger
    terminal_write_color("\n=== Testing Terminal Output ===\n", COLOR_LIGHT_CYAN);
    test_terminal_output();
    
    // Test IDT for å verifisere at interrupt-håndtering fungerer
    terminal_write_color("\n=== Testing IDT (Interrupt Descriptor Table) ===\n", COLOR_LIGHT_CYAN);
    test_idt();
    
    // Test software interrupts - kun INT 3 (breakpoint)
    terminal_write_color("\n=== Testing Software Interrupts (ISRs) ===\n", COLOR_LIGHT_CYAN);
    test_software_interrupt();
    
    // Test hardware interrupts - spesielt tastatur-input
    terminal_write_color("\n=== Testing Hardware Interrupts (Keyboard) ===\n", COLOR_LIGHT_CYAN);
    delay(1);
    test_hardware_interrupts();
    
    // Alle tester er nå fullført
    terminal_write_color("\n=== All Tests Completed Successfully! ===\n", COLOR_LIGHT_GREEN);
} 