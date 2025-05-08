#include "libc/stdio.h"
#include "libc/stdarg.h"
#include "common/monitor.h"  
#include "common/itoa.h"  

// Print a string using the monitor
void terminal_write(const char *str) {
    monitor_write(str);
}

// A minimal printf implementation with %x, %d, %s, and %p support
int printf(const char* __restrict__ format, ...) {
    va_list args;
    va_start(args, format);

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i + 1] != '\0') {
            i++;
            char buffer[32];

            switch (format[i]) {
                case 'x': {
                    unsigned int val = va_arg(args, unsigned int);
                    itoa(val, buffer, 16);        // Convert int to hex string
                    monitor_write(buffer);        // Print the string
                    break;
                }
                case 'd': {
                    int val = va_arg(args, int);
                    if (val < 0) {
                        monitor_put_char('-');
                        val = -val;
                    }
                    itoa(val, buffer, 10);         // Convert int to decimal string
                    monitor_write(buffer);
                    break;
                }
                case 'u': {
                    unsigned int val = va_arg(args, unsigned int);
                    itoa(val, buffer, 10);
                    monitor_write(buffer);
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    monitor_write(str);
                    break;
                }
                case 'p': {
                    void* ptr = va_arg(args, void*);
                    monitor_write("0x");
                    itoa((unsigned int)ptr, buffer, 16);
                    monitor_write(buffer);
                    break;
                }
                case 'c': {
                    char val = (char)va_arg(args, int); 
                    monitor_put_char(val);
                    break;
                }

                default:
                    monitor_put_char('%');
                    monitor_put_char(format[i]);
                    break;
            }
        } else {
            monitor_put_char(format[i]);  // Just a regular character
        }
    }

    va_end(args);
    return 0;
}

// Prints out the values of CS, DS, and SS segment registers
void check_segment_registers() {
    uint16_t cs, ds, ss;

    asm volatile("mov %%cs, %0" : "=r"(cs));
    asm volatile("mov %%ds, %0" : "=r"(ds));
    asm volatile("mov %%ss, %0" : "=r"(ss));

    printf("CS: 0x%x\n", cs);
    printf("DS: 0x%x\n", ds);
    printf("SS: 0x%x\n", ss);
}
