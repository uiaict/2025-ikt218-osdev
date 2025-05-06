#pragma once
#ifndef _STDDEF_H
#define _STDDEF_H

#include "libc/stdint.h"

typedef long unsigned int size_t;  // ✅ Ensure this is defined

#define NULL ((void*)0)

#endif // _STDDEF_H