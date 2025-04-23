#include "storage.h"
#include "display.h"
#include "interruptHandler.h"

// Venter til harddisken ikke er opptatt lenger
static bool wait_until_ready(void) {
    uint8_t status;
    int timeout = 100000;  // Legg til timeout
    
    while (((status = inb(HDD_STATUS)) & HDD_STATUS_BUSY) && timeout > 0) {
        io_wait();
        timeout--;
    }
    
    return timeout > 0;  // Returner false hvis timeout
}

// Venter til harddisken har data klare
static bool wait_for_data(void) {
    uint8_t status;
    int timeout = 100000;  // Legg til timeout
    
    while (!((status = inb(HDD_STATUS)) & HDD_STATUS_DATA) && timeout > 0) {
        io_wait();
        timeout--;
    }
    
    return timeout > 0;  // Returner false hvis timeout
}

/**
 * Starter harddisk-driveren
 */
bool harddisk_start(void) {
    display_write_color("Starting hard drive driver...\n", COLOR_WHITE);
    
    // Velg hovedharddisken
    outb(HDD_HEAD, 0xE0);
    io_wait();
    
    // Vent til harddisken er klar
    wait_until_ready();
    
    // Sjekk om harddisken finnes
    if (!harddisk_check()) {
        display_write_color("No hard drive found!\n", COLOR_LIGHT_RED);
        return false;
    }
    
    display_write_color("Hard drive driver is ready!\n", COLOR_LIGHT_GREEN);
    return true;
}

/**
 * Sjekker om harddisken finnes
 */
bool harddisk_check(void) {
    // Vi trenger ikke bruke buffer-dataene, bare sjekke om harddisken svarer
    
    // Velg harddisk
    outb(HDD_HEAD, 0xA0);
    io_wait();
    
    // Nullstill andre registre
    outb(HDD_SECTOR_COUNT, 0);
    outb(HDD_SECTOR, 0);
    outb(HDD_CYLINDER_LOW, 0);
    outb(HDD_CYLINDER_HIGH, 0);
    
    // Send kommando for å identifisere harddisken
    outb(HDD_COMMAND, HDD_CMD_IDENTIFY);
    io_wait();
    
    // Sjekk om harddisken svarer
    uint8_t status = inb(HDD_STATUS);
    if (status == 0) {
        return false;  // Ingen harddisk funnet
    }
    
    // Vent på at data skal bli klare
    wait_for_data();
    
    // Les og forkast identifikasjonsdata
    for (int i = 0; i < 256; i++) {
        inw(HDD_DATA);
    }
    
    return true;
}

/**
 * Leser data fra harddisken
 */
bool harddisk_read(uint32_t sector, uint8_t* buffer, uint32_t count) {
    if (!buffer || count == 0) {
        return false;
    }

    // Vent til harddisken er klar
    wait_until_ready();
    
    // Send kommandoer til harddisken
    outb(HDD_HEAD, 0xE0 | ((sector >> 24) & 0x0F));
    outb(HDD_SECTOR_COUNT, count);
    outb(HDD_SECTOR, sector & 0xFF);
    outb(HDD_CYLINDER_LOW, (sector >> 8) & 0xFF);
    outb(HDD_CYLINDER_HIGH, (sector >> 16) & 0xFF);
    outb(HDD_COMMAND, HDD_CMD_READ);
    
    // Les data fra hver sektor
    for (uint32_t s = 0; s < count; s++) {
        // Vent på at data skal bli klare
        if (!wait_for_data()) {
            return false;
        }
        
        // Status sjekk
        uint8_t status = inb(HDD_STATUS);
        if (status & HDD_STATUS_ERROR) {
            return false;
        }
        
        // Les 256 ord (512 bytes) for denne sektoren
        uint16_t* buf16 = (uint16_t*)(buffer + s * 512);
        for (int i = 0; i < 256; i++) {
            buf16[i] = inw(HDD_DATA);
        }
    }
    
    return true;
}

/**
 * Skriver data til harddisken
 */
bool harddisk_write(uint32_t sector, const uint8_t* buffer, uint32_t count) {
    if (!buffer || count == 0) {
        return false;
    }

    // Vent til harddisken er klar
    wait_until_ready();
    
    // Send kommandoer til harddisken
    outb(HDD_HEAD, 0xE0 | ((sector >> 24) & 0x0F));
    outb(HDD_SECTOR_COUNT, count);
    outb(HDD_SECTOR, sector & 0xFF);
    outb(HDD_CYLINDER_LOW, (sector >> 8) & 0xFF);
    outb(HDD_CYLINDER_HIGH, (sector >> 16) & 0xFF);
    outb(HDD_COMMAND, HDD_CMD_WRITE);
    
    // Skriv data til hver sektor
    for (uint32_t s = 0; s < count; s++) {
        // Vent på at harddisken er klar til å motta data
        if (!wait_for_data()) {
            return false;
        }
        
        // Status sjekk
        uint8_t status = inb(HDD_STATUS);
        if (status & HDD_STATUS_ERROR) {
            return false;
        }
        
        // Skriv 256 ord (512 bytes) for denne sektoren
        const uint16_t* buf16 = (const uint16_t*)(buffer + s * 512);
        for (int i = 0; i < 256; i++) {
            outw(HDD_DATA, buf16[i]);
        }
    }
    
    // Vent til harddisken er ferdig med å skrive
    wait_until_ready();
    
    return true;
} 