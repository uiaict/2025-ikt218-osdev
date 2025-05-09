#ifndef MEMORY_VIEWER_H
#define MEMORY_VIEWER_H

#include <libc/stdint.h>
#include <libc/stdbool.h>
#include <driver/include/keyboard.h>
#include <kernel/memory/memory.h>

////////////////////////////////////////
// Configuration
////////////////////////////////////////

#define MAX_MEMORY_RANGES 64

////////////////////////////////////////
// Memory Range Representation
////////////////////////////////////////

typedef struct {
    uint32_t start_addr;
    uint32_t end_addr;
    uint32_t size;
    bool is_free;
} MemoryRange;

////////////////////////////////////////
// Viewer State
////////////////////////////////////////

typedef struct {
    MemoryRange ranges[MAX_MEMORY_RANGES];
    uint32_t range_count;
    uint32_t selected_index;
    bool running;

    // Heap summary
    uint32_t heap_start;
    uint32_t heap_end;
    uint32_t heap_size;
    uint32_t used_memory;
    uint32_t free_memory;
} MemoryViewer;

////////////////////////////////////////
// Memory Viewer API
////////////////////////////////////////

void memory_viewer_init(MemoryViewer* viewer);
void memory_viewer_update(MemoryViewer* viewer);
void memory_viewer_render(MemoryViewer* viewer);
void memory_viewer_handle_input(MemoryViewer* viewer, uint8_t key);
void memory_viewer_run(MemoryViewer* viewer);
void memory_viewer_exit(MemoryViewer* viewer);
void launch_memory_viewer(void);

#endif // MEMORY_VIEWER_H
