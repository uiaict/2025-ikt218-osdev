#ifndef SYSTEM_H
#define SYSTEM_H



/**
 * @brief malloc.
 * @return pointer to memory address.
 */
void *malloc(int bytes);

/**
 * @brief free.
 */
void free(void* pointer);

typedef enum {
    VIDEO_BLACK = 0,
    VIDEO_BLUE = 1,
    VIDEO_GREEN = 2,
    VIDEO_CYAN = 3,
    VIDEO_RED = 4,
    VIDEO_PURPLE = 5,
    VIDEO_BROWN = 6,
    VIDEO_GRAY = 7,
    VIDEO_DARK_GRAY = 8,
    VIDEO_LIGHT_BLUE = 9,
    VIDEO_LIGHT_GREEN = 10,
    VIDEO_LIGHT_CYAN = 11,
    VIDEO_LIGHT_RED = 12,
    VIDEO_LIGHT_PURPLE = 13,
    VIDEO_YELLOW = 14,
    VIDEO_WHITE = 15
} VideoColour;
/**
 * @brief Simple print to screen function. Prints completely unformatted. Prints with white text.
 */
void printscrn(const char* string);

/**
 * @brief Simple print to screen function. Prints completely unformatted. Prints the colour specified.
 */
void printscrncol(VideoColour colour, const char* string);

#endif