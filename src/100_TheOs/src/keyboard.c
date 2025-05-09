#include "keyboard.h"
#include "common.h"
#include "interrupts.h"
#include "libc/string.h"
#include "libc/stdio.h"   
#include "libc/stdbool.h" 
#include "song/song.h"    

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
typedef enum
{
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
char scancode_to_ascii(unsigned char scancode)
{
     if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT)
     {
          shiftEnabled = true;
          return CHAR_NONE;
     }

     switch (scancode)
     {
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

// Add these CPU-related definitions
#define CPUID_VENDOR_ID        0x00000000
#define CPUID_FEATURES         0x00000001
#define CPUID_BRAND_STRING_1   0x80000002
#define CPUID_BRAND_STRING_2   0x80000003
#define CPUID_BRAND_STRING_3   0x80000004

// Structure to hold CPU information
typedef struct {
    char vendor[13]; // Vendor ID string (12 chars + null terminator)
    char brand[49];  // Brand string (48 chars + null terminator)
    bool has_mmx;
    bool has_sse;
    bool has_sse2;
    bool has_sse3;
    bool has_fpu;
    uint32_t family;
    uint32_t model;
    uint32_t stepping;
} cpu_info_t;

static cpu_info_t cpu_info;

// Function to detect CPU information
void detect_cpu() {
     uint32_t eax, ebx, ecx, edx;
    
     // Get vendor ID
     __asm__ __volatile__("cpuid"
                         : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                         : "a"(CPUID_VENDOR_ID));
    
     // Store vendor string (in EBX, EDX, ECX)
     memcpy(cpu_info.vendor, &ebx, 4);
     memcpy(cpu_info.vendor + 4, &edx, 4);
     memcpy(cpu_info.vendor + 8, &ecx, 4);
     cpu_info.vendor[12] = '\0';
    
     // Check if CPUID supports brand string (try to execute 0x80000000)
     __asm__ __volatile__("cpuid"
                          : "=a"(eax)
                          : "a"(0x80000000));
    
     if (eax >= 0x80000004) {
         // Get processor brand string
         __asm__ __volatile__("cpuid"
                             : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                             : "a"(CPUID_BRAND_STRING_1));
         memcpy(cpu_info.brand, &eax, 4);
         memcpy(cpu_info.brand + 4, &ebx, 4);
         memcpy(cpu_info.brand + 8, &ecx, 4);
         memcpy(cpu_info.brand + 12, &edx, 4);
        
         __asm__ __volatile__("cpuid"
                             : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                             : "a"(CPUID_BRAND_STRING_2));
         memcpy(cpu_info.brand + 16, &eax, 4);
         memcpy(cpu_info.brand + 20, &ebx, 4);
         memcpy(cpu_info.brand + 24, &ecx, 4);
         memcpy(cpu_info.brand + 28, &edx, 4);
        
         __asm__ __volatile__("cpuid"
                             : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                             : "a"(CPUID_BRAND_STRING_3));
         memcpy(cpu_info.brand + 32, &eax, 4);
         memcpy(cpu_info.brand + 36, &ebx, 4);
         memcpy(cpu_info.brand + 40, &ecx, 4);
         memcpy(cpu_info.brand + 44, &edx, 4);
         cpu_info.brand[48] = '\0';
     } else {
         // Instead of strcpy, manually copy "Unknown CPU"
         const char* unknown = "Unknown CPU";
         int i = 0;
         while (unknown[i] != '\0') {
             cpu_info.brand[i] = unknown[i];
             i++;
         }
         cpu_info.brand[i] = '\0';
     }
    
     // Get feature information
     __asm__ __volatile__("cpuid"
                         : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                         : "a"(CPUID_FEATURES));
    
     // Extract family, model, stepping
     cpu_info.family = (eax >> 8) & 0xF;
     cpu_info.model = (eax >> 4) & 0xF;
     cpu_info.stepping = eax & 0xF;
    
     // Check for features
     cpu_info.has_fpu = (edx & (1 << 0)) != 0;
     cpu_info.has_mmx = (edx & (1 << 23)) != 0;
     cpu_info.has_sse = (edx & (1 << 25)) != 0;
     cpu_info.has_sse2 = (edx & (1 << 26)) != 0;
     cpu_info.has_sse3 = (ecx & (1 << 0)) != 0;
 }

// Function to display CPU information
void display_cpu_info() {
    terminal_printf("Vendor: %s\n", cpu_info.vendor);
    terminal_printf("Brand: %s\n", cpu_info.brand);
    terminal_printf("Family: %d, Model: %d, Stepping: %d\n", 
                   cpu_info.family, cpu_info.model, cpu_info.stepping);
    terminal_printf("Features: ");
    if (cpu_info.has_fpu) terminal_printf("FPU ");
    if (cpu_info.has_mmx) terminal_printf("MMX ");
    if (cpu_info.has_sse) terminal_printf("SSE ");
    if (cpu_info.has_sse2) terminal_printf("SSE2 ");
    if (cpu_info.has_sse3) terminal_printf("SSE3 ");
    terminal_printf("\n");
}

// Function to display memory information
void display_memory_info() {
    terminal_printf("Memory Information\n");
    terminal_printf("------------------\n");
    print_memory_layout();
}

// Function to display OS information
void display_os_info() {
    terminal_printf("OS Information\n");
    terminal_printf("-------------\n");
    terminal_printf("OS Name: The...OS\n");
    terminal_printf("Version: 0.4\n");
    terminal_printf("Architecture: x86 (32-bit)\n");
}

// Function to display uptime information
void display_uptime_info() {
    int uptime_seconds = get_uptime_seconds();
    terminal_printf("Uptime Information\n");
    terminal_printf("-----------------\n");
    terminal_printf("System uptime: %d seconds\n", uptime_seconds);
}

// Function to display command statistics
void display_command_stats() {
    terminal_printf("Command Statistics\n");
    terminal_printf("-----------------\n");
    terminal_printf("Commands executed: %d\n", command_count);
}


// Display prompt
void display_prompt()
{
     terminal_printf("The...OS> ");
}

// Process a command with flag support
void process_command(const char *cmd)
{
    // Existing code...
    if (cmd[0] == 0)
    {
        // Empty command
        return;
    }
    // Increment command count
    command_count++;

    if (strncmp(cmd, "sysinfo", 7) == 0) {
        char flag[3] = {0};
        
        if (strlen(cmd) > 8 && cmd[7] == ' ' && cmd[8] == '-') {
            if (strlen(cmd) > 9) {
                flag[0] = cmd[9];
                
                if (strlen(cmd) > 10 && cmd[10] != ' ') {
                    flag[1] = cmd[10];
                }
            }
            
            if (strcmp(flag, "c") == 0) {
                // CPU information
                display_cpu_info();
            }
            else if (strcmp(flag, "m") == 0) {
                // Memory information
                display_memory_info();
            }
            else if (strcmp(flag, "os") == 0) {
                // OS information
                display_os_info();
            }
            else if (strcmp(flag, "up") == 0) {
                // Uptime information
                display_uptime_info();
            }
            else if (strcmp(flag, "cmd") == 0) {
                // Command statistics
                display_command_stats();
            }
            else if (strcmp(flag, "h") == 0) {
                // Available flags
                terminal_printf("Available flags: -c (CPU)\n -m (Memory)\n -os (OS)\n -up (Uptime)\n -cmd (Commands)\n");
            }
            else {
                // Unknown flag
                terminal_printf("Unknown flag: %s\n", flag);
                terminal_printf("Available flags: -c (CPU), -m (Memory), -os (OS), -up (Uptime), -cmd (Commands)\n");
            }
        }
        else {
            // No flag or invalid format, show all information
            terminal_printf("System Information\n");
            terminal_printf("==================\n\n");
            
            display_os_info();
            terminal_printf("\n");
            
            display_cpu_info();
            terminal_printf("\n");
            
            display_memory_info();
            terminal_printf("\n");
            
            display_uptime_info();
            terminal_printf("\n");
            
            display_command_stats();
        }
        return;
    }
    
    // Check for help command
    if (strcmp(cmd, "help") == 0) {
        terminal_printf("Available commands:\n");
        terminal_printf("  help     - Display this help message\n");
        terminal_printf("  clear    - Clear the screen\n");
        terminal_printf("  version  - Display OS version\n");
        terminal_printf("  echo     - Echo back text\n");
        terminal_printf("  int0     - Test divide-by-zero interrupt\n");
        terminal_printf("  int1     - Test debug interrupt\n");
        terminal_printf("  int2     - Test NMI interrupt\n");
        terminal_printf("  sysinfo  - Shows System information use flag -h for flag options\n");
        terminal_printf("  play <songname>  - Play a song (try 'play list' for options)\n");
        terminal_printf("  pitlong  - Run 10-second PIT accuracy test\n");
        terminal_printf("  memtest  - Run memory allocation tests\n");
    }
     // Check for echo command
     else if (strncmp(cmd, "echo ", 5) == 0)
     {
          terminal_printf("%s\n", cmd + 5);
     }
     // Check for clear command
     else if (strcmp(cmd, "clear") == 0)
     {
          // Call the clear function instead of printing newlines
          extern void terminal_clear(void);
          terminal_clear();
     }
     // Check for version command
     else if (strcmp(cmd, "version") == 0)
     {
         terminal_printf(" _____  _                                \n");
         terminal_printf("|_   _|| |                               \n");
         terminal_printf("  | |  | |__    ___            ___   ___ \n");
         terminal_printf("  | |  | '_ \\  / _ \\          / _ \\ / __|\n");
         terminal_printf("  | |  | | | ||  __/ _  _  _ | (_) |\\__ \\\n");
         terminal_printf("  \\_/  |_| |_| \\___|(_)(_)(_) \\___/ |___/\n");
         terminal_printf("                                         \n");
     
         terminal_printf("\nOS      : The...OS\n");
         terminal_printf("Version : 0.4\n");
     }
     
     // Check for int0 command
     else if (strcmp(cmd, "int0") == 0)
     {
          terminal_printf("Triggering divide-by-zero interrupt...\n");
          asm volatile("int $0x0");
     }
     // Check for int1 command
     else if (strcmp(cmd, "int1") == 0)
     {
          terminal_printf("Triggering debug interrupt...\n");
          asm volatile("int $0x1");
     }
     // Check for int2 command
     else if (strcmp(cmd, "int2") == 0)
     {
          terminal_printf("Triggering NMI interrupt...\n");
          asm volatile("int $0x2");
     }
     // Check for sysinfo command
     else if (strcmp(cmd, "sysinfo") == 0)
     {
          // Placeholders for system information
          // Will be implemented after Pit and Memory support
          int uptime_seconds = get_uptime_seconds();

          // Display system information
          terminal_printf("System Information\n");
          terminal_printf("------------------\n");
          terminal_printf("OS Name: The...OS\n");
          terminal_printf("Version: 0.4\n");
          terminal_printf("Architecture: x86 (32-bit)\n");
          terminal_printf("Uptime: %d seconds\n", uptime_seconds);
          terminal_printf("Commands executed: %d\n", command_count);


          print_memory_layout();
     }
     else if (strncmp(cmd, "play ", 5) == 0) {
          // Extract the song name
          const char* song_name = cmd + 5;
          
          // Display which song we're playing
          terminal_printf("Playing song: %s\n", song_name);
          
          // Create a temporary Song structure
          Song temp_song;
          
          // Match the song name and play the appropriate song
          if (strcmp(song_name, "mario") == 0) {
              temp_song.notes = mario_theme;
              temp_song.length = sizeof(mario_theme) / sizeof(Note);
              play_song_impl(&temp_song);
          }
          else if (strcmp(song_name, "starwars") == 0) {
              temp_song.notes = starwars_theme;
              temp_song.length = sizeof(starwars_theme) / sizeof(Note);
              play_song_impl(&temp_song);
          }
          else if (strcmp(song_name, "battlefield") == 0) {
              temp_song.notes = battlefield_1942_theme;
              temp_song.length = sizeof(battlefield_1942_theme) / sizeof(Note);
              play_song_impl(&temp_song);
          }
          else if (strcmp(song_name, "twinkle") == 0) {
              temp_song.notes = twinkle_twinkle;
              temp_song.length = sizeof(twinkle_twinkle) / sizeof(Note);
              play_song_impl(&temp_song);
          }
          else if (strcmp(song_name, "takeonme") == 0) {
              temp_song.notes = take_on_me;
              temp_song.length = sizeof(take_on_me) / sizeof(Note);
              play_song_impl(&temp_song);
          }
          else if (strcmp(song_name, "odetojoy") == 0) {
              temp_song.notes = ode_to_joy;
              temp_song.length = sizeof(ode_to_joy) / sizeof(Note);
              play_song_impl(&temp_song);
          }
          else if (strcmp(song_name, "tetris") == 0) {
              temp_song.notes = tetris_theme;
              temp_song.length = sizeof(tetris_theme) / sizeof(Note);
              play_song_impl(&temp_song);
          }
          else if (strcmp(song_name, "zelda") == 0) {
              temp_song.notes = zelda_theme;
              temp_song.length = sizeof(zelda_theme) / sizeof(Note);
              play_song_impl(&temp_song);
          }
          else if (strcmp(song_name, "castlevania") == 0) {
               temp_song.notes = castlevania_theme;
               temp_song.length = sizeof(castlevania_theme) / sizeof(Note);
               play_song_impl(&temp_song);
          }
          else if (strcmp(song_name, "hip-hop") == 0) {
               temp_song.notes = hiphop_melody;
               temp_song.length = sizeof(hiphop_melody) / sizeof(Note);
               play_song_impl(&temp_song);
          }
          else if (strcmp(song_name, "mario-underworld") == 0) {
               temp_song.notes = mario_underworld;
               temp_song.length = sizeof(mario_underworld) / sizeof(Note);
               play_song_impl(&temp_song);
          }
          else if (strcmp(song_name, "list") == 0) {
              // List all available songs
              terminal_printf("Available songs:\n");
              terminal_printf("  mario            - Super Mario Bros theme\n");
              terminal_printf("  starwars         - Star Wars theme\n");
              terminal_printf("  battlefield      - Battlefield 1942 theme\n");
              terminal_printf("  twinkle          - Twinkle Twinkle Little Star\n");
              terminal_printf("  takeonme         - Take On Me\n");
              terminal_printf("  odetojoy         - Ode to Joy\n");
              terminal_printf("  tetris           - Tetris theme\n");
              terminal_printf("  zelda            - Zelda theme\n");
              terminal_printf("  castlevania      - Castlevania theme\n");
              terminal_printf("  hip-hop          - Hip-hop inspired rhythm pattern\n");
              terminal_printf("  mario-underworld - Super Mario Bros Underworld theme\n");
          }
          else {
              terminal_printf("Unknown song: %s\n", song_name);
              terminal_printf("Type 'play list' to see available songs\n");
          }
      }
      else if (strcmp(cmd, "pitlong") == 0){
          terminal_printf("Running 10-second PIT accuracy test...\n");
          test_pit_10seconds();
     }
     else if (strcmp(cmd, "memtest") == 0)
     {
          terminal_printf("Running memory allocation tests...\n");
          test_memory();
     }
          else
     {
          terminal_printf("Unknown command: %s\n", cmd);
          terminal_printf("Type 'help' for available commands\n");
     }
}

// Keyboard controller function
void keyboard_controller(registers_t *regs, void *context)
{
     // Read scancode from keyboard port
     unsigned char scancode = inb(0x60);

     // Handle key release (bit 7 set)
     if (scancode & 0x80)
     {
          scancode &= 0x7F;
          if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT)
          {
               shiftEnabled = false;
          }
          return; // No need to process key releases further
     }

     // Handle special keys
     if (scancode == KEY_ENTER)
     {
          terminal_printf("\n");
          command_buffer[cmd_buffer_pos] = '\0';
          process_command(command_buffer);
          cmd_buffer_pos = 0;
          for (int i = 0; i < COMMAND_BUFFER_SIZE; i++)
          {
               command_buffer[i] = 0;
          }
          display_prompt();
          return;
     }
     else if (scancode == KEY_SPACE)
     {
          if (cmd_buffer_pos < COMMAND_BUFFER_SIZE - 1)
          {
               command_buffer[cmd_buffer_pos++] = ' ';
               terminal_printf(" ");
          }
          return;
     }
     else if (scancode == KEY_BACKSPACE)
     {
          if (cmd_buffer_pos > 0)
          {
               cmd_buffer_pos--;
               command_buffer[cmd_buffer_pos] = 0;
               monitor_backspace();
          }
          return;
     }

     // Convert scancode to ASCII
     char ascii = scancode_to_ascii(scancode);
     if (ascii != CHAR_NONE && cmd_buffer_pos < COMMAND_BUFFER_SIZE - 1)
     {
          command_buffer[cmd_buffer_pos++] = ascii;
          terminal_printf("%c", ascii);
     }
}

void start_keyboard(void)
{
     shiftEnabled = false;
     capsEnabled = false;
     buffer_pos = 0;
     cmd_buffer_pos = 0;

     // Clear command buffer
     for (int i = 0; i < COMMAND_BUFFER_SIZE; i++)
     {
          command_buffer[i] = 0;
     }

     // Enable PS/2 keyboard
     outb(0x64, 0xAE);

     // Unmask keyboard IRQ
     outb(0x21, inb(0x21) & ~(1 << 1));

     // Register keyboard handler
     register_irq_handler(1, keyboard_controller, NULL);

     // Display prompt
     display_prompt();
}