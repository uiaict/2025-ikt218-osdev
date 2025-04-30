#include "libc/stringutils.h"

//NORA LAGT TIL

// Sammenligner to strenger tegn for tegn
// Returnerer 0 hvis strengene er like, ellers forskjellen mellom første ulike tegn
// Brukes i f.eks. "if (strcmp(guess, word) == 0)"
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}


// Kopierer streng fra src til dest, inkludert '\0'
// Returnerer en peker til dest
// Brukes for å lagre navn og ord
char *strcpy(char *dest, const char *src) {
    char *ret = dest;
    while ((*dest++ = *src++));
    return ret;
}