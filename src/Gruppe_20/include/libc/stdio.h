#ifndef STDIO_H
#define STDIO_H

#include "libc/stdint.h"
#include "stdarg.h"


#ifdef __cplusplus
extern "C" {
#endif




void printf(const char* format, ...);
void vprintf(const char* format, va_list ap);

#ifdef __cplusplus
}
#endif

#endif // STDIO_H