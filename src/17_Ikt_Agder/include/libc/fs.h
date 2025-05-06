#ifndef FS_H
#define FS_H

#include "stdint.h"

// Basic function to read files (memory-based or static data for simplicity)
uint8_t* read_file(const char *path, size_t *length);

#endif // FS_H
