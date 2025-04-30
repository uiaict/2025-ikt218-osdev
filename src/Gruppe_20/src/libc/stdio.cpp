#pragma once
#ifndef STDIO_H
#define STDIO_H

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "libc/stdarg.h"
#include "libc/system.h"
#include "libc/print.h"
#include "io.h"

#ifdef __cplusplus
extern "C" {
#endif

int putchar(int ic);


#ifdef __cplusplus
}
#endif

#endif // STDIO_H