#include "filesystem.h"
#include "printf.h"
#include "libc/string.h"
#include "kernel/pit.h"
#include "buffer.h"

extern char buffer[INPUT_BUFFER_MAX];
extern int input_len;
extern int input_cursor;

typedef struct {
    char name[MAX_FILE_NAME_SIZE];
    char data[MAX_FILE_SIZE];
} file_t;


file_t files[MAX_FILES];
int file_count = 0;

void fs_init() {
    file_t myfile = { "myfile.txt", "dette er en test fil" };
    files[file_count++] = myfile;
    file_count = 1;
}

void fs_ls() {
    printf("Files:\n");
    for (int i = 0; i < file_count; i++) {
        printf("%s\n", files[i].name);
    }
}

void fs_save(char *filename, char* data) {
    // if file exists, overwrite it
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, filename) == 0) {
            strncpy(files[i].data, data, MAX_FILE_SIZE - 1); // copy data to file
            files[i].data[strlen(data)] = '\0';
            return;
        }
    }

    // if file does not exist, create a new one
    if (file_count < MAX_FILES) {
        file_t new_file;
        filename[strlen(filename)] = '\0'; 
        data[strlen(data)] = '\0'; 
        strcpy(new_file.name, filename);
        strcpy(new_file.data, data);
        files[file_count++] = new_file;
    } else {
        printf("Error: Maximum file limit reached.\n");
    }
}

void fs_cat(char filename[]) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, filename) == 0) {
            printf("%s: %s\n", files[i].name, files[i].data);
            return;
        }
    }
    printf("File not found: %s\n", filename);
}

int fs_file_exists(char filename[]) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, filename) == 0) {
            return 1;
        }
    }
    return 0;
}

void fs_print_file(char filename[]) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, filename) == 0) {
            printf("%s", files[i].data);
            return;
        }
    }
    printf("File not found: %s\n", filename);
}

void fs_add_file_to_buffer(char filename[]) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, filename) == 0) {
            input_len = 0;
            input_cursor = 0;
            for (int j = 0; j < strlen(files[i].data); j++) {
                buffer[input_len++] = files[i].data[j];
                input_cursor++;
            }
            return;
        }
    }
}

void fs_remove(char filename[]) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, filename) == 0) {
            for (int j = i; j < file_count - 1; j++) {
                files[j] = files[j + 1];
            }
            file_count--;
            printf("File removed: %s\n", filename);
            return;
        }
    }
    printf("File not found: %s\n", filename);
}
