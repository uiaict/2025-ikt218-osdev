#ifndef MEMVIS_H
#define MEMVIS_H

#include "libc/stdint.h"
#include "libc/stddef.h"

// Memory block states
typedef enum {
    BLOCK_FREE,
    BLOCK_ALLOCATED,
    BLOCK_RESERVED,
    BLOCK_KERNEL
} BlockState;

// Memory visualization config
typedef struct {
    uint32_t start_address;
    uint32_t end_address;
    uint32_t block_size;
    uint32_t selected_block;
    uint8_t auto_refresh;      // Added auto_refresh flag
    uint8_t show_details;
} MemVisConfig;

// Memory visualizer interface
typedef struct {
    void (*init)(void);
    void (*refresh)(void);
    void (*handle_key)(char key);
    uint8_t auto_refresh;      // Added auto_refresh member
} MemVisualizer;

// Function declarations
MemVisualizer* create_memory_visualizer(void);

#endif // MEMVIS_H