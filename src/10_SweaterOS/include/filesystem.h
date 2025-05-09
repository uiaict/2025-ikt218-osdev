#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "libc/stdint.h"

// Maximum filename length
#define MAX_FILENAME 128

// File open modes
#define FILE_READ   0x01
#define FILE_WRITE  0x02
#define FILE_APPEND 0x04

// File types
#define FILE_TYPE_REGULAR 0x01
#define FILE_TYPE_DIR     0x02

// Error codes
#define FS_SUCCESS        0
#define FS_ERROR_NOT_FOUND -1
#define FS_ERROR_EXISTS   -2
#define FS_ERROR_FULL     -3
#define FS_ERROR_INVALID  -4

// File descriptor structure
typedef struct {
    char filename[MAX_FILENAME];
    uint32_t size;
    uint32_t position;
    uint8_t mode;
    uint8_t type;
} File;

// Directory entry structure
typedef struct {
    char name[MAX_FILENAME];
    uint8_t type;
    uint32_t size;
} DirEntry;

// Filesystem functions
int fs_initialize(void);
File* fs_open(const char* filename, uint8_t mode);
int fs_close(File* file);
int fs_read(File* file, void* buffer, uint32_t size);
int fs_write(File* file, const void* buffer, uint32_t size);
int fs_seek(File* file, uint32_t position);
int fs_tell(File* file);
int fs_remove(const char* filename);
int fs_mkdir(const char* dirname);
int fs_list_dir(const char* dirname, DirEntry* entries, uint32_t max_entries);

#endif // FILESYSTEM_H 