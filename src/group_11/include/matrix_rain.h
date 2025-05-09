#ifndef MATRIX_RAIN_H
#define MATRIX_RAIN_H

#include <libc/stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define MAX_DROPS 80

typedef struct {
    int x;
    int y;
    int speed;
    int length;
    char character;
} MatrixDrop;

// Function declarations
void init_matrix_rain(void);
void update_matrix_rain(void);
void render_matrix_rain(void);

#ifdef __cplusplus
}
#endif

#endif // MATRIX_RAIN_H