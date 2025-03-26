#include "libc/stdint.h"
#include "isr.h"
#include "libc/stdio.h"

void isr_handler(struct registers reg){
    printf("Interrupt\n\r");
}