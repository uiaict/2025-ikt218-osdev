#include "libc/stdint.h"      // Inkluderer standard integer-typer som uint8_t og uint16_t
#include "miscFuncs.h"    // Inkluderer headerfilen med funksjonene vi definerer her
#include "descriptorTables.h"
#include "interruptHandler.h"

// Multiboot2 magic number - dette er verdien multiboot2-kompatible bootloadere
// vil sende som parameter til kernel_main()
#define MULTIBOOT2_MAGIC 0x36d76289

// Adressen til VGA-tekstbufferen i minnet (fast for tekstmodus)
#define VGA_ADDRESS 0xB8000

// Standard VGA-tekstmodus har 80 kolonner og 25 rader
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Peker til VGA-minnet, hvor vi kan skrive tegn som vises på skjermen
static uint16_t* terminal_buffer = (uint16_t*)VGA_ADDRESS;

// Variabler for å holde styr på hvor vi skal skrive neste tegn
static uint8_t terminal_row = 0, terminal_column = 0;

// Standard farge for tekst (hvit på svart bakgrunn)
static uint8_t terminal_color = COLOR_WHITE;

/**
 * Initialiserer terminalen ved å fylle hele skjermen med blanke tegn (' ')
 * og sette standardfargen til hvit.
 */
void terminal_initialize() {
    // Sikre at terminal_buffer er korrekt
    if (terminal_buffer == 0) {
        terminal_buffer = (uint16_t*)VGA_ADDRESS;
    }
    
    // Gå gjennom hver rad og kolonne og fyll skjermen med mellomrom
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            const int index = y * VGA_WIDTH + x;
            if (index >= 0 && index < (VGA_WIDTH * VGA_HEIGHT)) {
                terminal_buffer[index] = (COLOR_WHITE << 8) | ' ';
            }
        }
    }

    // Nullstill posisjon og farge
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = COLOR_WHITE;
}

/**
 * Scrolls the terminal one line up
 * Flytter alt innhold en linje opp og tømmer nederste linje
 */
void terminal_scroll() {
    // Flytt alle linjer en linje opp
    for (int y = 0; y < VGA_HEIGHT - 1; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            // Kopier fra linjen under
            terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
        }
    }
    
    // Tøm nederste linje
    for (int x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (terminal_color << 8) | ' ';
    }
}

/**
 * Skriver ett tegn til skjermen på gjeldende posisjon.
 * Hvis tegnet er '\n', går vi til neste linje.
 */
void terminal_write_char(char c) {
    if (c == '\n') { // Hvis vi får et linjeskift, flytt til neste linje
        terminal_row++;
        terminal_column = 0;
        
        // Sjekk om vi trenger å scrolle
        if (terminal_row >= VGA_HEIGHT) {
            terminal_scroll();
            terminal_row = VGA_HEIGHT - 1;
        }
        return;
    }
    
    // Sørg for at vi er innenfor skjermstørrelsen
    if (terminal_row >= VGA_HEIGHT || terminal_column >= VGA_WIDTH) {
        return; // For sikkerhet - ikke skriv utenfor skjermområdet
    }
    
    // Beregn posisjonen i VGA-minnet
    const int index = terminal_row * VGA_WIDTH + terminal_column;
    
    // Sjekk at indeksen er innenfor gyldig område
    if (index >= 0 && index < (VGA_WIDTH * VGA_HEIGHT)) {
        // Sett tegnet med gjeldende farge i VGA-minnet
        terminal_buffer[index] = (terminal_color << 8) | c;
    }
    
    // Gå til neste posisjon
    terminal_column++;
    
    // Hvis vi har nådd slutten av linjen, gå til neste linje
    if (terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
        
        // Sjekk om vi trenger å scrolle
        if (terminal_row >= VGA_HEIGHT) {
            terminal_scroll();
            terminal_row = VGA_HEIGHT - 1;
        }
    }
}

/**
 * Skriver en hel streng til skjermen, tegn for tegn.
 */
void terminal_write(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        terminal_write_char(str[i]);
    }
}

/**
 * Skriver ut en tekststreng med en spesifisert farge
 * 
 * str er tekststrengen som skal skrives ut
 * color er fargen som skal brukes (definert i miscFuncs.h)
 */
void terminal_write_color(const char* str, VGA_COLOR color) {
    // Lagre den nåværende fargen
    uint8_t original_color = terminal_color;
    
    // Sett ny farge
    terminal_color = color;
    
    // Skriv ut teksten med den nye fargen
    terminal_write(str);
    
    // Gjenopprett den opprinnelige fargen
    terminal_color = original_color;
}

/**
 * Konverterer et tall til en heksadesimal streng.
 * Resultatet blir lagret i str-parameteren, som må ha plass til minst 11 tegn.
 */
void hexToString(uint32_t num, char* str) {
    const char* hex_chars = "0123456789ABCDEF";
    str[0] = '0';
    str[1] = 'x';
    
    for (int i = 0; i < 8; i++) {
        str[9-i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    str[10] = '\0';
}

/**
 * En mer nøyaktig forsinkelsesrutine
 * Bruker en enkel loop for å forsinke utførelsen av koden
 * 
 * ms er omtrentlig antall millisekunder å vente
 * 
 * Merk: Dette er ikke en nøyaktig timing, men er nyttig for debugging
 */
void delay(uint32_t ms) {
    // Multipliser med en faktor for å gi ca. riktig forsinkelse
    // Denne verdien må kanskje justeres basert på maskinvaren
    volatile uint32_t large_number = ms * 100000;
    
    // Enkel telleloop
    while (large_number--) {
        // Gjør ingenting, bare tell ned
        __asm__ volatile ("nop");
    }
}

/**
 * Stopper CPU fullstendig - brukes i kritiske feilsituasjoner
 */
void halt() {
    terminal_write_color("\n\nSYSTEM HALTED - CPU Stopped\n", COLOR_LIGHT_RED);
    
    // Deaktiver interrupts og halt CPU
    __asm__ volatile("cli; hlt");
    
    // For sikkerhets skyld, hvis hlt ikke fungerer
    while (1) {
        __asm__ volatile("nop");
    }
}

/**
 * Verifiserer at magisk nummer fra bootloaderen er korrekt og viser resultat.
 * Dette er en sikkerhetskontroll for å bekrefte at OS-en ble startet av en multiboot2-kompatibel bootloader.
 */
void verify_boot_magic(uint32_t magic) {
    char magic_str[11];
    hexToString(magic, magic_str);
    
    // Vis magic number og indiker om det er korrekt
    terminal_write_color("Magic: ", COLOR_WHITE);
    if (magic == MULTIBOOT2_MAGIC) {
        terminal_write_color(magic_str, COLOR_GREEN);
        terminal_write_color(" (Correct)", COLOR_GREEN);
    } else {
        terminal_write_color(magic_str, COLOR_RED);
        terminal_write_color(" (Error - expected 0x36d76289)", COLOR_RED);
    }
    terminal_write_char('\n');
}

/**
 * Initialiserer systemets grunnleggende komponenter som GDT og IDT.
 * Dette setter opp segmentering og interrupt-håndtering, som er nødvendig for operativsystemet.
 */
void initialize_system() {
    // Initialiser Global Descriptor Table (GDT)
    // GDT definerer minneområder (segmenter) og deres rettigheter
    terminal_write_color("Initializing GDT... ", COLOR_WHITE);
    initializer_GDT();
    terminal_write_color("DONE\n", COLOR_GREEN);
    
    // Initialize interrupt system (IDT, PIC, and enable interrupts)
    terminal_write_color("Initializing interrupt system... ", COLOR_WHITE);
    interrupt_initialize();
    terminal_write_color("DONE\n", COLOR_GREEN);
    
    // Vis bekreftelse på at systeminitialisering er fullført
    terminal_write_char('\n');
    terminal_write_color("System initialization complete.\n", COLOR_LIGHT_CYAN);
}

/**
 * Skriver ut et heksadesimalt tall til terminalen
 * 
 * num er tallet som skal skrives ut i heksadesimalt format
 */
void terminal_write_hex(uint32_t num) {
    // Skriver ut '0x' prefiks
    terminal_write_char('0');
    terminal_write_char('x');
    
    // Sjekk om tallet er null
    if (num == 0) {
        terminal_write_char('0');
        return;
    }
    
    // Buffer for å lagre heksadesimale tegn
    char hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', 
                          '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    
    // Vi trenger å håndtere opptil 8 siffer (32-bit heksadesimalt)
    char buffer[9]; // 8 siffer + null-terminator
    buffer[8] = '\0';
    
    // Fyll bufferen bakfra
    int i = 7;
    while (num > 0 && i >= 0) {
        buffer[i] = hex_chars[num & 0xF]; // Hent de 4 minst signifikante bit
        num >>= 4; // Skift 4 bit til høyre
        i--;
    }
    
    // Skriv ut fra første gyldige tegn
    terminal_write_string(&buffer[i + 1]);
}

/**
 * Skriver ut et desimalt tall til terminalen
 * 
 * num er tallet som skal skrives ut i desimalt format
 */
void terminal_write_decimal(uint32_t num) {
    // Sjekk om tallet er null
    if (num == 0) {
        terminal_write_char('0');
        return;
    }
    
    // Buffer for å lagre desimale tegn
    char buffer[11]; // 32-bit tall kan ha opptil 10 siffer + null-terminator
    buffer[10] = '\0';
    
    // Fyll bufferen bakfra
    int i = 9;
    while (num > 0 && i >= 0) {
        buffer[i] = '0' + (num % 10); // Hent siste siffer
        num /= 10; // Fjern siste siffer
        i--;
    }
    
    // Skriv ut fra første gyldige tegn
    terminal_write_string(&buffer[i + 1]);
}

/**
 * Skriver ut en tekststreng
 * 
 * str er tekststrengen som skal skrives ut med standard tekstfarge
 */
void terminal_write_string(const char* str) {
    terminal_write(str);
}