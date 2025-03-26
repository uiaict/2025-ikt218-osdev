#include "ISR.h"
#include "../drivers/screen.h" // //må huske å korrigere til rett fil referanse 
#include "../kernel/util.h"   // her også

//array for å holde funksjon til hver interrupt (maks 256 som vi definerte til header filen) 
static interrupt_listener_t listeners[MAX_INTERRUPTS][MAX_LISTENERS_PER_ISR];

// Array for globale interrupt-lyttere
static interrupt_listener_t global_listeners[MAX_LISTENERS_PER_ISR];

//Initialiserer (nullstiller) handlers mha. en for-løkke. 
void isr_init() {
    for (int i = 0; i < MAX_INTERRUPTS; i++)
    {for (int j = 0; j < MAX_LISTENERS_PER_ISR; j++) {listeners[i][j] = NULL;}}
    
    for (int i = 0; i < MAX_LISTENERS_PER_ISR; i++)
    {global_listeners[i] = NULL; // Bruk NULL for å indikere NULL listener}
}

// Registrerer en interrupt listener til ein interrupt (spesifikt)
void subscribe_interrupt(uint8_t interrupt_number, interrupt_listener_t handler) {
    for (int i = 0; i < MAX_LISTENERS_PER_ISR; i++) {
        //leter etter en ledig plass for listeneren
        if (listeners[interrupt_number][i] == NULL)
        {listeners[interrupt_number][i] = handler;return;}
    }

    // Skriver feilmelding for dersom den ikke finner en ledig plass for listeneren i for-løkka over.
    kprint("ISR-ERROR: There are too many listeners for interrupt. ");
    char s[4];
    int_to_ascii(interrupt_number, s);
    kprint(s);
    kprint("\n");
}

// Registrerer en global interrupt listeneur
void subscribe_global(interrupt_listener_t handler) {

    // Prøver å finne en ledig plass for den globale listeneren (samme som over)
    for (int i = 0; i < MAX_LISTENERS_PER_ISR; i++)
    {if(global_listeners[i] == NULL) {global_listeners[i] = handler;return;}}

    // feilmelding for ikke-funnet listener
    kprint("ISR ERROR: Too many global listeners\n");}

// Utfører alle registrerte lyttere for en gitt interrupt
void isr_dispatch(registers_t* regs) {
    // Utfør spesifikke interrupt-lyttere
    for (int i = 0; i < MAX_LISTENERS_PER_ISR; i++)
    {if (listeners[regs->int_no][i] != NULL){listeners[regs->int_no][i](regs);}}

    // Utfør globale interrupt-lyttere
    for (int i = 0; i < MAX_LISTENERS_PER_ISR; i++)
    {if (global_listeners[i] != NULL) {global_listeners[i](regs);}}
}