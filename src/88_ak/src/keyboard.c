#include "keyboard.h"

bool capsEnabled = false;
bool shiftEnabled = false;
bool ctrlEnabled   = false;
bool altEnabled    = false;

char terminalBuffer[250];
int index = 0;

const char smallAscii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '\\', 0x0E,
    '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', (char)0x86, (char)0xF8, 0x1C,
    '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', (char)0x94, (char)0x91, '\'',
    '?', '<', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', '>', '?', '?', ' '};

const char capsAscii[] = {
    '?', '?', '!', '"', '#', (char)0xA4, '%', '&', '/', '(', ')', '=', '?', '`', '\016',
    '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', (char)0x8F, '^', (char)0x1C,
    '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', (char)0x99, (char)0x92, '*',
    '?', '>', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', '?', '?'};
// Basert på ASCII tables fra solution guide

void toggle_caps_lock() {
    capsEnabled = !capsEnabled;
}

char scancode_to_ascii(unsigned char scancode) {
    unsigned char scancode_without_bit7 = scancode & 0x7F;
    if (scancode_without_bit7 < sizeof(smallAscii)) {
        return (capsEnabled || shiftEnabled) ? capsAscii[scancode_without_bit7] : smallAscii[scancode_without_bit7];
    }
    return 0;
}

unsigned char *read_keyboard_data_from_buffer(void) {
    static unsigned char scancode;
    scancode = inPortB(0x60);
    return &scancode;
}

int check_keyboard_errors(unsigned char *scancode) {
    unsigned char index = *scancode & 0x7F;
    if (index >= sizeof(smallAscii) && index != 0x3A) { // 0x3A er CAPS LOCK
        Print("Scancode outside valid area: 0x%x\n", *scancode);
        return KEYBOARD_ERROR;
    }
    return KEYBOARD_OK;
}

int get_keyboard_event_type(unsigned char *scancode) {
    return (*scancode & 0x80) ? KEY_RELEASE : KEY_PRESS;
}

void log_key_press(char input) {
    Print("%c", input);
}

void log_buffer(char terminalBuffer[], int *index) {
    Print("Current buffer contents: ");
    for (int i = 0; i < *index; i++) {
        Print("%c", terminalBuffer[i]);
    }
    Print("\n");
}

void handle_key_press(unsigned char *scancode, char terminalBuffer[], int *index) {   
    unsigned char code = *scancode & 0x7F;
    
    // Håndter modifier-taster først
    switch (code) {
        case 0x1D:  // Ctrl ned
            ctrlEnabled = true;
            return;
        case 0x38:  // Alt ned
            altEnabled = true;
            return;
        // (la Enter, Backspace osv. gå videre)
        default:
            break;
    }

    char keyValue = scancode_to_ascii(*scancode);

    switch (*scancode) {
    case 0x1b: // ESC
               // IKKE IMPLEMENTERT

    case 0x1C: // ENTER
        terminalBuffer[*index] = '\n';
        (*index)++;
        break;

    case 0X0E: // BACKSPACE
        if (*index != 0) {
            (*index)--;
            terminalBuffer[*index] = 0;
        }
        break;

    case 0X39: // SPACE
        terminalBuffer[*index] = ' ';
        (*index)++;
        break;

    case 0x0F: // TAB
        for (int i = 0; i < 4; i++) {
            terminalBuffer[*index] = ' ';
            (*index)++;
        }
        break;

    case 0x2A: // LSHIFT
    case 0x36: // RSHIFT
        shiftEnabled = true;
        break;

    case 0X3A: // CAPS LOCK
        toggle_caps_lock();
        break;

    default:

        if (*index < 250) {
            terminalBuffer[*index] = keyValue;
            (*index)++;
        } else {
            *index = 0;
            terminalBuffer[*index] = keyValue;
            (*index)++;
        }
        break;
    }
}

void handle_key_release(unsigned char *scancode) {
    unsigned char code = *scancode & 0x7F;
    switch (code) {
        case 0x2A:  // LSHIFT opp
        case 0x36:  // RSHIFT opp
            shiftEnabled = false;
            break;
        case 0x1D:  // Ctrl opp
            ctrlEnabled = false;
            break;
        case 0x38:  // Alt opp
            altEnabled = false;
            break;
        default:
            break;
    }
}

void keyboard_isr(struct InterruptRegisters *regs) {
    unsigned char *input = read_keyboard_data_from_buffer();

    if (check_keyboard_errors(input) == KEYBOARD_ERROR) {
        return;
    }
    int event_type = get_keyboard_event_type(input);

    if (event_type == KEY_PRESS) {
        handle_key_press(input, terminalBuffer, &index);
    }
    else if (event_type == KEY_RELEASE) {
        handle_key_release(input);
    }
    if (regs->int_no >= 40) {
        outPortB(0xA0, 0x20); // Slave PIC EOI (IRQ 8-15)
    }
    outPortB(0x20, 0x20);
}

void init_keyboard(void) {
    uint8_t mask = inPortB(0x21); // Les masken fra PIC1
    mask &= ~(1 << 1);            // Deaktiver IRQ1
    outPortB(0x21, mask);         // Skriv tilbake til PIC

    irq_install_handler(1, keyboard_isr); // Installer ISR for IRQ1

    Print("Keyboard initialized\n");
}

bool keyboard_buffer_empty() {
    return (index == 0); // Sjekk at index er 0 og buffer tomt
}

char read_from_keyboard_buffer() {
    if (index > 0) {
        char input = terminalBuffer[0]; // Ta første tegn
        
        for (int i = 0; i < index - 1; i++) {
            terminalBuffer[i] = terminalBuffer[i + 1];
        }
        index--; // Reduser index
        return input;
    }
    return 0;
}