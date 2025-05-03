#include "memvis.h"
#include "terminal.h"
#include "memory.h"
#include "libc/stdio.h"

// External declarations
extern uint32_t kernel_start;
extern uint32_t kernel_end;

// Forward declarations
static void refresh_display(void);
static void handle_key_impl(char key);
static void init_impl(void);

static MemVisConfig config = {
    .start_address = 0x00000000,
    .end_address = 0x00400000,
    .block_size = 4096,
    .selected_block = 0,
    .auto_refresh = 1,
    .show_details = 0
};

static void draw_memory_block(BlockState state) {
    switch(state) {
        case BLOCK_FREE:      printf("░"); break;
        case BLOCK_ALLOCATED: printf("▒"); break;
        case BLOCK_RESERVED:  printf("■"); break;
        case BLOCK_KERNEL:    printf("█"); break;
    }
}

static BlockState get_block_state(uint32_t address) {
    // Check if address is in kernel space
    if (address >= (uint32_t)&kernel_start && 
        address < (uint32_t)&kernel_end) {
        return BLOCK_KERNEL;
    }
    return BLOCK_FREE;
}

static void draw_memory_map(void) {
    uint32_t blocks_per_line = 32;
    uint32_t current_block = 0;
    
    for (uint32_t addr = config.start_address; 
         addr < config.end_address; 
         addr += config.block_size) {
        
        if (current_block % blocks_per_line == 0) {
            printf("\n");
        }
        
        if (current_block == config.selected_block) {
            printf("*");
        } else {
            draw_memory_block(get_block_state(addr));
        }
        
        current_block++;
    }
}

static void refresh_display(void) {
    printf("\n\nUIAOS Memory Visualizer\n");
    printf("----------------------\n");
    printf("Memory Range: 0x%08x - 0x%08x\n", 
           config.start_address, config.end_address);
    printf("Block Size: %d bytes\n", config.block_size);
    
    draw_memory_map();
    
    printf("\n\nControls:\n");
    printf("[,/.] Navigate  [Space] Details\n");
    printf("[A] Auto-Refresh: %s\n", 
           config.auto_refresh ? "ON" : "OFF");
}

static void handle_key_impl(char key) {
    switch(key) {
        case 'a':
            config.auto_refresh = !config.auto_refresh;
            break;
        case ',':  // Left
            if (config.selected_block > 0) 
                config.selected_block--;
            break;
        case '.':  // Right
            if (config.selected_block < 
                (config.end_address - config.start_address) / config.block_size - 1)
                config.selected_block++;
            break;
    }
    refresh_display();
}

static void init_impl(void) {
    refresh_display();
}

static MemVisualizer visualizer = {
    .init = init_impl,
    .refresh = refresh_display,
    .handle_key = handle_key_impl,
    .auto_refresh = 1
};

MemVisualizer* create_memory_visualizer(void) {
    return &visualizer;
}