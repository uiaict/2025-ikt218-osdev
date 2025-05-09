#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Kernel panic */
[[noreturn]] void panic(const char* reason);

/* Utilities */
char* hex32_to_str(char buffer[], uint32_t val);
char* int32_to_str(char buffer[], int32_t val);

/* Memory operations */
void* memset(void* ptr, int value, size_t num);
void* memcpy(void* destination, const void* source, size_t num);

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_H