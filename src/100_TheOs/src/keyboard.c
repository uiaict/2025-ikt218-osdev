#include "interrupts.h"
#include "libc/system.h"
#include "libc/system.h"

extern void terminal_printf(const char* format, ...);

// Constants for keyboard input
#define CHAR_NONE 0
#define CHAR_ENTER 2
#define CHAR_SPACE 3
#define CHAR_BACKSPACE 8

// Keyboard buffer
#define KEYBOARD_BUFFER_SIZE 256
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_pos = 0;

// Command count for system info
static int command_count = 0;

// Almost all Scan codes
typedef enum {
    KEY_ESC = 1,
    KEY_1 = 2,
    KEY_2 = 3,
    KEY_3 = 4,
    KEY_4 = 5,
    KEY_5 = 6,
    KEY_6 = 7,
    KEY_7 = 8,
    KEY_8 = 9,
    KEY_9 = 10,
    KEY_0 = 11,
    KEY_DASH = 12,
    KEY_EQUALS = 13,
    KEY_BACKSPACE = 14,
    KEY_TAB = 15,
    KEY_Q = 16,
    KEY_W = 17,
    KEY_E = 18,
    KEY_R = 19,
    KEY_T = 20,
    KEY_Y = 21,
    KEY_U = 22,
    KEY_I = 23,
    KEY_O = 24,
    KEY_P = 25,
    KEY_LBRACKET = 26,
    KEY_RBRACKET = 27,
    KEY_ENTER = 28,
    KEY_CTRL = 29,
    KEY_A = 30,
    KEY_S = 31,
    KEY_D = 32,
    KEY_F = 33,
    KEY_G = 34,
    KEY_H = 35,
    KEY_J = 36,
    KEY_K = 37,
    KEY_L = 38,
    KEY_SIMICOLON = 39,
    KEY_THEN = 40,
    KEY_GRAVE = 41,
    KEY_LSHIFT = 42,
    KEY_BSLASH = 43,
    KEY_Z = 44,
    KEY_X = 45,
    KEY_C = 46,
    KEY_V = 47,
    KEY_B = 48,
    KEY_N = 49,
    KEY_M = 50,
    KEY_COMMA = 51,
    KEY_PERIOD = 52,
    KEY_FSLASH = 53,
    KEY_RSHIFT = 54,
    KEY_PRTSC = 55,
    KEY_ALT = 56,
    KEY_SPACE = 57,
    KEY_CAPS = 58,
    KEY_F1 = 59,
    KEY_F2 = 60,
    KEY_F3 = 61,
    KEY_F4 = 62,
    KEY_F5 = 63,
    KEY_F6 = 64,
    KEY_F7 = 65,
    KEY_F8 = 66,
    KEY_F9 = 67,
    KEY_F10 = 68,
    KEY_NUM = 69,
    KEY_SCROLL = 70,
    KEY_HOME = 71,
    KEY_UP = 72,
    KEY_PGUP = 73,
    KEY_MINUS = 74,
    KEY_LEFT = 75,
    KEY_CENTER = 76,
    KEY_RIGHT = 77,
    KEY_PLUS = 78,
    KEY_END = 79,
    KEY_DOWN = 80,
    KEY_PGDN = 81,
    KEY_INS = 82,
    KEY_DEL = 83,
} scan_code;

static bool capsEnabled = false;
static bool shiftEnabled = false;
char scancode_to_ascii(unsigned char scancode) {  
    if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT) {
        shiftEnabled = true;
        return CHAR_NONE;
    }
    
    switch (scancode){
       case KEY_CAPS:
            capsEnabled = !capsEnabled;
            return CHAR_NONE;

       case KEY_ENTER:
            return CHAR_ENTER;

       case KEY_SPACE:
            return ' ';
            
       case KEY_BACKSPACE:
            return CHAR_BACKSPACE;

       case KEY_END:
       case KEY_DOWN:
       case KEY_PGDN:
       case KEY_INS:
       case KEY_DEL:
       case KEY_LEFT:
       case KEY_CENTER:
       case KEY_RIGHT:
       case KEY_F1:
       case KEY_F2:
       case KEY_F3:
       case KEY_F4:
       case KEY_F5:
       case KEY_F6:
       case KEY_F7:
       case KEY_F8:
       case KEY_F9:
       case KEY_F10:
       case KEY_NUM:
       case KEY_SCROLL:
       case KEY_HOME:
       case KEY_UP:
       case KEY_PGUP:
       case KEY_ESC:
       case KEY_TAB:
       case KEY_CTRL:
       case KEY_PRTSC:
       case KEY_ALT:
            return CHAR_NONE;
       case KEY_1:
            return shiftEnabled ? '!' : '1';
       case KEY_2:
            return shiftEnabled ? '"' : '2';
       case KEY_3:
            return shiftEnabled ? '#' : '3';
       case KEY_4:
            return shiftEnabled ? '¤' : '4';
       case KEY_5:
            return shiftEnabled ? '%' : '5';
       case KEY_6:
            return shiftEnabled ? '&' : '6';
       case KEY_7:
            return shiftEnabled ? '/' : '7';
       case KEY_8:
            return shiftEnabled ? '(' : '8';
       case KEY_9:
            return shiftEnabled ? ')' : '9';
       case KEY_0:
            return shiftEnabled ? '=' : '0';
       case KEY_DASH:
            return shiftEnabled ? '_' : '-';
       case KEY_EQUALS:
            return '=';
       case KEY_Q:
            return capsEnabled || shiftEnabled ? 'Q' : 'q';
       case KEY_W:
            return capsEnabled || shiftEnabled ? 'W' : 'w';
       case KEY_E:
            return capsEnabled || shiftEnabled ? 'E' : 'e';
       case KEY_R:
            return capsEnabled || shiftEnabled ? 'R' : 'r';
       case KEY_T:
            return capsEnabled || shiftEnabled ? 'T' : 't';
       case KEY_Y:
            return capsEnabled || shiftEnabled ? 'Y' : 'y';
       case KEY_U:
            return capsEnabled || shiftEnabled ? 'U' : 'u';
       case KEY_I:
            return capsEnabled || shiftEnabled ? 'I' : 'i';
       case KEY_O:
            return capsEnabled || shiftEnabled ? 'O' : 'o';
       case KEY_P:
            return capsEnabled || shiftEnabled ? 'P' : 'p';
       case KEY_A:
            return capsEnabled || shiftEnabled ? 'A' : 'a';
       case KEY_S:
            return capsEnabled || shiftEnabled ? 'S' : 's';
       case KEY_D:
            return capsEnabled || shiftEnabled ? 'D' : 'd';
       case KEY_F:
            return capsEnabled || shiftEnabled ? 'F' : 'f';
       case KEY_G:
            return capsEnabled || shiftEnabled ? 'G' : 'g';
       case KEY_H:
            return capsEnabled || shiftEnabled ? 'H' : 'h';
       case KEY_J:
            return capsEnabled || shiftEnabled ? 'J' : 'j';
       case KEY_K:
            return capsEnabled || shiftEnabled ? 'K' : 'k';
       case KEY_L:
            return capsEnabled || shiftEnabled ? 'L' : 'l';
       case KEY_THEN:
            return shiftEnabled ? '>' : '<';
       case KEY_BSLASH:
            return shiftEnabled ? '\\' : '`';
       case KEY_Z:
            return capsEnabled || shiftEnabled ? 'Z' : 'z';
       case KEY_X:
            return capsEnabled || shiftEnabled ? 'X' : 'x';
       case KEY_C:
            return capsEnabled || shiftEnabled ? 'C' : 'c';
       case KEY_V:
            return capsEnabled || shiftEnabled ? 'V' : 'v';
       case KEY_B:
            return capsEnabled || shiftEnabled ? 'B' : 'b';
       case KEY_N:
            return capsEnabled || shiftEnabled ? 'N' : 'n';
       case KEY_M:
            return capsEnabled || shiftEnabled ? 'M' : 'm';
       case KEY_COMMA:
            return shiftEnabled ? ';' : ',';
       case KEY_PERIOD:
            return shiftEnabled ? ':' : '.';
       case KEY_FSLASH:
            return '/';
       case KEY_MINUS:
            return '-';
       case KEY_PLUS:
            return '+';
       default:
            return CHAR_NONE;
    }
}

// Command buffer
#define COMMAND_BUFFER_SIZE 256
static char command_buffer[COMMAND_BUFFER_SIZE];
static int cmd_buffer_pos = 0;

// Display prompt
void display_prompt() {
    terminal_printf("The...OS> ");
}

// Process a command
void process_command(const char* cmd) {
     // Implement basic commands
     if (cmd[0] == 0) {
         // Empty command
         return;

         
     }
     // Increment command count
     command_count++;
     
     // Simple command checking with string comparison
     if (strcmp(cmd, "help") == 0) {
         terminal_printf("Available commands:\n");
         terminal_printf("  help     - Display this help message\n");
         terminal_printf("  clear    - Clear the screen\n");
         terminal_printf("  version  - Display OS version\n");
         terminal_printf("  echo     - Echo back text\n");
         terminal_printf("  int0     - Test divide-by-zero interrupt\n");
         terminal_printf("  int1     - Test debug interrupt\n");
         terminal_printf("  int2     - Test NMI interrupt\n");
         terminal_printf("  sysinfo  - Shows System information\n");
     }
     // Check for echo command
     else if (strncmp(cmd, "echo ", 5) == 0) {
         terminal_printf("%s\n", cmd + 5);
     }
     // Check for clear command
     else if (strcmp(cmd, "clear") == 0) {
          // Call the clear function instead of printing newlines
          extern void terminal_clear(void);
          terminal_clear();
     }
     // Check for version command
     else if (strcmp(cmd, "version") == 0) {
         terminal_printf("myOS version 0.1\n");
     }
     // Check for int0 command
     else if (strcmp(cmd, "int0") == 0) {
         terminal_printf("Triggering divide-by-zero interrupt...\n");
         asm volatile("int $0x0");
     }
     // Check for int1 command
     else if (strcmp(cmd, "int1") == 0) {
         terminal_printf("Triggering debug interrupt...\n");
         asm volatile("int $0x1");
     }
     // Check for int2 command
     else if (strcmp(cmd, "int2") == 0) {
         terminal_printf("Triggering NMI interrupt...\n");
         asm volatile("int $0x2");
     }
     // Check for sysinfo command
     else if (strcmp(cmd, "sysinfo") == 0) {
          // Placeholders for system information
          // Will be implemented after Pit and Memory support
          int uptime_seconds = 0;
          int memory_used = 0;

          // Display system information
          terminal_printf("System Information\n");
          terminal_printf("------------------\n");
          terminal_printf("OS Name: myOS\n");
          terminal_printf("Version: 0.1\n");
          terminal_printf("Architecture: x86 (32-bit)\n");
          terminal_printf("Uptime: %d seconds\n", uptime_seconds); 
          terminal_printf("Commands executed: %d\n", command_count);
          terminal_printf("Memory used: %d KB\n", memory_used / 1024);
     }
     else {
         terminal_printf("Unknown command: %s\n", cmd);
         terminal_printf("Type 'help' for available commands\n");
     }
 }

// Keyboard controller function
void keyboard_controller(registers_t* regs, void* context) {
    unsigned char scancode = inb(0x60);
    
    // Check for buffer overflow
    if (scancode & 0x80) {
        scancode &= 0x7F;
        
        // Handle key release
        if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT) {
            shiftEnabled = false;
        }
        
        return;
    }
    
    // Check for buffer overflow
    if (scancode == KEY_ENTER) {
        terminal_printf("\n");
        
        // Check for command buffer overflow
        command_buffer[cmd_buffer_pos] = '\0';
        
        // Process the command
        process_command(command_buffer);
        
        // Clear the command buffer
        cmd_buffer_pos = 0;
        for (int i = 0; i < COMMAND_BUFFER_SIZE; i++) {
            command_buffer[i] = 0;
        }
        
        // Display the prompt again
        display_prompt();
        return;
    }
    // if space is pressed add space to command buffer
    else if (scancode == KEY_SPACE) {
        if (cmd_buffer_pos < COMMAND_BUFFER_SIZE - 1) {
            command_buffer[cmd_buffer_pos++] = ' ';
            terminal_printf(" ");
        }
        return;
    }
    // if backspace is pressed remove last character from command buffer
    else if (scancode == KEY_BACKSPACE) {
        if (cmd_buffer_pos > 0) {
            cmd_buffer_pos--;
            command_buffer[cmd_buffer_pos] = 0;
            terminal_printf("\b \b");
        }
        return;
    }
    
    // Convert scancode to ASCII
    // and add it to the command buffer
    char ascii = scancode_to_ascii(scancode);
    
     // Check for valid ASCII character
    if (ascii != CHAR_NONE && cmd_buffer_pos < COMMAND_BUFFER_SIZE - 1) {
        command_buffer[cmd_buffer_pos++] = ascii;
        terminal_printf("%c", ascii);
    }
}

// String comparison functions
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
/// String comparison with length limit
int strncmp(const char* s1, const char* s2, size_t n) {
    while (n && *s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
        --n;
    }
    if (n == 0) {
        return 0;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}



// This function initializes the keyboard and sets up the IRQ
void start_keyboard(void) {
    // Reset keyboard states
    shiftEnabled = false;
    capsEnabled = false;
    buffer_pos = 0;
    cmd_buffer_pos = 0;
    
    // Clear the command buffer
    for (int i = 0; i < COMMAND_BUFFER_SIZE; i++) {
        command_buffer[i] = 0;
    }
    
    // Enable the keyboard controller
    outb(0x21, inb(0x21) & ~(1 << 1));
    
    // Register the keyboard controller
    register_irq_controller(1, keyboard_controller, NULL);

    // Display the initial prompt
    terminal_printf("Keyboard initialized. Start typing...\n");
    display_prompt();
}