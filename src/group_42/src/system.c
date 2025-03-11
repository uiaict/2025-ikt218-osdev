#include "system.h"

void *malloc(int bytes) {

    return 0;
}

void free(void *pointer) {}

void printscrn(const char *string) {
    volatile char *video = (volatile char*)0xB8000;

    while(string != 0){
        *video = *string;
        video++;
        *video = VIDEO_WHITE;
        video++;
        string++;
    }
}

void printscrncol(VideoColour colour, const char *string) {
    volatile char *video = (volatile char*)0xB8000;

    while(string != 0){
        *video = *string;
        video++;
        *video = colour;
        video++;
        string++;
    }
}
