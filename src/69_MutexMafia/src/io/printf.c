#include <libc/stdarg.h>
#include <libc/stdbool.h>
#include <libc/stddef.h>
#include <libc/stdint.h>





volatile char *video_memory = (volatile char *)0xB8000; //minneadresse til VGA tekstbuffer
int cursor = 0;

/*printer i VGA text mode
void terminal_write(const char str) {
    while (*str) {
        video_memory[cursor++] = *str++;   
        video_memory[cursor++] = 0x07;     
    }
}
*/
void putc(char c) {
    video_memory[cursor++] = c;   
    video_memory[cursor++] = 0x07;  
}

void int_to_string(int num, char *str, int base)
{
    int i = 0;
    int isNegative = 0;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    } 
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem -10) + 'a' : rem + '0'; //konverterer til ASCII
        num = num/base;
    }
    if (isNegative)
    {
        str[i++] = '-';
    }
    str[i] = '\0';

    int start = 0;
    int end = i-1;
    while (start < end)
    {
        char tmp = str[start];
        str[start] = str[end];
        str[end] = tmp;
        start++;
        end--;
    }
}

void mafiaPrint(const char *format,...)
{
    va_list args;
    va_start(args, format);
    char buffer[20];


    for (int i = 0; format[i] != '\0'; i++)
    {
        if (format[i] == '%')
        {
            i++;
    
        switch (format[i])
        {
            case 'c' : 
            {
                char c = (char) va_arg(args,int);
                putc(c);
                break;
            }

            case 's' : 
            {
                char *s = va_arg(args, char*);
                while(*s){
                    putc(*s);
                    s++;
                }
                break;
            }
            case 'i' :
            {
                int num = va_arg(args, int);
                int_to_string(num, buffer, 10);
                for (int j = 0; buffer[j] != '\0'; j++){
                putc(buffer[j]);
                }
                break;
            }
            case 'd' :
            {
                int num = va_arg(args, int);
                int_to_string(num, buffer, 10);
                for (int j = 0; buffer[j] != '\0'; j++){
                putc(buffer[j]);
                }
                break;
            }

            case 'x' :
            {
                int num = va_arg(args, int);
                int_to_string(num, buffer, 16);
                for (int j = 0; buffer[j] != '\0'; j++)
                {
                putc(buffer[j]);
                }
                break;
            }
            default:
            putc('%');
            putc(format[i]);
            break;
        }
    }else{
        putc(format[i]);
    }
    
    }
    va_end(args);
}
