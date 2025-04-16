#include "isr.h"
#include "common.h"
//#include "keyboard.h"

isr_t interrupt_handlers[256];

void itoa(uint32_t n, char* buffer) {
    int i = 0;
    if (n == 0) {
        buffer[i++] = '0';
    } else {
        while (n != 0) {
            buffer[i++] = (n % 10) + '0'; // Get the last digit and convert it to char
            n /= 10;
        }
    }

    buffer[i] = '\0';

    // Reverse the string since the digits are stored in reverse order
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

void printf_dec(uint32_t n) {
    char buffer[12]; // Maximum of 10 digits for 32-bit integers + sign + null terminator
    itoa(n, buffer); 
    //printf(buffer);
}


// Interrupt service routine handler
void isr_handler(registers_t regs) {
    //printf("Received interrupt: ");
    int temp = regs.int_no;
    printf_dec(regs.int_no);

    
    // Additional specific interrupt handling code here
   
}

void irq_handler(registers_t regs)
{
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.

    uint8_t intno = regs.int_no & 0xFF;
    if (intno == 33)
    {
        //printf("a");
        //unsigned char scancode = inb(0x60);
        //keyboard_handler();
    }


   




   if (regs.int_no >= 40)
   {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}


void register_interrupt_handler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}