#include "keyboard.h"
#include "../io/printf.h"
#include "libc/stdint.h"
#include "../utils/utils.h"
#include "../idt/idt.h"
#include "libc/stdbool.h"

//tab og esc?

bool capsEnabled = false;
bool shiftEnabled = false;
char terminalBuffer[250];
int index = 0;
 //[0x1b] = '^',
    //[0x2B] = '\'',
//[0x56] = '<',

//mangler æøå og ¨ og 
const char smallAscii[] = {
    
   

    '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '\\', '\016', 
    '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'å', '¨', '\034',
    '?', 'a', 's', 'd', 'f', 'g','h', 'j', 'k', 'l', 'ø', 'æ','\'',
    '?','<','z','x','c','v','b','n','m',',','.','-','?','?','?',' '

};
//mangler ¤, ^ og ÆØÅ 
//[0x1b] = '¨',
    //[0x2b] = '*',
    //[0X5] = '¤',
//[0x56] = '>',
const char capsAscii[] ={
    
    
    '?', '?', '!', '"', '#', '¤', '%', '&','/', '(', ')', '=', '?', '`', '\016', 
    '?', 'Q', 'W', 'E', 'R', 'T', 'Y','U', 'I', 'O', 'P', 'Å', '^', '\034',
    '?','A','S','D','F','G','H','J','K','L','Ø','Æ','*',
    '?','>','Z','X','C','V','B','N','M',';',':', '_', '?', '?'
};

//These ASCII tables are heavily based upon the ones from the solution guide, with some modifications.




void toggle_caps_lock(){
    capsEnabled = !capsEnabled;  
}


char scancode_to_ascii(unsigned char scancode){
    
    unsigned char scancode_without_bit7 = scancode & 0x7F; // Fjerner bit 7 for å få den faktiske scancode uten status på om det er tastetrykk eller slipp
    if (scancode_without_bit7 < sizeof(smallAscii)){
        return (capsEnabled || shiftEnabled)  ? capsAscii[scancode_without_bit7]:smallAscii[scancode_without_bit7];
    }
    return 0;
    }

unsigned char* read_keyboard_data_from_buffer(void)
{
    static unsigned char scancode;
    scancode = inPortB(0x60);
    return &scancode;
}


int check_keyboard_errors(unsigned char* scancode)
{
    
    unsigned char index = *scancode & 0x7F;
    if (index >= sizeof(smallAscii) && index != 0x3A) { // 0x3A er CAPS LOCK
        mafiaPrint("Scancode outside valid area: 0x%x\n", *scancode);
        return KEYBOARD_ERROR;
    }
    return KEYBOARD_OK;
}



int get_keyboard_event_type(unsigned char* scancode)
{
    return (*scancode & 0x80) ? KEY_RELEASE : KEY_PRESS;
    
}

void log_key_press(char input){
    mafiaPrint("pressed key: %c \n", input);
}

void log_buffer(char terminalBuffer[], int* index) {
    mafiaPrint("Current buffer contents: ");
    for (int i = 0; i < *index; i++) {
        mafiaPrint("%c", terminalBuffer[i]);
    }
    mafiaPrint("\n");
}




void handle_key_press(unsigned char* scancode, char terminalBuffer[], int* index)
{
    //mafiaPrint("Scancode: 0x%x\n", *scancode);
    //*scancode = read_keyboard_data_from_buffer();
    char keyValue = scancode_to_ascii(*scancode);

    switch (*scancode)
    {
    case 0x1b: //ESC
       //IKKE IMPLEMENTERT ENDA

    case 0x28: //ENTER - FUNGERER IKKE HELT RIKTIG ENDA
    terminalBuffer[*index] = '\n';
         (*index)++;
         break;

    case 0X0E: //BACKSPACE
        if (*index != 0){
            (*index)--;
            terminalBuffer[*index] = 0;
        }
        break;
    
    case 0X39: // SPACE
        terminalBuffer[*index] = ' ';
        (*index)++;
        break;

    case 0x0F: // TAB
    for (int i =0; i < 4; i++){
            terminalBuffer[*index] = ' ';
            (*index)++;
        }
        break;

    case 0x2A: //LSHIFT
    case 0x36: //RSHIFT
        shiftEnabled = true;
        break;

    case 0X3A: //CAPS LOCK
        toggle_caps_lock();
        break;
    
    default:
    
    if (*index < 250){
        terminalBuffer[*index] = keyValue;
        (*index)++; 
    } 
    else{
        *index = 0;
        terminalBuffer[*index] = keyValue;
        (*index)++;
    }
        //log_key_press(keyValue); //logger kun tastetrykk
        break;
    }
    //log_buffer(terminalBuffer,keyValue);
    
}


void handle_key_release(unsigned char* scancode)
{
    unsigned char scancode_without_bit7 = *scancode & 0x7F;
    //mafiaPrint("Key released: 0x%x\n", *scancode);  // Debug utskrift
    switch (scancode_without_bit7)
    {
        case 0x2A: //LSHIFT
        case 0x36: //RSHIFT
            shiftEnabled = false;
            break;
    default:
        break;
    }
}


/*
// Funksjon for å varsle operativsystemet om en tastaturhendelse
void notify_os_keyboard_event(void)
{
    // 1. Utskriv en melding eller send et signal til operativsystemet om at en tastaturhendelse har funnet sted.
    // 2. Dette kan være en enkel utskrift (f.eks. `mafiaPrint`) eller en mer kompleks handling som å sende en interrupt eller en hendelsessignal.
}
*/





// Tastaturens ISR (Interrupt Service Routine)
void keyboard_isr(struct InterruptRegisters* regs)
{
    //mafiaPrint("Keyboard ISR triggered\n");
    unsigned char* input = read_keyboard_data_from_buffer();

    if (check_keyboard_errors(input) == KEYBOARD_ERROR){
        return; 
    }

    int event_type = get_keyboard_event_type(input);

    if (event_type == KEY_PRESS){
        handle_key_press(input, terminalBuffer, &index);  // Håndter tastetrykk.
    }
    else if (event_type == KEY_RELEASE){
        handle_key_release(input);  
    }

    // 6. Varsle operativsystemet om hendelsen.
    //notify_os_keyboard_event();
}

// Funksjon for å initialisere tastaturinnstillinger eller ISR
void initKeyboard(void)
{
    uint8_t mask = inPortB(0x21);
    mask &= ~(1 << 1);
    outPortB(0x21, mask);        // Skriv tilbake til PIC

    irq_install_handler(1, keyboard_isr); // Installer ISR for IRQ1

    mafiaPrint("Keyboard initialized\n");

}
