
extern "C"{
    #include "kernel/interrupts.h"
    #include "libc/stdio.h"
    #include "libc/system.h"
    #include "common/io.h"
    #include "common/input.h"
    #include "kernel/interrupts.h"
    #include "kernel/memory.h"
    #include "kernel/pit.h"
    #include "apps/song/song.h"
    #include "apps/game/snake.h"
    #include "common/monitor.h"
    #include "kernel/interrupt_functions.h"
}


extern volatile int last_key;

// Override the C++ 'new' operator to use the kernel's malloc
void* operator new(size_t size) {
    return malloc(size);
}

// Override the C++ 'new[]' operator for allocating arrays
void* operator new[](size_t size) {
    return malloc(size);
}

// Override the C++ 'delete' operator to use the kernel's free
void operator delete(void* ptr) noexcept {
    free(ptr);
}

// Override the C++ 'delete[]' operator to free arrays
void operator delete[](void* ptr) noexcept {
    free(ptr);
}

void operator delete(void* ptr, size_t size) noexcept {
    (void)size;
    free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size;
    free(ptr);
}

void run_game_stub() {
    printf("Game feature is under construction...\n");
    printf("Press any key to return to the main menu.\n");
    while (last_key == 0) {
        sleep_busy(100);
    }
    last_key = 0;
}

extern "C" int kernel_main(void);
int kernel_main() {
    const char* options[] = {
        "Music Player",
        "Game"
    };
    const int option_count = sizeof(options) / sizeof(options[0]);
    int selected = 0;

    while (1) {
        monitor_clear();
        printf("=== Main Menu ===\n");
        printf("Use UP/DOWN arrows to select. ENTER to confirm.\n\n");

        for (int i = 0; i < option_count; i++) {
            if (i == selected) {
                printf("  > [%d] %s <\n", i + 1, options[i]);
            } else {
                printf("    [%d] %s\n", i + 1, options[i]);
            }
        }

        last_key = 0;
        while (last_key == 0) {
            sleep_busy(50);
        }

        if (last_key == 1 && selected > 0) {
            selected--; // UP
        } else if (last_key == 2 && selected < option_count - 1) {
            selected++; // DOWN
        } else if (last_key == 6) { // ENTER
            monitor_clear();
            if (selected == 0) {
                run_song_menu();
            } else if (selected == 1) {
                set_keyboard_handler_mode(2);
                snake_main();
                set_keyboard_handler_mode(0);
            }
        }

        last_key = 0;
    }
    return 0;
}
