#include "drivers/terminal.h"
#include "libc/math.h"

extern float ring_radius;
extern float donut_radius;
extern float distance;
extern float scale;

extern char buffer[HEIGHT][WIDTH + 1];
extern float zbuffer[HEIGHT][WIDTH];

void clear_buffer();

void render_donut(float a, float b);

void animate_donut();