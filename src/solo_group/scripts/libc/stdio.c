#include "stdio.h"

void test() {
    volatile char *video = (volatile char*)0xB8000;
    char[] string = "TEST";

    while( *string != 0 )
    {
        *video++ = *string++;
        *video++ = 1;
    }
}