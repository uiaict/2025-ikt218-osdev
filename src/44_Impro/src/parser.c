#include "parser.h"
#include "vga.h"
#include "song/song.h"
#include "libc/stdio.h"
#include "random.h"
#include "memory.h"
#include "pit.h"


char input_buffer[INPUT_BUFFER_SIZE];
int input_index = 0;


int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}


void process_command(const char* input) {
    if (strcmp(input, "play song") == 0) {
        play_music();
    } else if (strcmp(input, "clear") == 0) {
        clear();
    }
    else if (strcmp(input, "help") == 0){
        printf("HELP! i need somebody!\n\r");
        printf("HELP! not just anybody!\n\r");
        printf("HELP! you know i need someone!\n\r");
        printf("HEEELP!\n\r");
        sleep_interrupt(3000);
        clear();
        printf("play song - To play songs\n\r");
        printf("clear - To clear screen\n\r");
        printf("random - To get a random number\n\r");
        printf("memory - To print the memory layout\n\r");
    }
    else if (strcmp(input, "random") == 0) {
        int random = rand();
        printf("Random number: %d\n\r", random);
    } else if(strcmp(input, "memory") == 0){
        print_memory_layout();
    }
    else {
        printf("Unknown command.\n\r");
    }
}
