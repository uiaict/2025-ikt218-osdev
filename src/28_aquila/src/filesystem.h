#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define MAX_FILES 100
#define MAX_FILE_SIZE 1024
#define MAX_FILE_NAME_SIZE 256

void fs_init();
void fs_ls();
void fs_print_file();
void fs_save(char filename[], char data[]);
void fs_cat(char filename[]);
int fs_file_exists(char filename[]);
void fs_add_file_to_buffer(char filename[]);


#endif