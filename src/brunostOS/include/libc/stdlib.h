#ifndef STDLIB_H
#define STDLIB_H

void itoa(long int, char[]);            // turn int to printable string
void utoa(unsigned long int, char[]);   // turn uint to printable string
void ftoa(double, char[], int);         // turn double to printable string
void xtoa(unsigned long int, char[]);   // turn uint to printable sting in hex format

void atoi(const char[], long int*);             // turn string into int
void atou(const char[], unsigned long int*);    // turn string into unsigned int
void atof(const char[], double*);               // turn string into float

#endif // STDLIB_H