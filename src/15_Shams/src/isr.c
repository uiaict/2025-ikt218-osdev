#include <libc/isr.h>
#include <libc/idt.h>
#include <libc/terminal.h>

extern void isr0();
extern void isr1();
extern void isr2();

#define MAX_INTERRUPTS 256

void (*interrupt_handlers[MAX_INTERRUPTS])(registers_t);

void itoa(int num, char *str, int base);

void isr_handler(registers_t regs)
{
    // Hvis det finnes en registrert handler, kall den
    if (interrupt_handlers[regs.int_no])
    {
        interrupt_handlers[regs.int_no](regs);
    }
    else
    {
        terminal_write("Received Interrupt: ");
        char buffer[10];
        itoa(regs.int_no, buffer, 10);
        terminal_write(buffer);
        terminal_write("\n");
    }
}

void isr_install()
{
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
}

void itoa(int num, char *str, int base)
{
    int i = 0;
    int isNegative = 0;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }

    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num /= base;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    int start = 0, end = i - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void register_interrupt_handler(uint8_t n, void (*handler)(registers_t))
{
    interrupt_handlers[n] = handler;
}
