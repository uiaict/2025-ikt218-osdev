#ifndef COMMON_H
#define COMMON_H

#include <libc/stdint.h>

// PIT and PCSPK constants (already in pit.h... but centralized? here)
#define PIT_BASE_FREQUENCY 1193180
#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL1_PORT 0x41
#define PIT_CHANNEL2_PORT 0x42
#define PC_SPEAKER_PORT 0x61

#endif
