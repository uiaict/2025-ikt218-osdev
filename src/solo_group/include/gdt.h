#include "libc/stdint.h"

#define GDT_ENTRIES 3

struct gdtEntry {
    uint16_t limitLow;
    uint16_t baseLow;
    uint8_t baseMiddle;
    uint8_t access;
    uint8_t granularity;
    uint8_t baseHigh;
} __attribute__((packed));

struct gdtPtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void initGdt();
void gdtLoad(struct gdtPtr *gp);
void gdtSetGate(int32_t num, uint32_t base, uint32_t limit, uint8_t access,
uint8_t gran);
void gdtFlush(uint32_t gp);