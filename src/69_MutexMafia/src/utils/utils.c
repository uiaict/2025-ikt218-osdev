#include "utils.h"


void memset(void *ptr, char value, uint32_t count){
    char *tmp = (char*)ptr;
    for (; count != 0; count--) {
        *tmp++ = value;
    }
}

//funksjon som skriver til en I/O-port
void outPortB(uint16_t port, uint8_t value)
{
    asm volatile ("outb %0, %1 " : : "a" (value), "dN" (port));
}
//funksjon som leser fra en I/O-port
uint8_t inPortB(uint16_t port){
    uint8_t value;
    asm volatile ("inb %1, %0" : "=a" (value) : "dN" (port));
    return value;
}

char get_input(char* buffer, int max_length)
{
  int index = 0;
  char c;

  while(1){
    unsigned char* scancode = read_keyboard_data_from_buffer();
    handle_key_press(scancode, buffer, &index);

    if (buffer[index-1] == '\n' || index >= max_length - 1) {
        buffer[index-1] = '\0'; 
        break;
    }
  }
}