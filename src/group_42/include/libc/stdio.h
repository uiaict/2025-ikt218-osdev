#ifndef STDIO_H
#define STDIO_H

/**
 * @brief Enumeration defining the available video color options.
 */
typedef enum {
  VIDEO_BLACK = 0,         /**Black color. */
  VIDEO_BLUE = 1,          /**Blue color. */
  VIDEO_GREEN = 2,         /**Green color. */
  VIDEO_CYAN = 3,          /**Cyan color. */
  VIDEO_RED = 4,           /**Red color. */
  VIDEO_PURPLE = 5,        /**Purple color. */
  VIDEO_BROWN = 6,         /**Brown color. */
  VIDEO_GRAY = 7,          /**Gray color. */
  VIDEO_DARK_GRAY = 8,     /**Dark gray color. */
  VIDEO_LIGHT_BLUE = 9,    /**Light blue color. */
  VIDEO_LIGHT_GREEN = 10,  /**Light green color. */
  VIDEO_LIGHT_CYAN = 11,   /**Light cyan color. */
  VIDEO_LIGHT_RED = 12,    /**Light red color. */
  VIDEO_LIGHT_PURPLE = 13, /**Light purple color. */
  VIDEO_YELLOW = 14,       /**Yellow color. */
  VIDEO_WHITE = 15         /**White color. */
} VideoColour;

/**
 * @brief Defines the width of the screen in characters.
 */
#define SCREEN_WIDTH 80

/**
 * @brief Defines the height of the screen in characters.
 */
#define SCREEN_HEIGHT 25

/**
 * @brief Global variable storing the current cursor position along the X-axis.
 */
extern int cursorPositionX_;

/**
 * @brief Global variable storing the current cursor position along the Y-axis.
 */
extern int cursorPositionY_;

/**
 * @brief Prints a null-terminated string to the screen in the default white
 * color.
 *
 * @param string A pointer to the null-terminated string to be printed.
 */
void print(const char *string);

/**
 * @brief Prints a null-terminated string to the screen with a specified color.
 *
 * @param string A pointer to the null-terminated string to be printed.
 * @param colour The color in which the string should be printed (VideoColour
 * enum).
 */
void printc(const char *string, VideoColour colour);

/**
 * @brief Prints a formatted string to the screen, similar to the standard
 * printf function.
 *
 * Supports the following format specifiers:
 * - %c: Character
 * - %s: String
 * - %d: Signed decimal integer
 * - %x: Unsigned hexadecimal integer (lowercase)
 * - %%: Prints a literal '%' character
 *
 * @param format A pointer to the format string.
 * @param ... Variable number of arguments to be formatted and printed.
 */
void printf(const char *format, ...);

/**
 * @brief Increments the cursor position, moving to the next character position.
 * If the end of the line is reached, it moves to the beginning of the next
 * line. If the end of the screen is reached, the cursor wraps back to the top.
 */
void incrementCursorPosition();

/**
 * @brief Converts screen coordinates (x, y) to the corresponding memory address
 * in the video memory.
 *
 * @param x The x-coordinate (column) on the screen (0 to SCREEN_WIDTH - 1).
 * @param y The y-coordinate (row) on the screen (0 to SCREEN_HEIGHT - 1).
 * @return A pointer to the memory address corresponding to the given
 * coordinates.
 */
char *cursorPosToAddress(int x, int y);

/**
 * @brief Clears a specific line on the screen by filling it with space
 * characters.
 *
 * @param line The line number to clear (0 to SCREEN_HEIGHT - 1).
 */
void clear_line(int line);

#endif