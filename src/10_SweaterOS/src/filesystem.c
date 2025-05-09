#include "filesystem.h"
#include "storage.h"
#include "memory_manager.h"
#include "miscFuncs.h"
#include "display.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

// Lokal implementasjon av strcmp siden vi ikke kan bruke stdlib
static int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// File system superblock structure
typedef struct {
    uint32_t magic;           // Magic number to identify filesystem
    uint32_t total_blocks;    // Total number of blocks
    uint32_t free_blocks;     // Number of free blocks
    uint32_t root_dir_block;  // Block number of root directory
} Superblock;

// Block size (same as sector size)
#define BLOCK_SIZE 512

// Magic number for filesystem identification
#define FS_MAGIC 0x53574541 // "SWEA"

// Maximum number of open files
#define MAX_OPEN_FILES 16

// File table entry
typedef struct {
    char filename[MAX_FILENAME];
    uint32_t start_block;
    uint32_t size;
    uint8_t in_use;
} FileTableEntry;

// Global variables
static Superblock sb;
static FileTableEntry file_table[MAX_OPEN_FILES];
static uint8_t fs_initialized = 0;

/**
 * Initialiserer filsystemet
 * 
 * Denne funksjonen:
 * 1. Starter harddisk-driveren
 * 2. Leser superblokken fra disken
 * 3. Oppretter et nytt filsystem hvis det ikke finnes
 */
int fs_initialize(void) {
    display_write_color("Initializing filesystem...\n", COLOR_WHITE);
    
    // Initialiser harddisk-driveren
    if (!harddisk_start()) {
        display_write_color("Could not start hard drive driver!\n", COLOR_LIGHT_RED);
        return -1;
    }
    
    // Les superblokk (første sektor)
    if (!harddisk_read(0, (uint8_t*)&sb, 1)) {
        display_write_color("Could not read superblock!\n", COLOR_LIGHT_RED);
        return -1;
    }
    
    // Sjekk om filsystemet eksisterer
    if (sb.magic != FS_MAGIC) {
        display_write_color("No filesystem found, creating new...\n", COLOR_YELLOW);
        
        // Initialiser nytt filsystem
        sb.magic = FS_MAGIC;
        sb.total_blocks = 1024; // Eksempel størrelse
        sb.free_blocks = sb.total_blocks - 2; // Reserver superblokk og rotmappe
        sb.root_dir_block = 1;
        
        // Skriv superblokk
        if (!harddisk_write(0, (const uint8_t*)&sb, 1)) {
            display_write_color("Could not write superblock!\n", COLOR_LIGHT_RED);
            return -1;
        }
        
        // Initialiser rotmappe
        uint8_t root_dir[BLOCK_SIZE] = {0};
        if (!harddisk_write(sb.root_dir_block, root_dir, 1)) {
            display_write_color("Could not initialize root directory!\n", COLOR_LIGHT_RED);
            return -1;
        }
    }
    
    // Initialiser filtabell
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        file_table[i].in_use = 0;
    }
    
    fs_initialized = 1;
    display_write_color("Filesystem initialized!\n", COLOR_LIGHT_GREEN);
    return 0;
}

/**
 * Åpner en fil
 * 
 * filnavn: Navnet på filen som skal åpnes
 * modus: Hvordan filen skal åpnes (FILE_READ, FILE_WRITE, FILE_APPEND)
 * Returnerer: Peker til fil-struktur, eller NULL hvis feil
 */
File* fs_open(const char* filename, uint8_t mode) {
    if (!fs_initialized) {
        return NULL;
    }
    
    // Finn ledig plass i filtabellen
    int free_entry = -1;
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (!file_table[i].in_use) {
            free_entry = i;
            break;
        }
    }
    
    if (free_entry == -1) {
        return NULL; // Ingen ledige filhåndtak
    }
    
    // Alloker fil-struktur
    File* file = (File*)malloc(sizeof(File));
    if (!file) {
        return NULL;
    }
    
    // Initialiser fil-struktur
    for (int i = 0; i < MAX_FILENAME && filename[i]; i++) {
        file->filename[i] = filename[i];
    }
    file->mode = mode;
    file->position = 0;
    file->size = 0;
    file->type = FILE_TYPE_REGULAR;
    
    // Merk filtabell-innslaget som i bruk
    file_table[free_entry].in_use = 1;
    
    return file;
}

/**
 * Lukker en fil
 * 
 * file: Peker til filen som skal lukkes
 * Returnerer: 0 ved suksess, negativ verdi ved feil
 */
int fs_close(File* file) {
    if (!file) {
        return FS_ERROR_INVALID;
    }
    
    // Finn og frigjør filtabell-innslaget
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (file_table[i].in_use && strcmp(file_table[i].filename, file->filename) == 0) {
            file_table[i].in_use = 0;
            break;
        }
    }
    
    free(file);
    return FS_SUCCESS;
}

/**
 * Leser fra en fil
 * 
 * file: Peker til filen som skal leses fra
 * buffer: Buffer som dataene skal leses inn i
 * size: Antall bytes som skal leses
 * Returnerer: Antall bytes lest, eller negativ verdi ved feil
 */
int fs_read(File* file, void* buffer, uint32_t size) {
    (void)size;  // Fjern advarsel om ubrukt parameter
    if (!file || !buffer || !(file->mode & FILE_READ)) {
        return FS_ERROR_INVALID;
    }
    
    // TODO: Implementer faktisk lesing fra disk
    // Foreløpig returnerer vi bare 0 bytes lest
    return 0;
}

/**
 * Skriver til en fil
 * 
 * file: Peker til filen som skal skrives til
 * buffer: Buffer med data som skal skrives
 * size: Antall bytes som skal skrives
 * Returnerer: Antall bytes skrevet, eller negativ verdi ved feil
 */
int fs_write(File* file, const void* buffer, uint32_t size) {
    (void)size;  // Fjern advarsel om ubrukt parameter
    if (!file || !buffer || !(file->mode & FILE_WRITE)) {
        return FS_ERROR_INVALID;
    }
    
    // TODO: Implementer faktisk skriving til disk
    // Foreløpig returnerer vi bare 0 bytes skrevet
    return 0;
}

/**
 * Flytter filpekeren til en bestemt posisjon
 * 
 * file: Peker til filen
 * position: Ny posisjon for filpekeren
 * Returnerer: 0 ved suksess, negativ verdi ved feil
 */
int fs_seek(File* file, uint32_t position) {
    if (!file) {
        return FS_ERROR_INVALID;
    }
    
    if (position > file->size) {
        return FS_ERROR_INVALID;
    }
    
    file->position = position;
    return FS_SUCCESS;
}

/**
 * Henter nåværende posisjon i filen
 * 
 * file: Peker til filen
 * Returnerer: Nåværende posisjon, eller negativ verdi ved feil
 */
int fs_tell(File* file) {
    if (!file) {
        return FS_ERROR_INVALID;
    }
    
    return file->position;
}

/**
 * Sletter en fil
 * 
 * filename: Navnet på filen som skal slettes
 * Returnerer: 0 ved suksess, negativ verdi ved feil
 */
int fs_remove(const char* filename) {
    if (!fs_initialized || !filename) {
        return FS_ERROR_INVALID;
    }
    
    // TODO: Implementer filsletting
    return FS_ERROR_NOT_FOUND;
}

/**
 * Oppretter en mappe
 * 
 * dirname: Navnet på mappen som skal opprettes
 * Returnerer: 0 ved suksess, negativ verdi ved feil
 */
int fs_mkdir(const char* dirname) {
    if (!fs_initialized || !dirname) {
        return FS_ERROR_INVALID;
    }
    
    // TODO: Implementer mappe-opprettelse
    return FS_ERROR_INVALID;
}

/**
 * Lister innholdet i en mappe
 * 
 * dirname: Navnet på mappen som skal listes
 * entries: Array som skal fylles med mappeinnhold
 * max_entries: Maksimalt antall innslag som kan returneres
 * Returnerer: Antall innslag funnet, eller negativ verdi ved feil
 */
int fs_list_dir(const char* dirname, DirEntry* entries, uint32_t max_entries) {
    (void)dirname;       // Fjern advarsel om ubrukt parameter
    (void)max_entries;   // Fjern advarsel om ubrukt parameter
    
    if (!fs_initialized || !entries) {
        return FS_ERROR_INVALID;
    }
    
    // TODO: Implementer mappelistning
    return 0;
} 