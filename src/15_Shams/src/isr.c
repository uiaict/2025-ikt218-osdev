#include <libc/isr.h>
#include <libc/idt.h>
#include <libc/terminal.h>

extern void isr0();
extern void isr1();
extern void isr2();

void isr_handler(registers_t regs) {
    terminal_write("Received Interrupt: ");

    char buffer[10];
    itoa(regs.int_no, buffer, 10); // Konverter interrupt-nummeret til string
    terminal_write(buffer);
    terminal_write("\n");
}

void isr_install() {
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
}


void itoa(int num, char* str, int base) {
    int i = 0;
    int isNegative = 0;
    
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }
    
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num /= base;
    }
    
    if (isNegative)
        str[i++] = '-';
    
    str[i] = '\0';
    
    int start = 0, end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

