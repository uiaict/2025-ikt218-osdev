#ifndef _STORAGE_H
#define _STORAGE_H

#include "libc/stdbool.h"
#include "libc/stdint.h"

// Porter for harddisk-kommunikasjon
#define HDD_DATA           0x1F0  // Port for å lese/skrive data
#define HDD_ERROR          0x1F1  // Port for feilmeldinger
#define HDD_SECTOR_COUNT   0x1F2  // Antall sektorer å lese/skrive
#define HDD_SECTOR        0x1F3  // Hvilken sektor vi vil lese/skrive
#define HDD_CYLINDER_LOW   0x1F4  // Nedre del av sylinderposisjon
#define HDD_CYLINDER_HIGH  0x1F5  // Øvre del av sylinderposisjon
#define HDD_HEAD          0x1F6  // Hvilket lesehode som skal brukes
#define HDD_STATUS        0x1F7  // Status på harddisken
#define HDD_COMMAND       0x1F7  // Port for å sende kommandoer

// Status-bits for harddisken
#define HDD_STATUS_ERROR    0x01  // Noe gikk galt
#define HDD_STATUS_DATA     0x08  // Harddisken er klar med data
#define HDD_STATUS_BUSY     0x80  // Harddisken er opptatt

// Kommandoer vi kan sende til harddisken
#define HDD_CMD_IDENTIFY    0xEC  // Sjekk om harddisken finnes
#define HDD_CMD_READ        0x20  // Les data fra harddisk
#define HDD_CMD_WRITE       0x30  // Skriv data til harddisk

/**
 * Starter harddisk-driveren
 * 
 * Denne funksjonen initialiserer harddisken og gjør den klar til bruk.
 * Returnerer true hvis alt gikk bra, false hvis noe gikk galt.
 */
bool harddisk_start(void);

/**
 * Sjekker om harddisken finnes
 * 
 * Denne funksjonen prøver å kommunisere med harddisken for å se om den er tilkoblet.
 * Returnerer true hvis harddisken finnes, false hvis den ikke ble funnet.
 */
bool harddisk_check(void);

/**
 * Leser data fra harddisken
 * 
 * sector: Hvilken sektor vi vil lese fra (tenk på det som en adresse på disken)
 * buffer: Hvor i minnet vi skal lagre dataene vi leser
 * count: Hvor mange sektorer vi skal lese
 * Returnerer true hvis lesingen var vellykket, false hvis noe gikk galt
 */
bool harddisk_read(uint32_t sector, uint8_t* buffer, uint32_t count);

/**
 * Skriver data til harddisken
 * 
 * sector: Hvilken sektor vi vil skrive til (tenk på det som en adresse på disken)
 * buffer: Dataene vi skal skrive til disken
 * count: Hvor mange sektorer vi skal skrive
 * Returnerer true hvis skrivingen var vellykket, false hvis noe gikk galt
 */
bool harddisk_write(uint32_t sector, const uint8_t* buffer, uint32_t count);

#endif /* _STORAGE_H */ 