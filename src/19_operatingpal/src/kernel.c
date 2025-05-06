#include "libc/stdio.h"
#include "libc/stdint.h"
#include "interrupts/desTables.h"
#include "drivers/keyboard.h"
#include "interrupts/pit.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "music/sound.h"
#include "music/song.h"
#include "music/notes.h"

extern uint32_t end;
extern char charBuffer[];
extern int bufferIndex;

extern Note music_1[];
extern size_t music_1_length;

bool memory_initialized = false;

// Displays ASCII art and menu
void show_ascii_homepage() {
    clear_screen();

    // Main UI with menu 
    printf("---------------------------------------------------------------------------------\n"); 
    printf("                                                                              \n"); 
    printf("  [1] Show assignment print/test functions              +#*=====-=--=+++      \n");
    printf("  [2] Keyboard Piano                                  .+*==+=+===++**+=-@%%    \n");
    printf("  [3] Stop song                                     -@.=+++=====--::::  @@   \n");
    printf("  [4] Play song                                   :@@ .   .-=--:....   +@%%  \n");
    printf("  Currently playing:                             %%@  *##*=--:+##+--*@  #@@ \n");
    printf("  Anthem of the Soviet Union                      @..-  =@%%-:-=#@@@@@@%. @@ \n");
    printf("  Type your choice and press Enter:             @ -@@@@@:.--..:%#-. .:  @  \n");
    printf("                                                @              ...:--=+ %% \n");
    printf("                                                 @ -:=-  =+#=:   -+=***=. .\n");
    printf("                                                 @.+=- ::%%.:-@@-+  .+*#.*  \n");
    printf("                                                 :::-=:-@@@:.@%%-+ :@=--.== \n");
    printf("                                                 *+:-+--   ::..-@@+.:=::+= \n");
    printf("               Group 19.                         *=:-+.+@@@@@@@%%*  :++--=%%\n");
    printf("                                                 :#-:=  ...-::--....--.:#@ \n");
    printf("                                                   @%%--:..-.. . .::-:+=+#@  \n");
    printf("                                                   =@#+=*++*@###%%%*===@@  \n");
    printf("                                                    +#==-++**#*++====*@@   \n");
    printf("                                                     @%%###*===++##%%@@@    \n");
    printf("  John Pork                                           @%%*-=+*#*+-:-@@      \n");
    printf("---------------------------------------------------------------------------------\n"); 
}

// Assignment test screen: runs malloc, paging, and sleep tests
void show_assignment_output(uint32_t magic, void* mb_info_addr) {
    printf("\n--- Assignment Output ---\n");

    if (!memory_initialized) {
        init_kernel_memory(&end);
        init_paging();
        memory_initialized = true;
    }
    
    print_memory_layout(magic, mb_info_addr);

    printf("[OK] Descriptors initialized\n");
    printf("[OK] Keyboard initialized\n");
    printf("[OK] PIT initialized\n");

    asm volatile("int $0x0");
    asm volatile("int $0x3");
    asm volatile("int $0x4");

    void* mem1 = malloc(1234);
    void* mem2 = malloc(4321);
    printf("Allocated mem1: 0x%x, mem2: 0x%x\n", (uint32_t)mem1, (uint32_t)mem2);

    for (int i = 0; i < 3; i++) {
        printf("[%d] Sleeping busy...\n", i);
        sleepBusy(1000);
        printf("[%d] Done busy sleep.\n", i);

        printf("[%d] Sleeping interrupt...\n", i);
        sleepInterrupt(1000);
        printf("[%d] Done interrupt sleep.\n", i);
    }

    printf("Press q to return to homepage.\n");
    bufferIndex = 0;
    
    while (1) {
        if (bufferIndex > 0) {
            char input = charBuffer[0];
            bufferIndex = 0;
    
            if (input == 'q') return;
        }
        asm volatile("hlt");
    }
}

// Piano mode – press keys to play notes
void piano() {
    clear_screen();
    printf("\n--- Piano Keyboard ---\n");
    printf("Press keys 1-8 to play notes:\n");
    printf("a = C4, s = D4, d = E4, f = F4\n");
    printf("g = G4, h = A4, j = B4, k = C5\n");
    printf("Press '0' to return to menu\n");
    printf("Twinkle twinkle little star: \n");
    printf("gg-ss-dd-s-aa-jj-hh-g-ss-aa-jj-h-ss-aa-jj-h-gg-ss-dd-s\n");

    bufferIndex = 0;
    while (1) {
        if (bufferIndex > 0) {
            char input = charBuffer[0];
            bufferIndex = 0;

            switch (input) {
                case 'a': play_sound(261); break; // C4
                case 's': play_sound(293); break; // D4
                case 'd': play_sound(329); break; // E4
                case 'f': play_sound(349); break; // F4
                case 'g': play_sound(392); break; // G4
                case 'h': play_sound(440); break; // A4
                case 'j': play_sound(493); break; // B4
                case 'k': play_sound(523); break; // C5
                case '0': stop_sound(); return;
                break;
            }

            sleepBusy(500);
            stop_sound();
        }

        asm volatile("hlt");
    }
}

// Kernel entry point
void kmain(uint32_t magic, void* mb_info_addr) {
    initDesTables();
    initKeyboard();
    initPit();

    // Start background music
    Song song = { music_1, music_1_length };
    play_song(&song);

    // Main menu loop
    while (1) {
        show_ascii_homepage();
        bufferIndex = 0;

        while (1) {
            if (bufferIndex > 0) {
                char input = charBuffer[0];
                bufferIndex = 0;

                if (input == '1') {
                    clear_screen();
                    show_assignment_output(magic, mb_info_addr);
                    clear_screen();
                    break;
                } else if (input == '2') {
                    clear_screen();
                    piano();
                    clear_screen();
                    break;
                } else if (input == '3') {
                    clear_screen();
                    stop_song();
                    break;
                } else if (input == '4') {
                    clear_screen();
                    Song song = { music_1, music_1_length };
                    play_song(&song);
                    break;
                } else {
                    printf("\nUnknown input: %c\n", input);
                    sleepBusy(1000);
                    break;
                }
            }
            asm volatile("hlt");
        }
    }
}
