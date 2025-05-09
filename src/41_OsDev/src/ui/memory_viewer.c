#include <ui/memory_viewer.h>
#include <libc/stdio.h>
#include <ui/ui_common.h>
#include <driver/include/keyboard.h>

////////////////////////////////////////
// External Dependencies
////////////////////////////////////////

extern void terminal_write(const char* str);

// These come from your memory allocator (malloc.c)
extern uint32_t last_alloc;
extern uint32_t heap_end;
extern uint32_t heap_begin;
extern uint32_t pheap_begin;
extern uint32_t pheap_end;
extern uint32_t memory_used;

////////////////////////////////////////
// Global Viewer Instance
////////////////////////////////////////

static MemoryViewer global_memory_viewer;

////////////////////////////////////////
// Initialization
////////////////////////////////////////

// Initialize the memory viewer's state
void memory_viewer_init(MemoryViewer* viewer) {
    viewer->range_count = 0;
    viewer->selected_index = 0;
    viewer->running = false;

    viewer->heap_start = heap_begin;
    viewer->heap_end = heap_end;
    viewer->heap_size = heap_end - heap_begin;
    viewer->used_memory = memory_used;
    viewer->free_memory = viewer->heap_size - memory_used;
}

////////////////////////////////////////
// Heap Scanning and Range Discovery
////////////////////////////////////////

// Walk the heap and collect memory blocks into viewer->ranges
void memory_viewer_update(MemoryViewer* viewer) {
    viewer->range_count = 0;

    viewer->heap_start = heap_begin;
    viewer->heap_end = heap_end;
    viewer->heap_size = heap_end - heap_begin;
    viewer->used_memory = memory_used;
    viewer->free_memory = viewer->heap_size - memory_used;

    uint8_t* mem = (uint8_t*)heap_begin;
    while ((uint32_t)mem < last_alloc && viewer->range_count < MAX_MEMORY_RANGES) {
        alloc_t* a = (alloc_t*)mem;

        if (!a->size) break;

        MemoryRange* range = &viewer->ranges[viewer->range_count++];
        range->start_addr = (uint32_t)mem + sizeof(alloc_t);
        range->size = a->size;
        range->end_addr = range->start_addr + range->size - 1;
        range->is_free = (a->status == 0);

        mem += a->size;
        mem += sizeof(alloc_t);
        mem += 4; // Adjust for alignment padding

        if (viewer->range_count >= MAX_MEMORY_RANGES) break;
    }

    // Add page-aligned heap section if applicable
    if (viewer->range_count < MAX_MEMORY_RANGES) {
        MemoryRange* range = &viewer->ranges[viewer->range_count++];
        range->start_addr = pheap_begin;
        range->end_addr = pheap_end - 1;
        range->size = pheap_end - pheap_begin;
        range->is_free = false;  // Treated as allocated
    }
}

////////////////////////////////////////
// UI Rendering
////////////////////////////////////////

// Draw the memory viewer screen with stats and blocks
void memory_viewer_render(MemoryViewer* viewer) {
    clear_screen();

    terminal_setcursor(30, 1);
    terminal_write("MEMORY VIEWER");

    terminal_setcursor(5, 3);
    printf("Heap Start: 0x%08X", viewer->heap_start);

    terminal_setcursor(5, 4);
    printf("Heap End:   0x%08X", viewer->heap_end);

    terminal_setcursor(5, 5);
    printf("Heap Size:  %d bytes (%d KB)", viewer->heap_size, viewer->heap_size / 1024);

    terminal_setcursor(5, 6);
    printf("Used Memory: %d bytes (%d KB)", viewer->used_memory, viewer->used_memory / 1024);

    terminal_setcursor(5, 7);
    printf("Free Memory: %d bytes (%d KB)", viewer->free_memory, viewer->free_memory / 1024);

    terminal_setcursor(5, 8);
    printf("Usage: %.2f%%", ((float)viewer->used_memory / viewer->heap_size) * 100.0f);

    terminal_setcursor(5, 10);
    terminal_write("Memory Blocks:");

    terminal_setcursor(5, 11);
    terminal_write("-----------------------------------------------------------------------");

    terminal_setcursor(5, 12);
    terminal_write("  |    Start Address    |     End Address     |    Size    |  Status  ");

    terminal_setcursor(5, 13);
    terminal_write("-----------------------------------------------------------------------");

    for (size_t i = 0; i < viewer->range_count && i < 8; i++) {
        terminal_setcursor(5, 14 + i);

        if (i == viewer->selected_index) {
            terminal_write("> ");
            uint8_t old_color = terminal_getcolor();
            terminal_setcolor(0x0F); // Highlight: white on black

            printf("| 0x%08X          | 0x%08X          | %8d   | %s",
                   viewer->ranges[i].start_addr,
                   viewer->ranges[i].end_addr,
                   viewer->ranges[i].size,
                   viewer->ranges[i].is_free ? "Free     " : "Allocated");

            terminal_setcolor(old_color);
        } else {
            printf("  | 0x%08X          | 0x%08X          | %8d   | %s",
                   viewer->ranges[i].start_addr,
                   viewer->ranges[i].end_addr,
                   viewer->ranges[i].size,
                   viewer->ranges[i].is_free ? "Free     " : "Allocated");
        }
    }

    terminal_setcursor(5, 23);
    terminal_write("Use UP/DOWN arrow keys to navigate, ESC to return to main menu");
}

////////////////////////////////////////
// Input Handling
////////////////////////////////////////

// Handle user key presses
void memory_viewer_handle_input(MemoryViewer* viewer, uint8_t key) {
    switch ((uint8_t)key) {
        case KEY_UP:
            if (viewer->selected_index > 0)
                viewer->selected_index--;
            break;

        case KEY_DOWN:
            if (viewer->selected_index < viewer->range_count - 1)
                viewer->selected_index++;
            break;

        case KEY_ENTER:
            // (Optional: implement block inspection)
            break;

        case KEY_SPACE:
            // (Optional: implement paging)
            break;

        case KEY_ESC:
            memory_viewer_exit(viewer);
            break;

        default:
            break;
    }
}

////////////////////////////////////////
// Runtime Control
////////////////////////////////////////

// Run the interactive memory viewer loop
void memory_viewer_run(MemoryViewer* viewer) {
    viewer->running = true;

    while (viewer->running) {
        memory_viewer_update(viewer);
        memory_viewer_render(viewer);

        char key = (char)keyboard_get_key();
        memory_viewer_handle_input(viewer, key);
    }

    clear_screen();
}

// Stop the viewer and clean up input state
void memory_viewer_exit(MemoryViewer* viewer) {
    viewer->running = false;

    while (!keyboard_buffer_empty()) {
        keyboard_buffer_dequeue();
    }
}

// Entry point for launching from main menu
void launch_memory_viewer(void) {
    memory_viewer_init(&global_memory_viewer);
    memory_viewer_run(&global_memory_viewer);
}
