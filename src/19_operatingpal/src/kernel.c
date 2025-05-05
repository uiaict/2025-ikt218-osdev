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

void show_ascii_homepage() {
    clear_screen();  

    // John Pork ASCII-art 
    printf("\n\n");
    printf("               +#*=====-=--=+++      \n");
    printf("             .+*==+=+===++**+=-@%%    \n");
    printf("            -@.=+++=====--::::  @@   \n");
    printf("           :@@ .   .-=--:....   +@%%  \n");
    printf("          %%@  *##*=--:+##+--*@  #@@ \n");
    printf("          @..-  =@%%-:-=#@@@@@@%. @@ \n");
    printf("          @ -@@@@@:.--..:%#-. .:  @  \n");
    printf("          @              ...:--=+ %% \n");
    printf("          @ -:=-  =+#=:   -+=***=. .\n");
    printf("          @.+=- ::%%.:-@@-+  .+*#.*  \n");
    printf("          :::-=:-@@@:.@%%-+ :@=--.== \n");
    printf("          *+:-+--   ::..-@@+.:=::+= \n");
    printf("          *=:-+.+@@@@@@@%%*  :++--=%%\n");
    printf("          :#-:=  ...-::--....--.:#@ \n");
    printf("           @%%--:..-.. . .::-:+=+#@  \n");
    printf("            =@#+=*++*@###%%%*===@@  \n");
    printf("             +#==-++**#*++====*@@   \n");
    printf("              @%%###*===++##%%@@@    \n");
    printf(" John Pork     @%%*-=+*#*+-:-@@      \n");
    
    // Meny
    printf("\n");
    printf("    [1] Show assignment print/test functions\n");
    printf("    [2] Start Assignment 6 Improvisation\n");
    printf("    [3] Stop song\n");
    printf("    [4] Play song\n");
    printf("\nCurrently playing: Anthem of the Soviet Union");
    printf("\nType your choice and press Enter: ");
}


void show_assignment_output(uint32_t magic, void* mb_info_addr) {
    printf("\n--- Assignment Output ---\n");

    init_kernel_memory(&end);
    init_paging();
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

    printf("\nReturning to main menu...\n");
    sleepBusy(1000);
}

void start_assignment6_demo() {
    printf("\n--- Assignment 6 Improvisation ---\n");
    printf("Here you could add animation, game, sound visualizer, etc.\n");
    sleepBusy(2000);
    printf("Returning to main menu...\n");
    sleepBusy(1000);
}

void kmain(uint32_t magic, void* mb_info_addr) {
    // Minimal init for homepage
    initDesTables();
    initKeyboard();
    initPit();

    // Start music in background
    Song song = { music_1, music_1_length };
    play_song(&song);

    // Main loop
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
                    break; // Gå tilbake til meny
                } else if (input == '2') {
                    clear_screen();
                    start_assignment6_demo();
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
