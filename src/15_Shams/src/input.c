#include <libc/stdbool.h>
#include <libc/stdint.h>

bool capsEnabled = false;

const char large_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                            '7', '8', '9', '0', '-', '=', '\016', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                            'U', 'I', 'O', 'P', '[', ']', '\034', '?', 'A', 'S', 'D', 'F', 'G',
                            'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                            'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

const char small_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                            '7', '8', '9', '0', '-', '=', '\016', '?', 'q', 'w', 'e', 'r', 't', 'y',
                            'u', 'i', 'o', 'p', '[', ']', '\034', '?', 'a', 's', 'd', 'f', 'g',
                            'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
                            'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

char scancode_to_ascii(unsigned char *scan_code)
{
    unsigned char a = *scan_code;
    switch (a)
    {
    case 42: // Shift
    case 54: // Right Shift
    case 58: // Caps Lock
        capsEnabled = !capsEnabled;
        return 0;
    case 14: // Backspace
        return '\b';
    case 28: // Enter
        return '\n';
    case 57: // Space
        return ' ';
    default:
        if (a < 60)
        {
            return capsEnabled ? large_ascii[a] : small_ascii[a];
        }
        else
        {
            return 0;
        }
    }
}
