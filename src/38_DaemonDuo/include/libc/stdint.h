#pragma once

#ifndef _LIBC_STDINT_H
#define _LIBC_STDINT_H

#include <libc/stddef.h> // Include stddef.h for size_t

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef long unsigned int uint32_t;
typedef long long unsigned int uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

#endif /* _LIBC_STDINT_H */
