#include "buffer.h"
#include "song.h"
#include "filesystem.h"
#include "printf.h"
#include "kernel/pit.h"
#include "nano.h"
#include "kernel/memory.h"
#include "libc/string.h"


char *old_vga;
int old_cursor;
extern int input_start;
char *currently_editing;
extern char buffer[];
extern int input_len;
extern int input_cursor;
extern int in_nano;

void open_nano(char filename[]) {
    // Save the current screen and cursor
    old_vga = malloc(VGA_WIDTH * VGA_HEIGHT * 2); // allocate memory for old screen
    memcpy(old_vga, (const void *)vga, VGA_WIDTH * VGA_HEIGHT * 2);
    old_cursor = cursor; // save the current cursor position

    clear_screen();
    printf("Editing file: %s\n", filename);

    input_start = VGA_WIDTH;

    // Clear the editing buffer
    for (int i = 0; i < INPUT_BUFFER_MAX; i++) {
        buffer[i] = 0;
    }

    // Copy filename into currently_editing
    int i = 0;
    while (filename[i] != '\0' && i < MAX_FILE_NAME_SIZE - 1) {
        currently_editing[i] = filename[i];
        i++;
    }
    currently_editing[i] = '\0'; // make sure it's null-terminated

    // If the file already exists, load its content into the buffer
    if (fs_file_exists(currently_editing)) {
        fs_print_file(currently_editing);
        fs_add_file_to_buffer(currently_editing);
    } else {
        input_len = 0; // reset buffer length
        input_cursor = 0; // reset cursor position
    }
}


void close_nano() {
    fs_save(currently_editing, buffer); // save the file

    in_nano = 0; // set in nano mode to 0
    buffer_handler(5, 0); // clear the buffer


    clear_screen();
    // paste old vga
    memcpy(vga, old_vga, VGA_WIDTH * VGA_HEIGHT * 2); // paste the old screen
    free(old_vga); // free the old screen
    cursor = old_cursor; // restore the cursor position
    printf("\naquila: ");
    input_start = cursor; // prevent deletion of "aquila: "
}
