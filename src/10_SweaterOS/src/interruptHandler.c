/**
 * Interrupt Handler Implementation
 * 
 * Dette er en sentral del av operativsystemet som håndterer både CPU exceptions
 * og hardware interrupts. Filen inneholder kode for å:
 * 
 * 1. Initialisere og konfigurere PIC (Programmable Interrupt Controller)
 * 2. Håndtere CPU exceptions (f.eks. division by zero, page fault)
 * 3. Håndtere hardware interrupts (f.eks. tastatur, timer)
 * 4. Implementere I/O-funksjoner for kommunikasjon med hardware
 * 5. Håndtere tastaturinput og konvertere scancodes til ASCII
 */

#include "libc/stdint.h"
#include "interruptHandler.h"
#include "descriptorTables.h"
#include "miscFuncs.h"

/**
 * PIC (Programmable Interrupt Controller) porter og kommandoer
 * 
 * PIC er en hardware-komponent som håndterer interrupts fra ulike enheter
 * og sender dem til CPU-en i prioritert rekkefølge. I x86-arkitekturen
 * bruker vi to 8259A PIC-er koblet sammen (master og slave).
 */
#define PIC1_COMMAND    0x20    // Master PIC kommandoport
#define PIC1_DATA       0x21    // Master PIC dataport
#define PIC2_COMMAND    0xA0    // Slave PIC kommandoport
#define PIC2_DATA       0xA1    // Slave PIC dataport

// Kommandoer for PIC
#define PIC_EOI         0x20    // End of Interrupt kommando - sendes når vi er ferdige med å håndtere et interrupt
#define ICW1_INIT       0x10    // Initialization kommando - starter initialiseringsprosessen
#define ICW1_ICW4       0x01    // ICW4 følger - forteller PIC at vi vil sende ICW4
#define ICW4_8086       0x01    // 8086/88 modus - setter PIC i modus for å fungere med x86 CPU

/**
 * Skriver en byte til en gitt I/O-port
 * 
 * I x86-arkitekturen kommuniserer CPU med periferienheter gjennom et separat
 * I/O-adresserom. Denne funksjonen bruker 'outb' assembly-instruksjonen for å
 * sende data til en spesifisert I/O-port.
 * 
 * @param port I/O-porten vi vil skrive til (0-65535)
 * @param value Verdien vi vil skrive (0-255)
 */
void outb(uint16_t port, uint8_t value) {
    // "a" betyr at value skal plasseres i AL/AX/EAX registeret
    // "Nd" betyr at port er en umiddelbar verdi som kan plasseres i DX registeret
    // "volatile" forteller kompilatoren at denne instruksjonen har sideeffekter
    // og ikke skal optimaliseres bort
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

/**
 * Leser en byte fra en gitt I/O-port
 * 
 * Denne funksjonen bruker 'inb' assembly-instruksjonen for å lese data
 * fra en spesifisert I/O-port. Dette er nødvendig for å motta data fra
 * hardware-enheter som tastatur, disk, osv.
 * 
 * @param port I/O-porten vi vil lese fra (0-65535)
 * @return Verdien som ble lest fra porten (0-255)
 */
uint8_t inb(uint16_t port) {
    uint8_t ret;
    // "=a" betyr at output-verdien skal plasseres i AL/AX/EAX registeret
    // og deretter i variabelen ret
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * En kort forsinkelse, brukes ofte etter I/O-operasjoner
 * 
 * Noen hardware-enheter trenger litt tid for å behandle kommandoer.
 * Denne funksjonen gir en kort forsinkelse ved å utføre en I/O-operasjon
 * til en ubrukt port (0x80). Dette er en standard teknikk i x86-systemer.
 */
void io_wait(void) {
    // Port 0x80 er vanligvis ikke koblet til noen enhet og brukes
    // ofte for I/O-forsinkelse i x86-systemer
    outb(0x80, 0);
}

/**
 * Eksterne referanser til ISR-handlere (Interrupt Service Routines)
 * 
 * Disse funksjonene er definert i interruptServiceRoutines.asm
 * og er de faktiske rutinene som kjøres når et interrupt oppstår.
 * De lagrer CPU-tilstanden, setter opp C-kall-miljøet, og kaller
 * våre C-funksjoner for å håndtere interruptet.
 */

// CPU Exceptions (0-31)
extern void isr0(void);  // Division by Zero - Divisjon med null
extern void isr1(void);  // Debug - Debugging-relatert exception
extern void isr2(void);  // Non-maskable Interrupt - Kan ikke maskeres/ignoreres
extern void isr3(void);  // Breakpoint - Brukt for debugging
extern void isr4(void);  // Overflow - Aritmetisk overflow
extern void isr5(void);  // Bound Range Exceeded - Array-indeks utenfor grenser
extern void isr6(void);  // Invalid Opcode - Ugyldig instruksjon
extern void isr7(void);  // Device Not Available - FPU/MMX/SSE ikke tilgjengelig
extern void isr8(void);  // Double Fault - Feil under håndtering av en annen exception
extern void isr9(void);  // Coprocessor Segment Overrun - Sjelden brukt
extern void isr10(void); // Invalid TSS - Problem med Task State Segment
extern void isr11(void); // Segment Not Present - Segment eksisterer ikke
extern void isr12(void); // Stack-Segment Fault - Problem med stack segment
extern void isr13(void); // General Protection Fault - Minnebeskyttelsesfeil
extern void isr14(void); // Page Fault - Feil ved aksess til en side i minnet
extern void isr15(void); // Reserved - Reservert av Intel
extern void isr16(void); // x87 Floating-Point Exception - FPU-feil
extern void isr17(void); // Alignment Check - Minneaksess ikke riktig justert
extern void isr18(void); // Machine Check - Intern CPU-feil
extern void isr19(void); // SIMD Floating-Point Exception - SSE/AVX-feil
extern void isr20(void); // Reserved - Reservert av Intel
extern void isr21(void); // Reserved - Reservert av Intel
extern void isr22(void); // Reserved - Reservert av Intel
extern void isr23(void); // Reserved - Reservert av Intel
extern void isr24(void); // Reserved - Reservert av Intel
extern void isr25(void); // Reserved - Reservert av Intel
extern void isr26(void); // Reserved - Reservert av Intel
extern void isr27(void); // Reserved - Reservert av Intel
extern void isr28(void); // Reserved - Reservert av Intel
extern void isr29(void); // Reserved - Reservert av Intel
extern void isr30(void); // Reserved - Reservert av Intel
extern void isr31(void); // Reserved - Reservert av Intel

// Hardware Interrupts (IRQs 0-15, mappet til interrupt 32-47)
extern void irq0(void);  // Timer (PIT) - Programmable Interval Timer
extern void irq1(void);  // Keyboard - PS/2 tastatur
extern void irq2(void);  // Cascade for 8259A Slave controller - Kobling til slave PIC
extern void irq3(void);  // COM2 - Serieport 2
extern void irq4(void);  // COM1 - Serieport 1
extern void irq5(void);  // LPT2 - Parallellport 2
extern void irq6(void);  // Floppy Disk - Diskettstasjon
extern void irq7(void);  // LPT1 / Unreliable "spurious" interrupt - Parallellport 1
extern void irq8(void);  // CMOS Real Time Clock - Sanntidsklokke
extern void irq9(void);  // Free for peripherals - Ledig for periferienheter
extern void irq10(void); // Free for peripherals - Ledig for periferienheter
extern void irq11(void); // Free for peripherals - Ledig for periferienheter
extern void irq12(void); // PS2 Mouse - PS/2 mus
extern void irq13(void); // FPU / Coprocessor / Inter-processor - Flyttallsprosessor
extern void irq14(void); // Primary ATA Hard Disk - Primær harddisk
extern void irq15(void); // Secondary ATA Hard Disk - Sekundær harddisk

/**
 * Tastatur-relaterte konstanter og variabler
 * 
 * Tastaturet er en av de viktigste input-enhetene i et OS. Når en tast
 * trykkes eller slippes, sender tastaturet en scancode til CPU-en via
 * IRQ1. Vi må tolke disse scancodes og konvertere dem til ASCII-tegn.
 */
#define KEYBOARD_DATA_PORT 0x60       // I/O-porten hvor tastaturdata kan leses
#define KEYBOARD_BUFFER_SIZE 32       // Størrelsen på vår tastatur-buffer

// Tastatur-buffer implementert som en sirkulær buffer
// Dette lar oss lagre inntastede tegn selv om vi ikke
// behandler dem umiddelbart
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int keyboard_buffer_head = 0;  // Peker til der vi skriver neste tegn
static int keyboard_buffer_tail = 0;  // Peker til der vi leser neste tegn

/**
 * Enkelt US keyboard layout for ASCII-konvertering
 * 
 * Denne tabellen mapper scancodes fra tastaturet til ASCII-tegn.
 * Når tastaturet sender en scancode, bruker vi denne tabellen for
 * å konvertere den til et ASCII-tegn som kan vises på skjermen.
 * 
 * Merk: Dette er en forenklet tabell som bare håndterer grunnleggende
 * tegn og ikke spesialtegn, shift, caps lock, osv.
 */
static const char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0
};

/**
 * Initialiserer PIC (Programmable Interrupt Controller)
 * 
 * I x86-arkitekturen bruker vi to 8259A PIC-er for å håndtere hardware
 * interrupts. Denne funksjonen konfigurerer dem for å fungere med vårt OS.
 * 
 * Hovedoppgavene er:
 * 1. Remappere IRQ-ene til å starte på interrupt 32 (0x20) for å unngå
 *    konflikt med CPU exceptions som bruker 0-31.
 * 2. Konfigurere master/slave-forholdet mellom de to PIC-ene.
 * 3. Sette masker for å aktivere bare de interrupts vi vil ha.
 */
void pic_initialize() 
{
    // ICW1: Start initialization of both PICs
    // Dette forteller begge PIC-ene at vi vil starte initialiseringsprosessen
    // og at vi vil sende ICW4 senere
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();  // Kort forsinkelse for å gi PIC tid til å reagere
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    
    // ICW2: Set base interrupt vectors
    // Dette forteller PIC-ene hvilke interrupt-numre de skal bruke
    outb(PIC1_DATA, 0x20);     // Master PIC starter på 32 (0x20)
    io_wait();
    outb(PIC2_DATA, 0x28);     // Slave PIC starter på 40 (0x28)
    io_wait();
    
    // ICW3: Tell Master PIC that Slave is connected to IRQ2
    // Dette konfigurerer master/slave-forholdet mellom PIC-ene
    outb(PIC1_DATA, 0x04);     // Bit 2 (IRQ2) er hvor Slave er koblet til
    io_wait();
    
    // ICW3: Tell Slave PIC that it's connected to IRQ2 on Master
    outb(PIC2_DATA, 0x02);     // Slave ID er 2
    io_wait();
    
    // ICW4: Set 8086 mode
    // Dette setter PIC-ene i modus for å fungere med x86 CPU
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();
    
    // Set specific masks to enable only what we need
    // 0 = enabled, 1 = disabled for hver bit
    // For tastatur-testing aktiverer vi:
    // - IRQ1 (keyboard)
    // - IRQ2 (cascade for slave PIC - må være aktivert for at slave skal fungere)
    uint8_t master_mask = ~((1 << 1) | (1 << 2));  // Aktiver IRQ1 og IRQ2
    uint8_t slave_mask = 0xFF;  // Deaktiver alle interrupts på slave PIC for nå
    
    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);
    
    terminal_write_color("PIC initialized and remapped to interrupts 32-47\n", COLOR_GREEN);
}

/**
 * Sender End-of-Interrupt (EOI) signal til PIC
 * 
 * Etter at vi har håndtert et hardware interrupt, må vi fortelle PIC
 * at vi er ferdige, slik at den kan sende flere interrupts av samme type.
 * Dette gjøres ved å sende en EOI-kommando til riktig PIC.
 * 
 * @param irq IRQ-nummeret (0-15) som ble håndtert
 */
void pic_send_eoi(uint8_t irq) 
{
    // Hvis IRQ kom fra Slave PIC (IRQ 8-15), må vi sende EOI til begge PIC-ene
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);  // Send EOI til Slave PIC
    }
    
    // Alltid send EOI til Master PIC
    // Dette er nødvendig for alle IRQs, siden Slave PIC er koblet til Master
    outb(PIC1_COMMAND, PIC_EOI);
}

/**
 * Exception handler - håndterer CPU exceptions (interrupt 0-31)
 * 
 * Denne funksjonen kalles fra vår assembly-kode når en CPU exception oppstår.
 * Den håndterer ulike typer exceptions, med spesiell håndtering for division by zero.
 * 
 * @param cpu CPU-tilstanden når exception oppstod (registre)
 * @param int_no Exception-nummeret (0-31)
 * @param stack Stack-tilstanden når exception oppstod (EIP, CS, EFLAGS)
 */
void exception_handler(struct cpu_state cpu, uint32_t int_no, struct stack_state stack) 
{
    // Teller for division by zero exceptions for å unngå uendelige løkker
    static int div_zero_count = 0;
    
    // Special handling for division by zero to prevent infinite loops
    if (int_no == 0) {
        div_zero_count++;
        
        terminal_write_color("\nCPU EXCEPTION: Division By Zero\n", COLOR_LIGHT_RED);
        
        if (div_zero_count == 1) {
            terminal_write_color("Dette er den første division by zero exception.\n", COLOR_YELLOW);
            terminal_write_color("Instruksjonen som forårsaket feilen vil bli hoppet over.\n", COLOR_YELLOW);
        } else {
            terminal_write_color("Exception-teller: ", COLOR_YELLOW);
            terminal_write_decimal(div_zero_count);
            terminal_write("\n");
        }
        
        // Hvis vi får for mange division by zero exceptions, er noe galt
        // Dette kan indikere en uendelig løkke eller en alvorlig feil i koden
        if (div_zero_count >= 3) {
            terminal_write_color("\nFor mange division by zero exceptions. Stopper systemet.\n", COLOR_LIGHT_RED);
            terminal_write_color("Dette kan indikere en uendelig løkke i koden.\n", COLOR_LIGHT_RED);
            terminal_write_color("\nSYSTEM HALTED - CPU Stopped\n", COLOR_LIGHT_RED);
            
            // Stopp CPU-en ved å gå inn i en uendelig løkke
            // cli = Clear Interrupt Flag (deaktiver interrupts)
            // hlt = Halt (stopp CPU-en til neste interrupt)
            __asm__ volatile("cli; hlt");
        }
        
        // Merk: Vi returnerer her og lar assembly-koden hoppe over
        // instruksjonen som forårsaket division by zero
        return;
    }
    
    // Handle other CPU exceptions
    terminal_write_color("\nCPU EXCEPTION: ", COLOR_LIGHT_RED);
    
    // Vis hvilken exception som oppstod
    switch (int_no) {
        case 1:  terminal_write_color("Debug Exception", COLOR_LIGHT_RED); break;
        case 2:  terminal_write_color("Non-maskable Interrupt", COLOR_LIGHT_RED); break;
        case 3:  terminal_write_color("Breakpoint", COLOR_LIGHT_RED); break;
        case 4:  terminal_write_color("Overflow", COLOR_LIGHT_RED); break;
        case 5:  terminal_write_color("Bound Range Exceeded", COLOR_LIGHT_RED); break;
        case 6:  terminal_write_color("Invalid Opcode", COLOR_LIGHT_RED); break;
        case 7:  terminal_write_color("Device Not Available", COLOR_LIGHT_RED); break;
        case 8:  terminal_write_color("Double Fault", COLOR_LIGHT_RED); break;
        case 9:  terminal_write_color("Coprocessor Segment Overrun", COLOR_LIGHT_RED); break;
        case 10: terminal_write_color("Invalid TSS", COLOR_LIGHT_RED); break;
        case 11: terminal_write_color("Segment Not Present", COLOR_LIGHT_RED); break;
        case 12: terminal_write_color("Stack-Segment Fault", COLOR_LIGHT_RED); break;
        case 13: terminal_write_color("General Protection Fault", COLOR_LIGHT_RED); break;
        case 14: terminal_write_color("Page Fault", COLOR_LIGHT_RED); break;
        case 16: terminal_write_color("x87 Floating-Point Exception", COLOR_LIGHT_RED); break;
        case 17: terminal_write_color("Alignment Check", COLOR_LIGHT_RED); break;
        case 18: terminal_write_color("Machine Check", COLOR_LIGHT_RED); break;
        case 19: terminal_write_color("SIMD Floating-Point Exception", COLOR_LIGHT_RED); break;
        default: terminal_write_color("Reserved/Unknown", COLOR_LIGHT_RED); break;
    }
    
    terminal_write_color("\n", COLOR_LIGHT_RED);
    
    // Vis mer informasjon om exception
    terminal_write_color("Error Code: ", COLOR_YELLOW);
    terminal_write_hex(stack.error_code);
    terminal_write_color("\nEIP: ", COLOR_YELLOW);
    terminal_write_hex(stack.eip);
    terminal_write_color(" (Instruction Pointer)\n", COLOR_GRAY);
    
    // For alvorlige exceptions, stopp systemet
    if (int_no == 8 || int_no == 13 || int_no == 14) {
        terminal_write_color("\nThis is a critical exception. System halted.\n", COLOR_LIGHT_RED);
        __asm__ volatile("cli; hlt");
    } else {
        terminal_write_color("\nException handled. Continuing execution.\n", COLOR_GREEN);
    }
}

/**
 * IRQ handler - håndterer hardware interrupts (IRQ 0-15)
 * 
 * Denne funksjonen kalles fra vår assembly-kode når et hardware interrupt oppstår.
 * Den identifiserer hvilken type IRQ det er og kaller riktig handler-funksjon.
 * 
 * @param cpu CPU-tilstanden når interrupt oppstod (registre)
 * @param int_no Interrupt-nummeret (32-47 for IRQ 0-15)
 * @param stack Stack-tilstanden når interrupt oppstod (EIP, CS, EFLAGS)
 */
void irq_handler(struct cpu_state cpu, uint32_t int_no, struct stack_state stack) 
{
    // Konverter interrupt-nummer til IRQ-nummer
    uint8_t irq = int_no - 32;
    
    // Håndter ulike typer IRQs
    switch (irq) {
        case 0:  // Timer (PIT)
            // For nå gjør vi ingenting med timer-interrupts
            break;
            
        case 1:  // Keyboard
            keyboard_handler();
            break;
            
        // Andre IRQs kan legges til her etter behov
            
        default:
            // For ukjente IRQs, bare vis en melding
            terminal_write_color("Received IRQ: ", COLOR_YELLOW);
            terminal_write_decimal(irq);
            terminal_write_color("\n", COLOR_YELLOW);
            break;
    }
    
    // Send End-of-Interrupt signal til PIC
    // Dette er viktig for at PIC skal sende flere interrupts av samme type
    pic_send_eoi(irq);
}

/**
 * Tastatur-handler - håndterer tastatur-interrupts (IRQ 1)
 * 
 * Denne funksjonen kalles når et tastatur-interrupt oppstår.
 * Den leser scancode fra tastaturet, konverterer det til ASCII,
 * og legger tegnet i tastatur-bufferen.
 */
void keyboard_handler() 
{
    // Les scancode fra tastaturet
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    
    // Ignorer key-release events (når en tast slippes)
    // Disse har bit 7 satt (scancode >= 128)
    if (scancode >= 128) {
        return;
    }
    
    // Konverter scancode til ASCII hvis mulig
    if (scancode < sizeof(scancode_to_ascii) && scancode_to_ascii[scancode] != 0) {
        char ascii = scancode_to_ascii[scancode];
        
        // Legg tegnet i tastatur-bufferen
        keyboard_buffer[keyboard_buffer_head] = ascii;
        keyboard_buffer_head = (keyboard_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
        
        // Hvis bufferen er full, overskriv eldste tegn
        if (keyboard_buffer_head == keyboard_buffer_tail) {
            keyboard_buffer_tail = (keyboard_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
        }
        
        // Vis tegnet på skjermen
        terminal_putchar(ascii);
    }
}

/**
 * Leser et tegn fra tastatur-bufferen
 * 
 * Denne funksjonen returnerer neste tegn fra tastatur-bufferen,
 * eller 0 hvis bufferen er tom.
 * 
 * @return Neste tegn fra tastatur-bufferen, eller 0 hvis tom
 */
char keyboard_getchar() 
{
    // Hvis bufferen er tom, returner 0
    if (keyboard_buffer_head == keyboard_buffer_tail) {
        return 0;
    }
    
    // Les neste tegn fra bufferen
    char c = keyboard_buffer[keyboard_buffer_tail];
    keyboard_buffer_tail = (keyboard_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    
    return c;
}

/**
 * Sjekker om det er tegn tilgjengelig i tastatur-bufferen
 * 
 * @return 1 hvis det er tegn tilgjengelig, 0 ellers
 */
int keyboard_data_available() 
{
    return keyboard_buffer_head != keyboard_buffer_tail;
}

/**
 * Initialiserer interrupt-håndtering
 * 
 * Denne funksjonen setter opp PIC og aktiverer interrupts.
 * Dette er nødvendig for at hardware-interrupts skal fungere.
 */
void interrupt_initialize() 
{
    // Initialiser PIC
    pic_initialize();
    
    // Aktiver interrupts (sti = Set Interrupt Flag)
    __asm__ volatile("sti");
    
    terminal_write_color("Interrupts enabled\n", COLOR_GREEN);
} 