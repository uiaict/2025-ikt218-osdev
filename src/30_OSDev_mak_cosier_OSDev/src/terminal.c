#include "libc/stdint.h"
#include "libc/teminal.h"
#include <libc/stdarg.h>  // For handling variable arguments

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define WHITE_ON_BLACK 0x0F

int cursor_x;
int cursor_y;

//u16int_t *video_memory = (u16int_t)VGA_ADDRESS;

uint16_t* terminal_buffer = (uint16_t*)VGA_ADDRESS;
int term_row = 0;
int term_col = 0;

void move_cursor()
{
   // The screen is 80 characters wide...
   uint16_t cursorLocation = cursor_y * 80 + cursor_x;
   outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
   outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
   outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
   outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Function to print a number (handles integer printing)
void print_number(int num) 
{
    char buffer[12];  // Enough for a 32-bit int (-2147483648 to 2147483647) + null terminator
    int i = 0, is_negative = 0;

    if (num == 0) 
    {
        terminal_putc('0');
        return;
    }

    if (num < 0) 
    {
        is_negative = 1;
        num = -num;  // Convert to positive
    }

    while (num > 0) 
    {
        buffer[i++] = (num % 10) + '0';  // Convert last digit to character
        num /= 10;
    }

    if (is_negative) 
    {
        buffer[i++] = '-';  // Add negative sign
    }

    buffer[i] = '\0';  // Null-terminate the string

    // Reverse the string since digits were stored in reverse order
    for (int j = 0, k = i - 1; j < k; j++, k--) 
    {
        char temp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = temp;
    }

    // Print the string one character at a time.
    for (int j = 0; buffer[j] != '\0'; j++) 
    {
        terminal_putc(buffer[j]);
    }
}

// Function to put a character on the screen
void terminal_putc(char c) 
{
    //if (c == '\n') 
    //{
    //    term_row++;
    //    term_col = 0;
    //    return;
    //}
    //terminal_buffer[term_row * VGA_WIDTH + term_col] = (WHITE_ON_BLACK << 8) | c;
    //term_col++;

     // The background colour is black (0), the foreground is white (15).
   uint8_t backColour = 0;
   uint8_t foreColour = 15;

   // The attribute byte is made up of two nibbles - the lower being the
   // foreground colour, and the upper the background colour.
   uint8_t  attributeByte = (backColour << 4) | (foreColour & 0x0F);
   // The attribute byte is the top 8 bits of the word we have to send to the
   // VGA board.
   uint16_t attribute = attributeByte << 8;
   uint16_t *location;

   // Handle a backspace, by moving the cursor back one space
   if (c == 0x08 && cursor_x)
   {
       cursor_x--;
   }

   // Handle a tab by increasing the cursor's X, but only to a point
   // where it is divisible by 8.
   else if (c == 0x09)
   {
       cursor_x = (cursor_x+8) & ~(8-1);
   }

   // Handle carriage return
   else if (c == '\r')
   {
       cursor_x = 0;
   }

   // Handle newline by moving cursor back to left and increasing the row
   else if (c == '\n')
   {
       cursor_x = 0;
       cursor_y++;
   }
   // Handle any other printable character.
   else if(c >= ' ')
   {
       location = terminal_buffer + (cursor_y*80 + cursor_x);
       *location = c | attribute;
       cursor_x++;
   }

   // Check if we need to insert a new line because we have reached the end
   // of the screen.
   if (cursor_x >= 80)
   {
       cursor_x = 0;
       cursor_y ++;
   }
}

// Function to handle formatted printing like printf
//void kprint(const char* format, ...) {
//    va_list args;
//    va_start(args, format);
//
//    // Iterate over each character in the format string
//    for (int i = 0; format[i] != '\0'; i++) {
//        if (format[i] == '%' && format[i + 1] == 'c') {
//            // Handle char argument
//            char c = (char)va_arg(args, int);  // 'int' is promoted to 'int' in va_arg
//            terminal_putc(c);  // Send to terminal
//            i++;  // Skip the 'c' in the format string
//        } 
//        else {
//            terminal_putc(format[i]);  // Print regular characters
//        }
//    }
//
//    va_end(args);
//}

void printf_string(char *c)
{
   int i = 0;
   while (c[i])
   {
    terminal_putc(c[i++]);
   }
}



void kprint(char* str, ...)
{
    va_list args;
    va_start(args, str);

    while (*str != '\0')
    {
        if (*str != '%')
        {
            terminal_putc(*str);
            str++;

            continue;
        }

        str++; 

        switch (*str)
        {
            case '%':
            {
                terminal_putc(*str);
                str++; 
                break;
            }

            case 'c':
            {
                char c = (char)va_arg(args, int);
                terminal_putc(c);
                str++;
                break;
            }

            case 's':
            {
                char* s = va_arg(args, const char*);
                while (*s != '\0')
                {
                    terminal_putc(*s);
                    *s++;
                }
                str++;
                break;
            }

            case 'd': 
            {
                int num = va_arg(args, int);
                char nstr[50];

                int_to_string(nstr, num);
                char *ptr = nstr;

                while (*ptr != '\0')
                {
                    terminal_putc(*ptr);
                    *ptr++;
                }

                str++;
                break;
            }

            /*case 'k':
            {
                int n = va_arg(args, int);
                char buffer[100]; 
                int_to_ascii(n,buffer);
                char *ptr = buffer;
                while (*ptr != '\0')
                {
                    printf_put(*ptr);
                    *ptr++;
                }
                str++;
                break;
                
            }*/


            case 'f': 
            {
                float f = va_arg(args, double);
                char nstr[50];

                float_to_string(nstr, f, 5);

                char *ptr = nstr;

                while (*ptr != '\0')
                {
                    terminal_putc(*ptr);
                    *ptr++;
                }

                str++;
                break;
            }

            case 'x':
            {
                // Get the next argument from the va_list as an unsigned integer.
                int num = va_arg(args, int);
                // Calculate the number of digits in the hexadecimal representation.
                int num_digits = 0;
                int temp = num;
                do 
                {
                    num_digits++;
                    temp /= 16;
                } 
                while (temp != 0);
                // Create a buffer to store the hexadecimal representation of the number.
                char hex_buffer[9] = {0};
                // Convert each digit of the number to a hexadecimal character.
                for (int i = num_digits - 1; i >= 0; i--) 
                {
                    int digit = (num >> (4 * i)) & 0xF;
                    if (digit < 10) 
                    {
                        hex_buffer[num_digits - i - 1] = '0' + digit;
                    } 
                    else 
                    {
                        hex_buffer[num_digits - i - 1] = 'a' + (digit - 10);
                    }
                }
                // Print the hexadecimal string to the terminal.
                for (int i = 0; i < num_digits; i++) 
                {
                    terminal_putc(hex_buffer[i]);
                }
                // Move on to the next character in the format string.
                str++;
                break;
            }
        }                    
    }
    va_end(args);
}

void int_to_string(char* str, int num)
{
    //Sets up the variable length that keeps track of the number of digits, and a variable temp that is equal to the num parameter.
    int end_index = 0;
    int temp = num;

    //If the number is 0 turn the string to 0 and return.
    if(num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    //While loop that gets the number of digits in the integer and stores it in the digits variable.
    while (temp != 0) 
    {
        end_index++;
        temp /= 10;
    }

    //If the number is negative add a minus at the start of the string and turn the number positive. Also increment the end_index variable.
    if (num < 0) 
    {
        str[0] = '-';
        num = -num;
        end_index++;
    }

    //Set the variable length as the number of digits -1, a.k.a as the length of the number.
    int length = end_index - 1;

    //While the num varieble is not 0
    while (num != 0) 
    {
        //Calculates the remainder of num divided by 10, which gives the rightmost digit of num.
        //Then adds the ASCII value of the character '0' to the remainder to convert it to the corresponding ASCII character.
        str[length--] = num % 10 + '0';

        //Updates num by dividing it by 10, which effectively removes the rightmost digit of num.
            num /= 10;
    }

    //Set the character at the end of the string as \0.
    str[end_index] = '\0';
}




void float_to_string(char* str, float f, int precision) 
{

    int integer_part = (int) f;

    float fractional_part = f - integer_part;  

    int power = 1;  

    if (f < 0) 
    {
        str[0] = '-';
        str++;
    }

    for (int p = 0; p < precision; p++) 
    {
        power *= 10;
    }

    int i = 0;


    if (integer_part == 0) 
    {
        str[i++] = '0';
    } 

    else 
    {

        while (integer_part != 0) 
        {

            int rem = integer_part % 10;

            str[i++] = rem + '0';

            integer_part /= 10;
        }
   }  

    for (int j = 0, k = i - 1; j < k; j++, k--) 
    {
        char temp = str[j];

        str[j] = str[k];

        str[k] = temp;
    }

    if (fractional_part > 0 && precision > 0) 
    {
        str[i++] = '.';

        int fractional_part_integer = (int) (fractional_part * power + 0.5);

        while (precision > 0) 
        {
            power /= 10;
            int digit = fractional_part_integer / power;

            str[i++] = digit + '0';

            fractional_part_integer -= digit * power;

            precision--;
        }
    }

    str[i] = '\0';
}