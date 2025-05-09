#pragma once
#ifndef STRING_H
#define STRING_H

#include <libc/stddef.h>

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
typedef unsigned int size_t;
#endif

size_t strlen(const char* str);

#endif // STRING_H
