#include "draw/art.h"

#include "terminal/print.h"
#include "terminal/cursor.h"
#include "memory/heap.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/string.h"
#include "libc/string.h"

#define SCREEN_WIDTH 80
#define LAST_ROW 24
#define VGA_ADDRESS 0xB8000

#define MAX_DRAWINGS 7

static Drawing* drawing_pointers[MAX_DRAWINGS] = {NULL};
static size_t drawing_count = 0;

static bool space_available() {
    return drawing_count < MAX_DRAWINGS;
}

static bool drawings_exist() {
    return drawing_count > 0;
}

static void create_drawing(char* str) {
    Drawing* new_drawing = (Drawing *)malloc(sizeof(Drawing));
    if (new_drawing == NULL) {
        printf("Failed to allocate memory for new drawing\n");
        return;
    }

    // Memset name and board to zero
    memset(new_drawing->name, 0, sizeof(new_drawing->name));
    memset(new_drawing->board, 0, sizeof(new_drawing->board));

    // Copy standard message
    const char* message = "You can draw with keyboard letters here!";
    int i = (int) strlen(message);
    for (int j = 0; j < i; j++) {
        new_drawing->board[j] = message[j];
    }

    // Copy the name
    strncpy(new_drawing->name, str, sizeof(new_drawing->name) - 1);
    new_drawing->name[strlen(str)] = '\0';

    // Add the new drawing to the array
    drawing_pointers[drawing_count] = new_drawing;
    drawing_count++;
}

static Drawing* get_drawing(char* str) {
    for (size_t i = 0; i < drawing_count; i++) {
        if (strcmp(drawing_pointers[i]->name, str) == 0) {
            return drawing_pointers[i];
        }
    }
    return NULL;
}

static void save_drawing(Drawing* drawing) {
    uint16_t *video_memory = (uint16_t*) VGA_ADDRESS;
    for (int i = 0; i < SCREEN_SIZE; i++){
        uint16_t entry = video_memory[i];
        char c = (char)(entry & 0xFF);
        drawing->board[i] = c;
    }
}

static void print_drawing(Drawing* drawing) {
    clearTerminal();
    for (int i = 0; i < SCREEN_SIZE; i++) {
        printf("%c", drawing->board[i]);
    }
}

ArtManager *create_art_manager(void) {
    ArtManager* manager = (ArtManager *)malloc(sizeof(ArtManager));
    if (manager == NULL) {
        printf("Failed to allocate memory for ArtManager\n");
        return NULL;
    }
    manager->space_available = space_available;
    manager->drawings_exist = drawings_exist;
    manager->create_drawing = create_drawing;
    manager->fetch_drawing = get_drawing;
    manager->print_board = print_drawing;
    manager->save_drawing = save_drawing;
    return manager;
}

void destroy_art_manager(ArtManager *manager) {
    if (manager != NULL)
        free(manager);
}