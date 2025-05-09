#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <libc/stdio.h>

// Function for kernel panic
void panic(const char* message);